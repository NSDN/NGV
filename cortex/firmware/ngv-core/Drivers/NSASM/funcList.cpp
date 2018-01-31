#include "NSASM.h"

#include "Util.h"

namespace NSASM {

	void NSASM::loadFuncList() {

		funcList["rem"] = $OP_{
			return Result::RES_OK;
		};

		funcList["var"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (!verifyWord(dst->s, WordType::WD_VAR)) return Result::RES_ERR;
			if (heapManager.count(dst->s) != 0) return Result::RES_ERR;
			heapManager[dst->s] = *src;
			if (heapManager[dst->s].type != RegType::REG_STR)
				heapManager[dst->s].readOnly = false;
			return Result::RES_OK;
		};

		funcList["int"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (!verifyWord(dst->s, WordType::WD_VAR)) return Result::RES_ERR;
			if (heapManager.count(dst->s) != 0) return Result::RES_ERR;
			if (src->type != RegType::REG_INT) return Result::RES_ERR;
			heapManager[dst->s] = *src;
			heapManager[dst->s].readOnly = false;
			return Result::RES_OK;
		};

		funcList["char"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (!verifyWord(dst->s, WordType::WD_VAR)) return Result::RES_ERR;
			if (heapManager.count(dst->s) != 0) return Result::RES_ERR;
			if (src->type != RegType::REG_CHAR) return Result::RES_ERR;
			heapManager[dst->s] = *src;
			heapManager[dst->s].readOnly = false;
			return Result::RES_OK;
		};

		funcList["float"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (!verifyWord(dst->s, WordType::WD_VAR)) return Result::RES_ERR;
			if (heapManager.count(dst->s) != 0) return Result::RES_ERR;
			if (src->type != RegType::REG_FLOAT) return Result::RES_ERR;
			heapManager[dst->s] = *src;
			heapManager[dst->s].readOnly = false;
			return Result::RES_OK;
		};

		funcList["str"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (!verifyWord(dst->s, WordType::WD_VAR)) return Result::RES_ERR;
			if (heapManager.count(dst->s) != 0) return Result::RES_ERR;
			if (src->type != RegType::REG_STR) return Result::RES_ERR;
			heapManager[dst->s] = *src;
			heapManager[dst->s].readOnly = true;
			return Result::RES_OK;
		};

		funcList["code"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (!verifyWord(dst->s, WordType::WD_VAR)) return Result::RES_ERR;
			if (heapManager.count(dst->s) != 0) return Result::RES_ERR;
			if (src->type != RegType::REG_CODE) return Result::RES_ERR;
			heapManager[dst->s] = *src;
			heapManager[dst->s].readOnly = false;
			return Result::RES_OK;
		};

		funcList["mov"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->readOnly) return Result::RES_ERR;
			if (dst->type == RegType::REG_CHAR && src->type == RegType::REG_STR) {
				dst->n.c = src->s[src->sp];
			} else if (dst->type == RegType::REG_STR && src->type == RegType::REG_CHAR) {
				dst->s[dst->sp] = src->n.c;
			} else {
				*dst = *src;
				if (dst->readOnly) dst->readOnly = false;
			}
			return Result::RES_OK;
		};

		funcList["push"] = $OP_{
			if (src != nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (stackManager.size() >= stackSize) return Result::RES_ERR;
			stackManager.push(*dst);
			return Result::RES_OK;
		};

		funcList["pop"] = $OP_{
			if (src != nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->readOnly) return Result::RES_ERR;
			*dst = stackManager.top(); stackManager.pop();
			return Result::RES_OK;
		};

		funcList["in"] = $OP_{
			string buf; Register* reg;
			if (src == nullptr) {
				if (dst == nullptr) return Result::RES_ERR;
				if (dst->readOnly && dst->type != RegType::REG_STR) return Result::RES_ERR;
				buf = Util::scan();
				switch (dst->type) {
				case RegType::REG_INT:
					reg = getRegister(buf);
					if (reg == nullptr) return Result::RES_OK;
					if (reg->type != RegType::REG_INT) return Result::RES_OK;
					dst->n.i = reg->n.i;
					delete reg;
					break;
				case RegType::REG_CHAR:
					if (buf.length() < 1) return Result::RES_OK;
					dst->n.c = buf[0];
					break;
				case RegType::REG_FLOAT:
					reg = getRegister(buf);
					if (reg == nullptr) return Result::RES_OK;
					if (reg->type != RegType::REG_FLOAT) return Result::RES_OK;
					dst->n.f = reg->n.f;
					delete reg;
					break;
				case RegType::REG_STR:
					if (buf.length() < 1) return Result::RES_OK;
					dst->s = buf;
					dst->sp = 0;
					break;
				}
			} else {
				if (dst == nullptr) return Result::RES_ERR;
				if (src->type != RegType::REG_INT) return Result::RES_ERR;
				switch (src->n.i) {
				case 0x00:
					if (dst->readOnly && dst->type != RegType::REG_STR) return Result::RES_ERR;
					buf = Util::scan();
					switch (dst->type) {
					case RegType::REG_INT:
						reg = getRegister(buf);
						if (reg == nullptr) return Result::RES_OK;
						if (reg->type != RegType::REG_INT) return Result::RES_OK;
						dst->n.i = reg->n.i;
						delete reg;
						break;
					case RegType::REG_CHAR:
						if (buf.length() < 1) return Result::RES_OK;
						dst->n.c = buf[0];
						break;
					case RegType::REG_FLOAT:
						reg = getRegister(buf);
						if (reg == nullptr) return Result::RES_OK;
						if (reg->type != RegType::REG_FLOAT) return Result::RES_OK;
						dst->n.f = reg->n.f;
						delete reg;
						break;
					case RegType::REG_STR:
						if (buf.length() < 1) return Result::RES_OK;
						dst->s = buf;
						dst->sp = 0;
						break;
					}
					break;
				case 0xFF:
					Util::print("[DEBUG] <<< ");
					if (dst->readOnly && dst->type != RegType::REG_STR) return Result::RES_ERR;
					buf = Util::scan();
					switch (dst->type) {
					case RegType::REG_INT:
						reg = getRegister(buf);
						if (reg == nullptr) return Result::RES_OK;
						if (reg->type != RegType::REG_INT) return Result::RES_OK;
						dst->n.i = reg->n.i;
						delete reg;
						break;
					case RegType::REG_CHAR:
						if (buf.length() < 1) return Result::RES_OK;
						dst->n.c = buf[0];
						break;
					case RegType::REG_FLOAT:
						reg = getRegister(buf);
						if (reg == nullptr) return Result::RES_OK;
						if (reg->type != RegType::REG_FLOAT) return Result::RES_OK;
						dst->n.f = reg->n.f;
						delete reg;
						break;
					case RegType::REG_STR:
						if (buf.length() < 1) return Result::RES_OK;
						dst->s = buf;
						dst->sp = 0;
						break;
					}
					break;
				default:
					return Result::RES_ERR;
				}
			}
			return Result::RES_OK;
		};

		funcList["out"] = $OP_{
			if (dst == nullptr) return Result::RES_ERR;
			string s = "";
			if (src == nullptr) {
				if (dst->type == RegType::REG_CODE) {
					Register* reg = eval(dst);
					if (reg == nullptr) return Result::RES_ERR;
					*reg >> s; if (reg->gcFlag) delete reg;
				} else *dst >> s;
				Util::print(s);
			} else {
				if (dst->type != RegType::REG_INT) return Result::RES_ERR;
				switch (dst->n.i) {
				case 0x00:
					if (src->type == RegType::REG_CODE) {
						Register* reg = eval(src);
						if (reg == nullptr) return Result::RES_ERR;
						*reg >> s; if (reg->gcFlag) delete reg;
					} else *src >> s;
					Util::print(s);
					break;
				case 0xFF:
					Util::print("[DEBUG] >>> ");
					if (src->type == RegType::REG_CODE) {
						Register* reg = eval(src);
						if (reg == nullptr) return Result::RES_ERR;
						*reg >> s; if (reg->gcFlag) delete reg;
					} else *src >> s;
					Util::print(s);
					Util::print('\n');
					break;
				default:
					return Result::RES_ERR;
				}
			}
			return Result::RES_OK;
		};

		funcList["prt"] = $OP_{
			if (dst == nullptr) return Result::RES_ERR;
			stringstream ss; string s;
			if (src != nullptr) {
				if (dst->type == RegType::REG_STR) {
					if (dst->readOnly) return Result::RES_ERR;
					if (src->type == RegType::REG_CHAR && src->n.c == '\b') {
						if (dst->s.find('\n') != dst->s.npos) {
							string buf = "", res = "";
							ss << dst->s;
							while (!ss.eof()) {
								getline(ss, buf);
								if (ss.eof()) break;
								res += buf;
							}
						}
					} else if (src->type == RegType::REG_CODE) {
						Register* reg = eval(src);
						if (reg == nullptr) return Result::RES_ERR;
						*reg >> s; if (reg->gcFlag) delete reg;
						dst->s += ("\n" + s);
					} else if (src->type == RegType::REG_STR) {
						dst->s += ("\n" + src->s);
					} else return Result::RES_ERR;
				} else if (dst->type == RegType::REG_CODE) {
					if (dst->readOnly) return Result::RES_ERR;
					if (src->type == RegType::REG_CHAR && src->n.c == '\b') {
						if (dst->s.find('\n') != dst->s.npos) {
							string buf = "", res = "";
							ss << dst->s;
							while (!ss.eof()) {
								getline(ss, buf);
								if (ss.eof()) break;
								res += buf;
							}
						}
					} else if (src->type == RegType::REG_CODE) {
						dst->s += ("\n" + src->s);
					} else if (src->type == RegType::REG_STR) {
						dst->s += ("\n" + src->s);
					} else return Result::RES_ERR;
				} else return Result::RES_ERR;
			} else {
				if (dst == nullptr) return Result::RES_ERR;
				if (dst->type == RegType::REG_CODE) {
					Register* reg = eval(src);
					if (reg == nullptr) return Result::RES_ERR;
					*reg >> s; if (reg->gcFlag) delete reg;
				} else *dst >> s;
				Util::print(s + '\n');
			}
			return Result::RES_OK;
		};

		funcList["add"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->readOnly) return Result::RES_ERR;
			if (src->type == RegType::REG_CODE) {
				Register* reg = eval(src);
				Result res =  calc(dst, reg, '+');
				if (reg != nullptr) if (reg->gcFlag) delete reg;
				return res;
			} else return calc(dst, src, '+');
		};

		funcList["inc"] = $OP_{
			if (src != nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->readOnly) return Result::RES_ERR;
			Register reg;
			reg.type = RegType::REG_INT;
			reg.n.i = 0x01;
			reg.readOnly = true;
			reg.gcFlag = true;
			return calc(dst, &reg, '+');
		};

		funcList["sub"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->readOnly) return Result::RES_ERR;
			if (src->type == RegType::REG_CODE) {
				Register* reg = eval(src);
				Result res = calc(dst, reg, '-');
				if (reg != nullptr) if (reg->gcFlag) delete reg;
				return res;
			} else return calc(dst, src, '-');
		};

		funcList["dec"] = $OP_{
			if (src != nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->readOnly) return Result::RES_ERR;
			Register reg;
			reg.type = RegType::REG_INT;
			reg.n.i = 0x01;
			reg.readOnly = true;
			reg.gcFlag = true;
			return calc(dst, &reg, '-');
		};

		funcList["mul"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->readOnly) return Result::RES_ERR;
			if (src->type == RegType::REG_CODE) {
				Register* reg = eval(src);
				Result res = calc(dst, reg, '*');
				if (reg != nullptr) if (reg->gcFlag) delete reg;
				return res;
			} else return calc(dst, src, '*');
		};

		funcList["div"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->readOnly) return Result::RES_ERR;
			if (src->type == RegType::REG_CODE) {
				Register* reg = eval(src);
				Result res = calc(dst, reg, '/');
				if (reg != nullptr) if (reg->gcFlag) delete reg;
				return res;
			} else return calc(dst, src, '/');
		};

		funcList["and"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->readOnly) return Result::RES_ERR;
			if (src->type == RegType::REG_CODE) {
				Register* reg = eval(src);
				Result res = calc(dst, reg, '&');
				if (reg != nullptr) if (reg->gcFlag) delete reg;
				return res;
			} else return calc(dst, src, '&');
		};

		funcList["or"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->readOnly) return Result::RES_ERR;
			if (src->type == RegType::REG_CODE) {
				Register* reg = eval(src);
				Result res = calc(dst, reg, '|');
				if (reg != nullptr) if (reg->gcFlag) delete reg;
				return res;
			} else return calc(dst, src, '|');
		};

		funcList["xor"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->readOnly) return Result::RES_ERR;
			if (src->type == RegType::REG_CODE) {
				Register* reg = eval(src);
				Result res = calc(dst, reg, '^');
				if (reg != nullptr) if (reg->gcFlag) delete reg;
				return res;
			} else return calc(dst, src, '^');
		};

		funcList["not"] = $OP_{
			if (src != nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->readOnly) return Result::RES_ERR;
			return calc(dst, nullptr, '~');
		};

		funcList["shl"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->readOnly) return Result::RES_ERR;
			if (src->type == RegType::REG_CODE) {
				Register* reg = eval(src);
				Result res = calc(dst, reg, '<');
				if (reg != nullptr) if (reg->gcFlag) delete reg;
				return res;
			} else return calc(dst, src, '<');
		};

		funcList["shr"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->readOnly) return Result::RES_ERR;
			if (src->type == RegType::REG_CODE) {
				Register* reg = eval(src);
				Result res = calc(dst, reg, '>');
				if (reg != nullptr) if (reg->gcFlag) delete reg;
				return res;
			} else return calc(dst, src, '>');
		};

		funcList["cmp"] = $OP_{
			if (src == nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (funcList["mov"](&stateReg, dst) == Result::RES_ERR)
				return Result::RES_ERR;
			if (src->type == RegType::REG_CODE) {
				Register* reg = eval(src);
				if (funcList["sub"](&stateReg, reg) == Result::RES_ERR) {
					if (reg != nullptr) if (reg->gcFlag) delete reg;
					return Result::RES_ERR;
				}
				if (reg != nullptr) if (reg->gcFlag) delete reg;
			} else {
				if (funcList["sub"](&stateReg, src) == Result::RES_ERR)
					return Result::RES_ERR;
			}
			return Result::RES_OK;
		};

		funcList["test"] = $OP_{
			if (src != nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->type == RegType::REG_CODE) {
				Register* reg = eval(dst);
				if (funcList["mov"](&stateReg, reg) == Result::RES_ERR) {
					if (reg != nullptr) if (reg->gcFlag) delete reg;
					return Result::RES_ERR;
				}
				if (reg != nullptr) if (reg->gcFlag) delete reg;
			} else {
				if (funcList["mov"](&stateReg, dst) == Result::RES_ERR)
					return Result::RES_ERR;
			}
			Register zero;
			zero.type = RegType::REG_INT; zero.readOnly = false;
			zero.n.i = 0; zero.gcFlag = true;
			if (funcList["sub"](&stateReg, &zero) == Result::RES_ERR)
				return Result::RES_ERR;
			return Result::RES_OK;
		};
		
		funcList["jmp"] = $OP_{
			if (src != nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->type != RegType::REG_STR) return Result::RES_ERR;
			if (!verifyWord(dst->s, WordType::WD_TAG)) return Result::RES_ERR;
			for (int seg = 0; seg < segs.size(); seg++) {
				if (code[segs[seg]].empty()) continue;
				for (int line = 0; line < code[segs[seg]].size(); line++) {
					if (code[segs[seg]][line] == dst->s) {
						tmpSeg = seg; tmpCnt = line;
						return Result::RES_OK;
					}
				}
			}
			return Result::RES_ERR;
		};

		funcList["jz"] = $OP_{
			stringstream ss; ss.clear();
			string s; stateReg >> s;
			ss << s; float f = 0.0F; ss >> f;
			if (f == 0.0F) {
				return funcList["jmp"](dst, src);
			}
			return Result::RES_OK;
		};

		funcList["jnz"] = $OP_{
			stringstream ss; ss.clear();
			string s; stateReg >> s;
			ss << s; float f = 0.0F; ss >> f;
			if (f != 0.0F) {
				return funcList["jmp"](dst, src);
			}
			return Result::RES_OK;
		};

		funcList["jg"] = $OP_{
			stringstream ss; ss.clear();
			string s; stateReg >> s;
			ss << s; float f = 0.0F; ss >> f;
			if (f > 0.0F) {
				return funcList["jmp"](dst, src);
			}
			return Result::RES_OK;
		};

		funcList["jl"] = $OP_{
			stringstream ss; ss.clear();
			string s; stateReg >> s;
			ss << s; float f = 0.0F; ss >> f;
			if (f < 0.0F) {
				return funcList["jmp"](dst, src);
			}
			return Result::RES_OK;
		};

		funcList["end"] = $OP_{
			if (dst == nullptr && src == nullptr)
				return Result::RES_ETC;
			return Result::RES_ERR;
		};

		funcList["ret"] = $OP_{
			if (src == nullptr) {
				if (dst != nullptr) prevDstReg = *dst;
				else prevDstReg = regGroup[0];
				return Result::RES_ETC;
			}
			return Result::RES_ERR;
		};

		funcList["nop"] = $OP_{
			if (dst == nullptr && src == nullptr)
				return Result::RES_OK;
			return Result::RES_ERR;
		};

		funcList["rst"] = $OP_{
			if (dst == nullptr && src == nullptr) {
				tmpSeg = 0; tmpCnt = 0;
				return Result::RES_OK;
			}
			return Result::RES_ERR;
		};

		funcList["run"] = $OP_{
			if (src != nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->type != RegType::REG_STR) return Result::RES_ERR;
			if (!verifyWord(dst->s, WordType::WD_SEG)) return Result::RES_ERR;
			for (int seg = 0; seg < segs.size(); seg++) {
				if (segs[seg] == dst->s) {
					tmpSeg = seg; tmpCnt = 0;
					return Result::RES_OK;
				}
			}
			return Result::RES_ERR;
		};

		funcList["call"] = $OP_{
			if (src != nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->type != RegType::REG_STR) return Result::RES_ERR;
			if (!verifyWord(dst->s, WordType::WD_SEG)) return Result::RES_ERR;
			for (int seg = 0; seg < segs.size(); seg++) {
				if (segs[seg] == dst->s) {
					tmpSeg = seg; tmpCnt = 0;
					backupReg.push(progSeg);
					backupReg.push(progCnt);
					return Result::RES_OK;
				}
			}
			return Result::RES_OK;
		};

		funcList["ld"] = $OP_{
			if (src != nullptr) return Result::RES_ERR;
			if (dst == nullptr) return Result::RES_ERR;
			if (dst->type != RegType::REG_STR && dst->type != RegType::REG_CODE)
				return Result::RES_ERR;

			string path;
			if (dst->type == RegType::REG_CODE) {
				Register* reg = eval(dst);
				if (reg == nullptr) return Result::RES_ERR;
				if (reg->type != RegType::REG_STR) {
					if (reg->gcFlag) delete reg;
					return Result::RES_ERR;
				}
				path = reg->s;
				if (reg->gcFlag) delete reg;
			} else path = dst->s;

			string code = Util::read(path);
			if (code == nulstr) return Result::RES_ERR;
			auto segs = Util::getSegments(code);
			if (appendCode(segs) == Result::RES_ERR) {
				Util::print("At file: " + path + "\n");
				return Result::RES_ERR;
			}
			return Result::RES_OK;
		};

		funcList["eval"] = $OP_{
			if (dst == nullptr) return Result::RES_ERR;

			Register* reg;
			if (src == nullptr) reg = eval(dst);
			else {
				reg = eval(src);
				*dst = *reg;
			}
			if (reg != nullptr) if (reg->gcFlag) delete reg;

			return Result::RES_OK;
		};
		
	}

}
