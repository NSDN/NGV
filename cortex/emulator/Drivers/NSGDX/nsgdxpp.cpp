#include "../NSASM/NSASM.h"
#include "../NSASM/Util.h"

#include "NSGDX.h"

#include <fstream>
using namespace std;

#include <string.h>
#include <time.h>

#include "../NGV/Include/nsio.h"
#include "../NGV/Include/nshel.h"

void run(string path) {
    string str = NSGDX::Util::read(path);
    if (str == nulstr) return;

    int heap = 64, stack = 32, regs = 16;

    string conf = NSGDX::Util::getSegment(str, ".<conf>");
    if (conf == nulstr) {
        NSGDX::Util::print("Conf load error.\n");
        NSGDX::Util::print("At file: " + path + "\n\n");
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
            NSGDX::Util::print("Conf load error.\n");
            NSGDX::Util::print("At file: " + path + "\n\n");
            return;
        }
    }

    map<string, string> code = NSGDX::Util::getSegments(str);
    NSGDX::NSGDX core(heap, stack, regs, code);
    core.run();
    _nshel_fun_clear(0, (char**) 0);
}

int nsgdx(int argc, char* argv[]) {
	NSGDX::Util::I().Output = [](string v) -> void {
		print("%s", v.c_str());
	};
	NSGDX::Util::I().Input = [](void) -> string {
		char buffer[IOBUF];
		scan(buffer);
		string s = buffer;
		return s;
	};
	NSGDX::Util::I().FileInput = [](string path) -> string {
		char* buffer = read((char*)path.c_str());
		string s = buffer;
		return s;
	};

	if (argc <= 1) {
		NSGDX::Util::print("Usage: nsgdx [FILE]\n\n");
	} else {
		string path = argv[1];
		run(path);
	}

	return 0;
}
