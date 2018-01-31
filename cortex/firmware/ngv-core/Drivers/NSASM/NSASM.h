#ifndef __NSASM_H_
#define __NSASM_H_


#include <functional>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>

namespace NSASM {

	using namespace std;

	class NSASM {

	public:
		static string ver() { return "0.43"; }

		enum Result {
			RES_OK, RES_ERR, RES_ETC
		};
		
		enum RegType {
			REG_CHAR, REG_STR, REG_INT, REG_FLOAT, REG_CODE
		};

		class Register {
		public:
			Register() {
				this->type = RegType::REG_INT;
				this->readOnly = false;
				this->gcFlag = false;
				this->n.c = 0;
				this->n.i = 0;
				this->n.f = 0;
				this->s = "";
			}
			Register(const Register& reg) {
				gcFlag = false;
				type = reg.type; sp = reg.sp; readOnly = reg.readOnly;
				switch (type) {
				case RegType::REG_INT: n.i = reg.n.i; break;
				case RegType::REG_CHAR: n.c = reg.n.c; break;
				case RegType::REG_FLOAT: n.f = reg.n.f; break;
				case RegType::REG_STR: s = reg.s; break;
				case RegType::REG_CODE: s = reg.s; break;
				}
			}
			~Register() {
				if (this->type == RegType::REG_STR || this->type == RegType::REG_CODE)
					this->s.clear();
			}

		public:
			Register& operator=(const Register& reg) {
				gcFlag = false;
				type = reg.type; sp = reg.sp; readOnly = reg.readOnly;
				switch (type) {
				case RegType::REG_INT: n.i = reg.n.i; break;
				case RegType::REG_CHAR: n.c = reg.n.c; break;
				case RegType::REG_FLOAT: n.f = reg.n.f; break;
				case RegType::REG_STR: s = reg.s; break;
				case RegType::REG_CODE: s = reg.s; break;
				}
				return *this;
			}
			Register& operator>>(string& s) {
				stringstream parser; parser.clear();
				switch (this->type) {
				case RegType::REG_INT: parser << this->n.i; s = parser.str(); break;
				case RegType::REG_CHAR: parser << this->n.c; s = parser.str(); break;
				case RegType::REG_FLOAT: parser << this->n.f; s = parser.str(); break;
				case RegType::REG_STR: s = this->s.substr(this->sp); break;
				case RegType::REG_CODE: s = this->s; break;
				}
				return *this;
			}

		public:
			RegType type;
			union Num {
			public:
				Num() { i = 0; }
				~Num() {}
			public:
				int i; char c; float f;
			} n;
			string s; int sp = 0;
			bool readOnly; bool gcFlag;
		};

		typedef function<Result(Register*, Register*)> Operator;
		#define $OP_ [&](Register* dst, Register* src) -> Result

		Result execute(string var);
		Register* run();
		void call(string segName);

		NSASM(int heapSize, int stackSize, int regCnt, map<string, string>& code);
		~NSASM();

	protected:
		vector<Register> regGroup;
		map<string, Operator> funcList;

		Register* eval(Register* reg);
		void loadFuncList();

	private:
		enum WordType {
			WD_REG, WD_CHAR, WD_STR, WD_INT,
			WD_FLOAT, WD_VAR, WD_TAG, WD_SEG,
			WD_CODE
		};

		map<string, Register> heapManager;
		stack<Register> stackManager;
		int heapSize, stackSize;
		Register stateReg;
		Register prevDstReg;

		stack<int> backupReg;
		int progSeg, tmpSeg;
		int progCnt, tmpCnt;

		vector<string> segs;
		map<string, vector<string>> code;

		bool verifyBound(string var, char left, char right);
		bool verifyWord(string var, WordType type);
		Register* getRegister(string var);
		vector<string> convToArray(string var);
		Result appendCode(map<string, string>& code);
		void copyRegGroup(NSASM& super);

		Result calc(int* dst, int src, char type);
		Result calc(char* dst, char src, char type);
		Result calc(float* dst, float src, char type);
		Result calc(Register* dst, int src, char type);
		Result calc(Register* dst, Register* src, char type);

		NSASM(NSASM& super, map<string, string>& code);

	};

}


#endif
