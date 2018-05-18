#include "NSASM.h"

#include <algorithm>
#include "Util.h"

template <typename T>
inline T* _gc(T* v) { delete v; return nullptr; }

namespace NSASM {

	NSASM::NSASM(int heapSize, int stackSize, int regCnt, map<string, string>& code) {
		heapManager.clear();
		stackManager.empty();

		this->heapSize = heapSize;
		this->stackSize = stackSize;
		
		stateReg.type = RegType::REG_INT;
		stateReg.readOnly = false;
		stateReg.n.i = 0;

		backupReg.empty();
		progSeg = 0; progCnt = 0;
		tmpSeg = -1; tmpCnt = -1;

		Register* ptr;
		for (int i = 0; i < regCnt; i++) {
			ptr = new Register();
			ptr->type = RegType::REG_INT;
			ptr->readOnly = false;
			ptr->n.i = 0;
			regGroup.push_back(*ptr);
		}

		funcList.clear();
		loadFuncList();

		segs.clear();
		this->code.clear();
		if (appendCode(code) == Result::RES_ERR) {
			string path = "_main_";
			Util::print("At file: " + path + "\n\n");
			code.clear();
		}
	}
	NSASM::NSASM(NSASM& super, map<string, string>& code) : NSASM::NSASM(super.heapSize, super.stackSize, super.regGroup.size(), code) {
		copyRegGroup(super);
	}
	NSASM::~NSASM() {
		heapManager.clear();
		stackManager.empty();
		backupReg.empty();
		regGroup.clear();
		funcList.clear();
		code.clear();
	}

	bool NSASM::verifyBound(string var, char left, char right) {
		return var[0] == left && var[var.length() - 1] == right;
	}
	bool NSASM::verifyWord(string var, WordType type) {
		switch (type) {
		case WordType::WD_REG:
			return var[0] == 'r' || var[0] == 'R';
		case WordType::WD_CHAR:
			return verifyBound(var, '\'', '\'');
		case WordType::WD_STR:
			size_t pos; bool state;
			state = verifyBound(var, '\"', '\"');
			if ((pos = var.find('\"')) != var.npos) {
				if (var.find('\"', pos + 1) != var.npos) {
					state |= var.find('*') != var.npos;
				}
			}
			return state;
		case WordType::WD_INT:
			if (var[var.length() - 1] == 'f' || var[var.length() - 1] == 'F') {
				return var.find("0x") == 0 || var.find("0X") == 0;
			}
			return var.find('.') == var.npos && (
				(var[0] >= '0' && var[0] <= '9') ||
				var[0] == '-' || var[0] == '+' ||
				var[var.length() - 1] == 'h' || var[var.length() - 1] == 'H'
			);
		case WordType::WD_FLOAT:
			return (
				var.find('.') != var.npos ||
				var[var.length() - 1] == 'f' || var[var.length() - 1] == 'F'
			) && (
				(var[0] >= '0' && var[0] <= '9') ||
				var[0] == '-' || var[0] == '+'
			) && (var.find("0x") != 0 || var.find("0X") != 0);
		case WordType::WD_TAG:
			return verifyBound(var, '[', ']');
		case WordType::WD_SEG:
			return verifyBound(var, '<', '>');
		case WordType::WD_CODE:
			return verifyBound(var, '(', ')');
		case WordType::WD_VAR:
			return !verifyWord(var, WordType::WD_REG) && !verifyWord(var, WordType::WD_CHAR) &&
				!verifyWord(var, WordType::WD_STR) && !verifyWord(var, WordType::WD_INT) &&
				!verifyWord(var, WordType::WD_FLOAT) && !verifyWord(var, WordType::WD_TAG) &&
				!verifyWord(var, WordType::WD_SEG) && !verifyWord(var, WordType::WD_CODE);
		}
		return false;
	}
	NSASM::Register* NSASM::getRegister(string var) {
		if (var.length() == 0) return nullptr;

		stringstream parser;
		if (verifyWord(var, WordType::WD_REG)) {
			// Register
			int index = -1;
			parser.clear(); parser << var.substr(1);
			parser >> index;
			if (index < 0 || index >= regGroup.size()) return nullptr;
			return &regGroup[index];
		} else if (verifyWord(var, WordType::WD_VAR)) {
			// Variable
			if (heapManager.count(var) == 0) return nullptr;
			return &heapManager[var];
		} else {
			// Immediate number
			Register* reg = nullptr;
			if (verifyWord(var, WordType::WD_CHAR)) {
				if (var.length() < 3) return nullptr;
				char tmp = (char)0;
				if (var[1] == '\\') {
					if (var.length() < 4) return nullptr;
					switch (var[2]) {
					case '0': tmp = '\0'; break;
					case 'b': tmp = '\b'; break;
					case 'n': tmp = '\n'; break;
					case 'r': tmp = '\r'; break;
					case 't': tmp = '\t'; break;
					case '\\': tmp = '\\'; break;
					}
				} else {
					tmp = var[1];
				}

				reg = new Register();
				reg->type = RegType::REG_CHAR;
				reg->readOnly = true;
				reg->gcFlag = true;
				reg->n.c = tmp;
			} else if (verifyWord(var, WordType::WD_STR)) {
				if (var.length() < 3) return nullptr;
				string tmp, rep; size_t pos;
				if ((pos = var.find("\"*")) != var.npos) {
					tmp = rep = var.substr(0, pos).substr(1);
					Register* repeat = getRegister(var.substr(pos + 2));
					if (repeat == nullptr) return nullptr;
					if (repeat->type != RegType::REG_INT) {
						if (repeat->gcFlag) return _gc(repeat);
						else return nullptr;
					}
					for (int i = 1; i < repeat->n.i; i++)
						tmp += rep;
					if (repeat->gcFlag) _gc(repeat);
				} else {
					tmp = var.substr(1, var.length() - 2);
				}

				Util::formatString(tmp);
				reg = new Register();
				reg->type = RegType::REG_STR;
				reg->readOnly = true;
				reg->gcFlag = true;
				reg->s = tmp;
			} else if (verifyWord(var, WordType::WD_INT)) {
				int tmp; string buf = var;
				if (
					(var.find("x") != var.npos || var.find("X") != var.npos) ^
					(var.find("h") != var.npos || var.find("H") != var.npos)
				) {
					if (
						(var.find("x") != var.npos || var.find("X") != var.npos) &&
						(var.find("h") != var.npos || var.find("H") != var.npos)
					) return nullptr;
					Util::replace(buf, "h", ""); Util::replace(buf, "H", "");
					Util::replace(buf, "x", ""); Util::replace(buf, "X", "");
					parser.clear(); parser << buf; parser >> hex >> tmp;
				} else {
					parser.clear(); parser << buf; parser >> dec >> tmp;
				}

				reg = new Register();
				reg->type = RegType::REG_INT;
				reg->readOnly = true;
				reg->gcFlag = true;
				reg->n.i = tmp;
			} else if (verifyWord(var, WordType::WD_FLOAT)) {
				float tmp; string buf = var;
				Util::replace(buf, "f", ""); Util::replace(buf, "F", "");
				parser.clear(); parser << buf; parser >> tmp;

				reg = new Register();
				reg->type = RegType::REG_FLOAT;
				reg->readOnly = true;
				reg->gcFlag = true;
				reg->n.f = tmp;
			} else if (verifyWord(var, WordType::WD_TAG) || verifyWord(var, WordType::WD_SEG)) {
				reg = new Register();
				reg->type = RegType::REG_STR;
				reg->readOnly = true;
				reg->gcFlag = true;
				reg->s = var;
			} else if (verifyWord(var, WordType::WD_CODE)) {
				string code = var.substr(1, var.length() - 2);

				reg = new Register();
				reg->type = RegType::REG_CODE;
				reg->readOnly = true;
				reg->gcFlag = true;
				Util::decodeLambda(code);
				reg->s = code;
			} else return nullptr;

			return reg;
		}
	}
	vector<string> NSASM::convToArray(string var) {
		stringstream reader(var); string buf;
		vector<string> result;
		while (!reader.eof()) {
			getline(reader, buf);
			result.push_back(buf);
		}
		return result;
	}
	NSASM::Result NSASM::appendCode(map<string, string>& code) {
		if (code.size() == 0) return Result::RES_OK;
		if (this->code.empty()) { // Load pub seg
			for (auto it = code.begin(); it != code.end(); it++) {
				if (it->first.find("_pub_") == 0) {
					this->segs.push_back(it->first);
					this->code[it->first] = convToArray(it->second);
					break;
				}
			}
		}
		for (auto it = code.begin(); it != code.end(); it++) {
			if (it->first[0] == '.') continue; // This is conf seg
			if (it->first[0] == '@') { // This is override seg
				if (this->code.count(it->first.substr(1)) == 0) {
					Util::print("\nNSASM loading error!\n");
					Util::print("At " + it->first.substr(1) + "\n");
					return Result::RES_ERR;
				}
				this->segs.push_back(it->first.substr(1));
				this->code[it->first.substr(1)] = convToArray(it->second);
			} else {
				if (this->code.count(it->first) > 0) {
					if (it->first.find("_pub_") == 0) continue; // This is pub seg
					Util::print("\nNSASM loading error!\n");
					Util::print("At " + it->first + "\n");
					return Result::RES_ERR;
				}
				this->segs.push_back(it->first);
				this->code[it->first] = convToArray(it->second);
			}
		}
		return Result::RES_OK;
	}
	void NSASM::copyRegGroup(NSASM& super) {
		for (int i = 0; i < super.regGroup.size(); i++)
			this->regGroup[i] = super.regGroup[i];
	}

	NSASM::Result NSASM::calc(int* dst, int src, char type) {
		switch (type) {
		case '+': *dst += src; break;
		case '-': *dst -= src; break;
		case '*': *dst *= src; break;
		case '/': *dst /= src; break;
		case '&': *dst &= src; break;
		case '|': *dst |= src; break;
		case '~': *dst = ~(*dst); break;
		case '^': *dst ^= src; break;
		case '<': *dst = *dst << src; break;
		case '>': *dst = *dst >> src; break;
		default: return Result::RES_ERR;
		}
		return Result::RES_OK;
	}
	NSASM::Result NSASM::calc(char* dst, char src, char type) {
		switch (type) {
		case '+': *dst += src; break;
		case '-': *dst -= src; break;
		case '*': *dst *= src; break;
		case '/': *dst /= src; break;
		case '&': *dst &= src; break;
		case '|': *dst |= src; break;
		case '~': *dst = ~(*dst); break;
		case '^': *dst ^= src; break;
		case '<': *dst = *dst << src; break;
		case '>': *dst = *dst >> src; break;
		default: return Result::RES_ERR;
		}
		return Result::RES_OK;
	}
	NSASM::Result NSASM::calc(float* dst, float src, char type) {
		switch (type) {
		case '+': *dst += src; break;
		case '-': *dst -= src; break;
		case '*': *dst *= src; break;
		case '/': *dst /= src; break;
		default: return Result::RES_ERR;
		}
		return Result::RES_OK;
	}
	NSASM::Result NSASM::calc(Register* dst, int src, char type) {
		switch (type) {
		case '+': dst->sp += src; break;
		case '-': dst->sp -= src; break;
		default: return Result::RES_ERR;
		}
		return Result::RES_OK;
	}
	NSASM::Result NSASM::calc(Register* dst, Register* src, char type) {
		switch (dst->type) {
		case RegType::REG_CHAR:
			switch (src->type) {
			case RegType::REG_CHAR:
				return calc(&dst->n.c, src->n.c, type);
			case RegType::REG_FLOAT:
				return calc(&dst->n.c, src->n.f, type);
			case RegType::REG_INT:
				return calc(&dst->n.c, src->n.i, type);
			case RegType::REG_STR:
				return Result::RES_ERR;
			}
			break;
		case RegType::REG_FLOAT:
			switch (src->type) {
			case RegType::REG_CHAR:
				return calc(&dst->n.f, src->n.c, type);
			case RegType::REG_FLOAT:
				return calc(&dst->n.f, src->n.f, type);
			case RegType::REG_INT:
				return calc(&dst->n.f, src->n.i, type);
			case RegType::REG_STR:
				return Result::RES_ERR;
			}
			break;
		case RegType::REG_INT:
			switch (src->type) {
			case RegType::REG_CHAR:
				return calc(&dst->n.i, src->n.c, type);
			case RegType::REG_FLOAT:
				return calc(&dst->n.i, src->n.f, type);
			case RegType::REG_INT:
				return calc(&dst->n.i, src->n.i, type);
			case RegType::REG_STR:
				return Result::RES_ERR;
			}
			break;
		case RegType::REG_STR:
			switch (src->type) {
			case RegType::REG_CHAR:
				return calc(dst, src->n.c, type);
			case RegType::REG_FLOAT:
				return Result::RES_ERR;
			case RegType::REG_INT:
				return calc(dst, src->n.i, type);
			case RegType::REG_STR:
				return Result::RES_ERR;
			}
			break;
		}
		return Result::RES_OK;
	}

	NSASM::Register* NSASM::eval(Register* reg) {
		if (reg == nullptr) return nullptr;
		if (reg->type != RegType::REG_CODE) return nullptr;
		map<string, string> code = Util::getSegments(reg->s);
		NSASM nsasm(*this, code);
		Register* result = new Register(*nsasm.run());
		result->gcFlag = true;
		return result;
	}

	NSASM::Result NSASM::execute(string var) {
		string op, dst, src; size_t pos;
		Register* dr = nullptr; Register* sr = nullptr;

		if ((pos = var.find(' ')) != var.npos)
			op = var.substr(0, pos);
		else op = var;
		transform(op.begin(), op.end(), op.begin(), ::tolower); // To lower case
		if (op.length() + 1 < var.length()) {
			if (
				op == "var" || op == "int" || op == "char" ||
				op == "float" || op == "str" || op == "code"
			) {
				// Variable define
				dst = var.substr(op.length() + 1);
				if ((pos = dst.find('=')) != var.npos)
					dst = dst.substr(0, pos);
				if (var.length() <= op.length() + 1 + dst.length()) return Result::RES_ERR;
				if (var[op.length() + 1 + dst.length()] == '=')
					src = var.substr(op.length() + 1 + dst.length() + 1);
				else src = "";
				dr = new Register(); dr->gcFlag = true;
				dr->readOnly = true; dr->type = RegType::REG_STR; dr->s = dst;
				sr = getRegister(src);
			} else {
				// Normal code
				if (
					verifyWord(var.substr(op.length() + 1), WordType::WD_STR) ||
					verifyWord(var.substr(op.length() + 1), WordType::WD_CHAR)
				) {
					dst = var.substr(op.length() + 1);
					src = "";
				} else {
					dst = var.substr(op.length() + 1);
					if ((pos = dst.find(',')) != var.npos)
						dst = dst.substr(0, pos);
					if (var.length() <= op.length() + 1 + dst.length())
						src = "";
					else if (var[op.length() + 1 + dst.length()] == ',')
						src = var.substr(op.length() + 1 + dst.length() + 1);
					else src = "";
				}
				dr = getRegister(dst);
				sr = getRegister(src);
			}
		}

		if (funcList.count(op) == 0)
			return verifyWord(op, WordType::WD_TAG) ? Result::RES_OK : Result::RES_ERR;

		prevDstReg = (dr != nullptr) ? *dr : prevDstReg;

		Result res = funcList[op](dr, sr);
		if (dr != nullptr) if (dr->gcFlag) _gc(dr); 
		if (sr != nullptr) if (sr->gcFlag) _gc(sr);
		return res;
	}
	NSASM::Register* NSASM::run() {
		if (code.empty()) return nullptr;
		Result result; string seg;

		progSeg = progCnt = 0;

		for (; progSeg < segs.size(); progSeg++) {
			seg = segs[progSeg];
			if (code[seg].empty()) continue;

			for (; progCnt < code[seg].size(); progCnt++) {
				if (tmpSeg >= 0 || tmpCnt >= 0) {
					progSeg = tmpSeg; progCnt = tmpCnt;
					tmpSeg = -1; tmpCnt = -1;
				}

				seg = segs[progSeg];
				if (code[seg].empty()) continue;

				if (code[seg][progCnt].length() == 0) continue;

				result = execute(code[seg][progCnt]);
				if (result == Result::RES_ERR) {
					Util::print("\nNSASM running error!\n");
					Util::print("At " + seg + ", line ");
					Util::print(progCnt + 1);
					Util::print(": " + code[seg][progCnt] + "\n\n");
					return nullptr;
				} else if (result == Result::RES_ETC) {
					prevDstReg.readOnly = false;
					return &prevDstReg;
				}
			}

			if (!backupReg.empty()) {
				progCnt = backupReg.top() + 1; backupReg.pop();
				progSeg = backupReg.top() - 1; backupReg.pop();
			} else progCnt = 0;
		}

		prevDstReg.readOnly = false;
		return &prevDstReg;
	}
	void NSASM::call(string segName) {
		Result result; string seg;

		int pos = 0;
		for (auto seg = code.begin(); seg != code.end(); seg++) {
			if (seg->first == segName) {
				tmpSeg = pos;
				tmpCnt = 0;
				break;
			} else pos++;
		}

		for (; progSeg < segs.size(); progSeg++) {
			seg = segs[progSeg];
			if (code[seg].empty()) continue;

			for (; progCnt < code[seg].size(); progCnt++) {
				if (tmpSeg >= 0 || tmpCnt >= 0) {
					progSeg = tmpSeg; progCnt = tmpCnt;
					tmpSeg = -1; tmpCnt = -1;
				}

				seg = segs[progSeg];
				if (code[seg].empty()) continue;

				if (code[seg][progCnt].length() == 0) continue;

				result = execute(code[seg][progCnt]);
				if (result == Result::RES_ERR) {
					Util::print("\nNSASM running error!\n");
					Util::print("At " + seg + ", line ");
					Util::print(progCnt + 1);
					Util::print(": " + code[seg][progCnt] + "\n\n");
					return;
				} else if (result == Result::RES_ETC) {
					return;
				}

			}

			if (!backupReg.empty()) {
				progCnt = backupReg.top() + 1; backupReg.pop();
				progSeg = backupReg.top() - 1; backupReg.pop();
			} else progCnt = 0;
		}
	}

}
