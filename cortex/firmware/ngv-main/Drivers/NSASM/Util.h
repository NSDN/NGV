#ifndef __UTIL_H_
#define __UTIL_H_


#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>

#define nulstr ""

namespace NSASM {

	using namespace std;

	class DefBlock {

	public:
		string name;
		vector<string> args;
		string block;

	public:
		DefBlock() {
			name = "";
			args.clear();
			block = "";
		}

		DefBlock(const DefBlock& defBlock) {
			name = defBlock.name;
			args = defBlock.args;
			block = defBlock.block;
		}

		static DefBlock& nulblk() {
			static DefBlock blk;
			return blk;
		}

		friend bool operator==(const DefBlock& left, const DefBlock& right) {
			return left.name == right.name && left.args == right.args && left.block == right.block;
		}

		static DefBlock getBlock(string head, string body);
	};

	class DefCall {

	public:
		string name;
		vector<string> args;

	public:
		DefCall() {
			name = "";
			args.clear();
		}

		DefCall(const DefCall& defCall) {
			name = defCall.name;
			args = defCall.args;
		}

		static DefCall getCall(string str);
	};

	class Util {

	public:
		static Util& I() {
			static Util inst;
			return inst;
		}

		typedef function<void(string)> Printer;
		typedef function<string(void)> Scanner;
		typedef function<string(string)> FileReader;
		typedef function<vector<unsigned char>(string)> BinReader;
		typedef function<void(string, vector<unsigned char>)> BinWriter;

		Printer Output;
		Scanner Input;
		FileReader FileInput;
		BinReader BinaryInput;
		BinWriter BinaryOutput;

		Util() {
			Output = [](string str) -> void { cout << str; };
			Input = [](void) -> string { string s = ""; getline(cin, s); return s; };
			FileInput = [](string path) -> string { return ""; };
			BinaryInput = [](string path) ->vector<unsigned char> { vector<unsigned char> res; return res; };
			BinaryOutput = [](string path, vector<unsigned char> data) -> void {};
		}

	public:
		static inline void replace(string& var, string src, string dst) {
			size_t pos = 0; stack<size_t> s;
			while ((pos = var.find(src, pos)) != var.npos) {
				s.push(pos); pos += 1;
			}	
			while (!s.empty()) {
				var = var.replace(s.top(), src.length(), dst); s.pop();
			}
		}

		static inline void cycleReplace(string& var, string src, string dst) {
			if (dst.find(src) != dst.npos) return;
			size_t pos;
			while ((pos = var.find(src)) != var.npos) {
				var = var.replace(pos, src.length(), dst);
			}
		}

		template<typename T>
		static inline string strHash(T var) {
			hash<T> h;
			stringstream s;
			s << hex << h(var);
			return s.str();
		}

		template<typename T>
		static inline size_t intHash(T var) {
			hash<T> h;
			return h(var);
		}

	public:
		static void print(int value);
		static void print(char value);
		static void print(float value);
		static void print(string value);
		static string scan();
		static void cleanSymbol(string& var, string symbol, string trash);
		static void cleanSymbol(string& var, string symbol, string trashA, string trashB);
		static void cleanSymbolLeft(string& var, string symbol, string trashA, string trashB);
		static void cleanSymbolRight(string& var, string symbol, string trashA, string trashB);
		static string formatLine(string var);
		static string formatCode(string var);
		static void repairBrackets(string& var, string left, string right);
		static void encodeLambda(string& var);
		static void decodeLambda(string& var);
		static void formatString(string& var);
		static string formatLambda(string var);
		static string preProcessCode(string var);
		static map<string, string> getSegments(string var);
		static string getSegment(string var, string head);
		static vector<string> parseArgs(string str, char split);
		static vector<DefBlock> getDefBlocks(string var);
		static string doPreProcess(vector<DefBlock> blocks, string var);
		static string read(string path);
		static void run(string path);
		static void execute(string str);
		static void interactive();
		static string compile(string inPath, string outPath);
		static void binary(string path);
		
	private:
		static void putToList(vector<unsigned char>& list, unsigned short value);
		static void putToList(vector<unsigned char>& list, string value);
		static unsigned short getUint16(vector<unsigned char> data, int offset);
		static string getStr2(vector<unsigned char> data, int offset);

	};

}


#endif
