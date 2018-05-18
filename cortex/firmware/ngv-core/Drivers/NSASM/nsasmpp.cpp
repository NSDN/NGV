#include "NSASM.h"
#include "Util.h"

#include <fstream>
using namespace std;

#include <string.h>
#include <time.h>

#include "nsio.h"

extern "C" int nsasmpp(int argc, char* argv[]);

int nsasmpp(int argc, char* argv[]) {
	NSASM::Util::I().Output = [](string v) -> void {
		print("%s", v.c_str());
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

	NSASM::Util::print("NyaSama Assembly Script Module\n");
	NSASM::Util::print("Version: ");
	NSASM::Util::print(NSASM::NSASM::ver());
	NSASM::Util::print("\n\n");

	if (argc <= 1) {
		NSASM::Util::print("Usage: nsasm [c/r] [FILE]\n\n");
		NSASM::Util::interactive();
	} else {
		if (argc == 3) {
			if (strcmp(argv[1], "r") == 0) {
				clock_t begin, end; float t;
				string path = argv[2];
				begin = clock();
				NSASM::Util::run(path);
				end = clock();
				t = (float)(end - begin) / (float)CLOCKS_PER_SEC * 1000.0F;
				NSASM::Util::print("This script took ");
				NSASM::Util::print(t);
				NSASM::Util::print("ms.\n");
				return 0;
			}
		}
		if (strcmp(argv[1], "c") == 0) {
			NSASM::Util::interactive();
			return 0;
		}
		string path = argv[1];
		NSASM::Util::run(path);
	}

	return 0;
}
