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

	class Util {

	public:
		static Util& I() {
			static Util inst;
			return inst;
		}

		typedef function<void(string)> Printer;
		typedef function<string(void)> Scanner;
		typedef function<string(string)> FileReader;

		Printer Output;
		Scanner Input;
		FileReader FileInput;

		Util() {
			Output = [](string str) -> void { cout << str; };
			Input = [](void) -> string { string s = ""; getline(cin, s); return s; };
			FileInput = [](string path) -> string { return ""; };
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

	public:
		static void print(int value);
		static void print(char value);
		static void print(float value);
		static void print(string value);
		static string scan();
		static void cleanSymbol(string& var, string symbol, string trash);
		static void cleanSymbol(string& var, string symbol, string trashA, string trashB);
		static string formatLine(string var);
		static string formatCode(string var);
		static void repairBrackets(string& var, string left, string right);
		static void encodeLambda(string& var);
		static void decodeLambda(string& var);
		static void formatString(string& var);
		static string formatLambda(string var);
		static map<string, string> getSegments(string var);
		static string getSegment(string var, string head);
		static string read(string path);
		static void run(string path);
		static void execute(string str);
		static void interactive();

	};

}


#endif
