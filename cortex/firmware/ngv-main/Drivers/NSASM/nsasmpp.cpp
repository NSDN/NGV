#include "NSASM.h"
#include "Util.h"

#include <fstream>
using namespace std;

#include <string.h>
#include <time.h>

#include "../NGV/Include/nsio.h"

extern "C" int nsasmpp(int argc, char* argv[]);

int nsasmpp(int argc, char* argv[]) {
	NSASM::Util::I().Output = [](string v) -> void {
		for (int ptr = 0; ptr < v.length(); ptr += IOBUF)
			print("%s", v.substr(ptr, IOBUF).c_str());
	};
	NSASM::Util::I().Input = [](void) -> string {
		char buffer[IOBUF];
		scan(buffer);
		string s = buffer;
		return s;
	};
	NSASM::Util::I().FileInput = [](string path) -> string {
		char* buffer = read((char*)path.c_str());
		string s = buffer;
		return s;
	};
	NSASM::Util::I().BinaryInput = [](string path) ->vector<unsigned char> { 
		vector<unsigned char> res;
		uint32_t len = 0; FILTYPE file;
		if (filopen(&file, (char*)path.c_str(), FIL_READ | FIL_BIN) != FIL_OK)
			return res;
		len = filsiz(&file); res.resize(len);
		uint32_t ptr = 0;
		filread(&file, (uint8_t*)res.data(), len, &ptr);
		filclose(&file);
		return res;
	};
	NSASM::Util::I().BinaryOutput = [](string path, vector<unsigned char> data) -> void {
		FILTYPE file;
		if (filopen(&file, (char*)path.c_str(), FIL_WRITE | FIL_BIN) != FIL_OK)
			return;
		filwrite(&file, (uint8_t*)data.data(), data.size());
		filclose(&file);
	};

	NSASM::Util::print("NyaSama Assembly Script Module\n");
	NSASM::Util::print("Version: ");
	NSASM::Util::print(NSASM::NSASM::ver());
	NSASM::Util::print("\n\n");

	if (argc <= 1) {
		NSASM::Util::print("Usage: nsasm [c/r/i] [FILE]\n\n");
		NSASM::Util::interactive();
	} else {
		if (argc == 4) {
			if (strcmp(argv[1], "c") == 0) {
				string res = NSASM::Util::compile(argv[2], argv[3]);
				if (res != nulstr)
					NSASM::Util::print("Compilation OK.\n\n");
				return 0;
			}
		} else if (argc == 3) {
			if (strcmp(argv[1], "r") == 0) {
				clock_t begin, end; float t;
				string path = argv[2];
				begin = clock();
				NSASM::Util::run(path);
				end = clock();
				t = (float)(end - begin) / (float)CLOCKS_PER_SEC * 1000.0F;
				NSASM::Util::print("This script took ");
				NSASM::Util::print(t);
				NSASM::Util::print("ms.\n\n");
				return 0;
			} else if (strcmp(argv[1], "c") == 0) {
				string res = NSASM::Util::compile(argv[2], nulstr);
				NSASM::Util::print("\n" + res + "\n");
				return 0;
			} else {
				clock_t begin, end; float t;
				string path = argv[1]; string seg = argv[2];
				string code = NSASM::Util::read(path);
				auto segs = NSASM::Util::getSegments(code);
				NSASM::NSASM nsasm(64, 32, 32, segs);
				begin = clock();
				nsasm.call(seg);
				end = clock();
				t = (float)(end - begin) / (float)CLOCKS_PER_SEC * 1000.0F;
				NSASM::Util::print("This script took ");
				NSASM::Util::print(t);
				NSASM::Util::print("ms.\n\n");
				return 0;
			}
		}
		if (strcmp(argv[1], "i") == 0) {
			NSASM::Util::interactive();
			return 0;
		}
		string path = argv[1];
		NSASM::Util::run(path);
	}

	return 0;
}
