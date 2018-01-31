#include "Util.h"

#include "NSASM.h"

namespace NSASM {

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

	map<string, string> Util::getSegments(string var) {
		map<string, string> segs;
		string pub = "", buf = var;

		buf = formatCode(buf);
		repairBrackets(buf, "{", "}");
		repairBrackets(buf, "(", ")");
		buf = formatCode(buf);

		buf = formatLambda(buf);

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

		hash<string> strHash;
		stringstream s;
		s << hex << strHash(var);
		segs["_pub_" + s.str()] = pub;

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

}
