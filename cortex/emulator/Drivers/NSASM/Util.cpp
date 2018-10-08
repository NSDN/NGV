#include "Util.h"

#include "NSASM.h"

namespace NSASM {

	DefBlock DefBlock::getBlock(string head, string body) {
		if (head.find("<") == head.npos || head[head.length() - 1] != '>')
			return nulblk();

		DefBlock ret; size_t pos;
		pos = head.find("<");
		ret.name = head.substr(1, pos - 1);
		string arg = head.substr(pos + 1, head.find(">", pos) - pos - 1);
		ret.args = Util::parseArgs(arg, ',');
		ret.block = body;

		return ret;
	}

	DefCall DefCall::getCall(string str) {
		DefCall ret; size_t pos;
		pos = str.find("<");
		ret.name = str.substr(0, pos);
		string arg = str.substr(pos + 1, str.find(">", pos) - pos - 1);
		ret.args = Util::parseArgs(arg, ',');

		return ret;
	}

	void Util::print(int value) {
		stringstream parser;
		parser << value;
		I().Output(parser.str());
	}
	void Util::print(char value) {
		stringstream parser;
		parser << value;
		I().Output(parser.str());
	}
	void Util::print(float value) {
		stringstream parser;
		parser << value;
		I().Output(parser.str());
	}
	void Util::print(string value) {
		I().Output(value);
	}

	string Util::scan() {
		return I().Input();
		return nulstr;
	}

	void Util::cleanSymbol(string& var, string symbol, string trash) {
		cycleReplace(var, symbol + trash, symbol);
		cycleReplace(var, trash + symbol, symbol);
	}

	void Util::cleanSymbol(string& var, string symbol, string trashA, string trashB) {
		cycleReplace(var, symbol + trashA, symbol);
		cycleReplace(var, symbol + trashB, symbol);
		cycleReplace(var, trashA + symbol, symbol);
		cycleReplace(var, trashB + symbol, symbol);
	}

	void Util::cleanSymbolLeft(string& var, string symbol, string trashA, string trashB) {
		cycleReplace(var, trashA + symbol, symbol);
		cycleReplace(var, trashB + symbol, symbol);
	}

	void Util::cleanSymbolRight(string& var, string symbol, string trashA, string trashB) {
		cycleReplace(var, symbol + trashA, symbol);
		cycleReplace(var, symbol + trashB, symbol);
	}

	string Util::formatLine(string var) {
		if (var.length() == 0) return nulstr;

		cycleReplace(var, "\r", "");
		while (var[0] == '\t' || var[0] == ' ') {
			var = var.substr(1);
			if (var.length() == 0) return nulstr;
		}
		while (var[var.length() - 1] == '\t' || var[var.length() - 1] == ' ') {
			var = var.substr(0, var.length() - 1);
			if (var.length() == 0) return nulstr;
		}

		string left, right; size_t pos;
		if ((pos = var.find("\'")) != var.npos) {
			left = var.substr(0, pos);
			right = var.substr(left.length());
		} else if ((pos = var.find("\"")) != var.npos) {
			left = var.substr(0, pos);
			right = var.substr(left.length());
			if ((pos = right.substr(1).find("\"")) != right.npos && (pos + 1) < right.length()) {
				if (right.find("*", pos + 1) != right.npos) {
					cleanSymbol(right, "*", "\t", " ");
				}
			}
		} else {
			left = var;
			right = "";
		}
		cycleReplace(left, "\t", " "); cycleReplace(left, "  ", " ");
		cleanSymbol(left, ",", " "); cleanSymbol(left, "=", " ");
		cleanSymbol(left, "{", "\t", " "); cleanSymbol(left, "}", "\t", " ");
		cleanSymbol(left, "(", "\t", " "); cleanSymbol(left, ")", "\t", " ");

		cleanSymbol(left, "]", "\t", " ");

		return left + right;
	}

	string Util::formatCode(string var) {
		string buf = "", tmp = "";
		stringstream reader(var);
		while (!reader.eof()) {
			getline(reader, tmp);
			buf += formatLine(tmp + "\n");
		}
		cycleReplace(buf, "\n\n", "\n");
		reader.clear();

		cleanSymbolRight(buf, "<", "\t", " ");
		cleanSymbolLeft(buf, ">", "\t", " ");
		cleanSymbolRight(buf, "[", "\t", " ");
		cleanSymbolLeft(buf, "]", "\t", " ");

		return buf;
	}

	void Util::repairBrackets(string& var, string left, string right) {
		cycleReplace(var, "\n" + left, left);
		replace(var, left, left + "\n");
		replace(var, right, "\n" + right);
		cycleReplace(var, "\n\n", "\n");
		cycleReplace(var, left + " ", left);
		cycleReplace(var, " \n" + right, "\n" + right);
	}

	void Util::encodeLambda(string& var) {
		replace(var, "\n", "\f");
	}

	void Util::decodeLambda(string& var) {
		replace(var, "\f", "\n");
	}

	void Util::formatString(string& var) {
		replace(var, "\\\"", "\"");
		replace(var, "\\\'", "\'");
		replace(var, "\\\\", "\\");
		replace(var, "\\n", "\n");
		replace(var, "\\t", "\t");
	}

	string Util::formatLambda(string var) {
		const int IDLE = 0, RUN = 1, DONE = 2;
		int state = IDLE, count = 0, begin = 0, end = 0;
		string a = "", b = "", c = "";

		for (int i = 0; i < var.length(); i++) {
			switch (state) {
			case IDLE:
				count = begin = end = 0;
				if (var[i] == '(')
				{
					begin = i;
					count += 1;
					state = RUN;
				}
				break;
			case RUN:
				if (var[i] == '(')
					count += 1;
				else if (var[i] == ')')
					count -= 1;
				if (count == 0)
				{
					end = i;
					state = DONE;
				}
				break;
			case DONE:
				a = var.substr(0, begin);
				b = var.substr(begin, end - begin + 1);
				c = var.substr(end + 1);
				encodeLambda(b);
				var = a + b + c;
				state = IDLE;
				break;
			default:
				return var;
			}
		}

		return var;
	}

	string Util::preProcessCode(string var) {
		string buf = var;

		vector<DefBlock> blocks = getDefBlocks(buf);
		if (!blocks.empty())
			buf = doPreProcess(blocks, buf);

		if (blocks.empty() || buf == nulstr) {
			buf = var;

			buf = formatCode(buf);
			repairBrackets(buf, "{", "}");
			repairBrackets(buf, "(", ")");
			buf = formatCode(buf);

			buf = formatLambda(buf);
		}

		return buf;
	}

	map<string, string> Util::getSegments(string var) {
		map<string, string> segs;
		string pub = "";

		string buf = preProcessCode(var);

		// Here we got formated code

		stringstream reader(buf);
		string head = "", body = "", tmp;
		const int IDLE = 0, RUN = 1;
		int state = IDLE, count = 0;
		while (!reader.eof()) {
			switch (state) {
			case IDLE:
				getline(reader, head);
				count = 0; body = "";
				if (head.find("{") != head.npos) {
					replace(head, "{", "");
					count += 1;
					state = RUN;
				} else pub += (head + "\n");
				break;
			case RUN:
				if (!reader.eof()) {
					getline(reader, tmp);
					if (tmp.find("{") != tmp.npos)
						count += 1;
					else if (tmp.find("}") != tmp.npos)
						count -= 1;
					if (tmp.find("(") != tmp.npos && tmp.find(")") != tmp.npos) {
						if (tmp.find("{") != tmp.npos && tmp.find("}") != tmp.npos) 
							count -= 1;
					}
					if (count == 0) {
						segs[head] = body;
						state = IDLE;
					}
					body += (tmp + "\n");
				}
				break;
			default:
				break;
			}
		}

		segs["_pub_" + strHash(var)] = pub;

		return segs;
	}

	string Util::getSegment(string var, string head) {
		map<string, string> segs = getSegments(var);
		string seg = "nop";

		for (auto it = segs.begin(); it != segs.end(); it++) {
			if (it->first == head) {
				if (seg == "nop")
					seg = it->second;
				else
					return nulstr;
			}
		}

		return seg;
	}

	vector<string> Util::parseArgs(string str, char split) {
		vector<string> args;

		const int IDLE = 0, RUN = 1;
		int state = IDLE;
		string buf = "";
		char old, now = '\0';
		for (int i = 0; i < str.length(); i++) {
			old = now;
			now = str[i];
			switch (state) {
			case IDLE:
				if (now == split) {
					args.push_back(buf);
					buf = "";
					continue;
				}
				if (now == ' ' || now == '\t')
					continue;
				buf += now;
				if (now == '\'' || now == '\"')
					state = RUN;
				break;
			case RUN:
				buf += now;
				if (now == '\'' || now == '\"')
					if (old != '\\')
						state = IDLE;
				break;
			default:
				break;
			}
		}

		if (state == IDLE && buf.length() != 0)
			args.push_back(buf);

		return args;
	}

	vector<DefBlock> Util::getDefBlocks(string var) {
		vector<DefBlock> blocks;
		string buf = var;

		buf = formatCode(buf);
		repairBrackets(buf, "{", "}");
		repairBrackets(buf, "(", ")");
		buf = formatCode(buf);

		buf = formatLambda(buf);

		stringstream reader(buf); DefBlock blk;
		string head = "", body = "", tmp;
		const int IDLE = 0, RUN = 1;
		int state = IDLE, count = 0;
		while (!reader.eof()) {
			switch (state) {
			case IDLE:
				getline(reader, head);
				count = 0; body = "";
				if (head.find("{") != head.npos) {
					replace(head, "{", "");
					count += 1;
					state = RUN;
				}
				break;
			case RUN:
				if (!reader.eof()) {
					getline(reader, tmp);
					if (tmp.find("{") != tmp.npos)
						count += 1;
					else if (tmp.find("}") != tmp.npos)
						count -= 1;
					if (tmp.find("(") != tmp.npos && tmp.find(")") != tmp.npos) {
						if (tmp.find("{") != tmp.npos && tmp.find("}") != tmp.npos)
							count -= 1;
					}
					if (count == 0) {
						if (head[0] == '.' && head[1] != '<') {
							blk = DefBlock::getBlock(head, body);
							if (blk == DefBlock::nulblk()) {
								print("Error at: \"" + head + "\"\n\n");
								blocks.clear();
								return blocks;
							}
							blocks.push_back(blk);
						}
						state = IDLE;
					}
					body += (tmp + "\n");
				}
				break;
			default:
				break;
			}
		}

		return blocks;
	}

	string Util::doPreProcess(vector<DefBlock> blocks, string var) {
		string buf = var;

		buf = formatCode(buf);
		repairBrackets(buf, "{", "}");
		repairBrackets(buf, "(", ")");
		buf = formatCode(buf);

		buf = formatLambda(buf);

		stringstream reader(buf);
		string tmp = "", line, block;
		DefCall call; bool defRes; size_t pos;
		while (!reader.eof()) {
			getline(reader, line);
			if ((pos = line.find("<")) != line.npos && line[0] != '<' && line[line.length() - 1] == '>' && line.substr(0, pos).find(" ") == string::npos) {
				call = DefCall::getCall(line); defRes = false;
				for (DefBlock blk : blocks) {
					if (blk.name == call.name)
						if (blk.args.size() == call.args.size()) {
							block = blk.block;
							for (int i = 0; i < call.args.size(); i++) {
								replace(block, blk.args[i] + ",", call.args[i] + ",");
								replace(block, blk.args[i] + "\n", call.args[i] + "\n");
							}
							tmp += (block + "\n");
							defRes = true;
							break;
						}
				}
				if (!defRes) {
					print("Error at: \"" + line + "\"\n\n");
					return nulstr;
				}
			} else {
				tmp += (line + "\n");
			}
		}

		buf = tmp;
		buf = formatCode(buf);

		return buf;
	}

	string Util::read(string path) {
		stringstream reader;
		try {
			reader << I().FileInput(path);
		} catch (exception e) {
			print("File open failed.\n");
			print("At file: " + path + "\n\n");
			return nulstr;
		}

		string str = "", tmp;
		try {
			while (!reader.eof()) {
				getline(reader, tmp);
				str += (tmp + "\n");
			}
		} catch (exception e) {
			print("File read error.\n");
			print("At file: " + path + "\n\n");
			return nulstr;
		}

		return str;
	}

	void Util::run(string path) {
		if (path.find(".nsb") == path.length() - 4) {
			binary(path);
			return;
		}

		string str = read(path);
		if (str == nulstr) return;

		int heap = 64, stack = 32, regs = 16;

		string conf = getSegment(str, ".<conf>");
		if (conf == nulstr) {
			print("Conf load error.\n");
			print("At file: " + path + "\n\n");
			return;
		}
		if (conf.size() > 0) {
			stringstream reader(conf), parser;
			string head, buf; size_t pos;
			try {
				while (!reader.eof()) {
					getline(reader, buf);
					if ((pos = buf.find(' ')) != buf.npos) {
						head = buf.substr(0, pos);
						parser.clear();
						parser << buf.substr(pos + 1);
						if (head == "heap") parser >> heap;
						else if (head == "stack") parser >> stack;
						else if (head == "reg") parser >> regs;
					}
				}
			} catch (exception e) {
				print("Conf load error.\n");
				print("At file: " + path + "\n\n");
				return;
			}
		}

		map<string, string> code = getSegments(str);
		NSASM nsasm(heap, stack, regs, code);
		nsasm.run();
		print("\nNSASM running finished.\n\n");
	}

	void Util::execute(string str) {
		string path = "local";
		if (str == nulstr) return;

		int heap = 64, stack = 32, regs = 16;

		string conf = getSegment(str, ".<conf>");
		if (conf == nulstr) {
			print("Conf load error.\n");
			print("At file: " + path + "\n\n");
			return;
		}
		if (conf.size() > 0) {
			stringstream reader(conf), parser;
			string head, buf; size_t pos;
			try {
				while (!reader.eof()) {
					getline(reader, buf);
					if ((pos = buf.find(' ')) != buf.npos) {
						head = buf.substr(0, pos);
						parser.clear();
						parser << buf.substr(pos + 1);
						if (head == "heap") parser >> heap;
						else if (head == "stack") parser >> stack;
						else if (head == "reg") parser >> regs;
					}
				}
			} catch (exception e) {
				print("Conf load error.\n");
				print("At file: " + path + "\n\n");
				return;
			}
		}

		map<string, string> code = getSegments(str);
		NSASM nsasm(heap, stack, regs, code);
		nsasm.run();
		print("\nNSASM running finished.\n\n");
	}

	void Util::interactive() {
		print("Now in console mode.\n\n");
		string buf; int lines = 1;
		NSASM::Result result;

		map<string, string> code = getSegments("nop\n"); //ld func allowed
		NSASM nsasm(64, 32, 16, code);

		while (true) {
			print(lines); print(" >>> ");
			buf = scan();
			if (buf.length() == 0) {
				lines += 1;
				continue;
			}
			buf = formatLine(buf);

			if (buf.find("#") != buf.npos) {
				print("<" + buf + ">");
				continue;
			}
			result = nsasm.execute(buf);
			if (result == NSASM::Result::RES_ERR) {
				print("\nNSASM running error!\n");
				print("At line "); print(lines); print(": " + buf + "\n\n");
			} else if (result == NSASM::Result::RES_ETC) {
				break;
			}
			if (buf.find("run") == 0 || buf.find("call") == 0) {
				nsasm.run();
			}

			lines += 1;
		}
	}

	void Util::putToList(vector<unsigned char>& list, unsigned short value) {
		list.push_back((unsigned char)value);
		list.push_back((unsigned char)(value >> 8));
	}

	void Util::putToList(vector<unsigned char>& list, string value) {
		for (int i = 0; i < value.length(); i++)
			list.push_back((unsigned char)value[i]);
	}

	string Util::compile(string inPath, string outPath) {
		string str = read(inPath);
		if (str == nulstr) return nulstr;

		if (outPath == nulstr) return preProcessCode(str);

		unsigned short heap = 64, stack = 32, regs = 16;

		string conf = getSegment(str, ".<conf>");
		if (conf == nulstr) {
			print("Conf load error.\n");
			print("At file: " + inPath + "\n\n");
			return nulstr;
		}
		if (conf.size() > 0) {
			stringstream reader(conf), parser;
			string head, buf; size_t pos;
			try {
				while (!reader.eof()) {
					getline(reader, buf);
					if ((pos = buf.find(' ')) != buf.npos) {
						head = buf.substr(0, pos);
						parser.clear();
						parser << buf.substr(pos + 1);
						if (head == "heap") parser >> heap;
						else if (head == "stack") parser >> stack;
						else if (head == "reg") parser >> regs;
					}
				}
			}
			catch (exception e) {
				print("Conf load error.\n");
				print("At file: " + inPath + "\n\n");
				return nulstr;
			}
		}

		map<string, string> code = getSegments(str);
		unsigned short segCnt = code.size();

		vector<unsigned char> bytes;
		putToList(bytes, "NS");
		putToList(bytes, 0xFFFF);
		putToList(bytes, heap);
		putToList(bytes, stack);
		putToList(bytes, regs);
		putToList(bytes, segCnt);

		for (auto seg : code) {
			putToList(bytes, 0xA5A5);
			putToList(bytes, seg.first);
			putToList(bytes, 0xAAAA);
			putToList(bytes, seg.second);
		}

		putToList(bytes, 0xFFFF);
		unsigned short sum = 0;
		for (int i = 0; i < bytes.size(); i++)
			sum += bytes[i];
		putToList(bytes, sum);

		try {
			I().BinaryOutput(outPath, bytes);
		} catch (exception e) {
			print("File write failed.\n");
			print("At file: " + outPath + "\n\n");
			return nulstr;
		}

		return preProcessCode(str);
	}

	unsigned short Util::getUint16(vector<unsigned char> data, int offset) {
		unsigned short res = 0;
		if (data.size() >= offset + 1)
			res = (data[offset] | (data[offset + 1] << 8));
		return res;
	}

	string Util::getStr2(vector<unsigned char> data, int offset) {
		string res = "";
		if (data.size() >= offset + 1) {
			res += (char)data[offset];
			res += (char)data[offset + 1];
		}
		return res;
	}

	void Util::binary(string path) {
		vector<unsigned char> data = I().BinaryInput(path);
		if (data.size() < 16) return;

		if (getStr2(data, 0) != "NS") return;
		if (getUint16(data, 2) != 0xFFFF) return;

		unsigned short sum = 0;
		for (int i = 0; i < data.size() - 2; i++)
			sum += data[i];
		if (sum != getUint16(data, data.size() - 2))
			return;

		unsigned short heap, stack, regs, segCnt;
		heap = getUint16(data, 4);
		stack = getUint16(data, 6);
		regs = getUint16(data, 8);
		segCnt = getUint16(data, 10);

		int offset = 12, segPos = -1;
		map<string, string> code;
		
		vector<string> segName, segCode;
		segName.resize(segCnt); segCode.resize(segCnt);

		const int SEG_NAME = 0, SEG_CODE = 1;
		int state = SEG_NAME, offmax = data.size() - 1;
		while (offset <= offmax - 4) {
			if (getUint16(data, offset) == 0xA5A5) {
				if (segPos >= segCnt - 1)
					return;
				segPos += 1; offset += 2;
				state = SEG_NAME;
				segName[segPos] = "";
			}
			else if (getUint16(data, offset) == 0xAAAA) {
				offset += 2;
				state = SEG_CODE;
				segCode[segPos] = "";
			}
			else {
				if (state == SEG_NAME)
					segName[segPos] += (char)data[offset];
				else if (state == SEG_CODE)
					segCode[segPos] += (char)data[offset];
				offset += 1;
			}
		}

		if (getUint16(data, offset) != 0xFFFF) return;

		for (int i = 0; i < segCnt; i++)
			code[segName[i]] = segCode[i];

		NSASM nsasm(heap, stack, regs, code);
		nsasm.run();
		print("\nNSASM running finished.\n\n");
	}

}
