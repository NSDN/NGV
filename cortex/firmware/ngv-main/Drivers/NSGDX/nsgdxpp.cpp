#include "../NSASM/NSASM.h"
#include "../NSASM/Util.h"

#include "NSGDX.h"

#include <fstream>
using namespace std;

#include <malloc.h>
#include <string.h>
#include <time.h>

#include "../NGV/Include/nsio.h"
#include "../NGV/Include/nshel.h"

extern "C" int nsgdx(int argc, char* argv[]);

unsigned short getUint16(vector<unsigned char> data, int offset) {
    unsigned short res = 0;
    if (data.size() >= offset + 1)
        res = (data[offset] | (data[offset + 1] << 8));
    return res;
}

string getStr2(vector<unsigned char> data, int offset) {
    string res = "";
    if (data.size() >= offset + 1) {
        res += (char)data[offset];
        res += (char)data[offset + 1];
    }
    return res;
}

void binary(string path) {
    vector<unsigned char> data = NSGDX::Util::I().BinaryInput(path);
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

    NSGDX::NSGDX core(heap, stack, regs, code);
    core.run();
    _nshel_fun_clear(0, (char**) 0);
}

void run(string path) {
    if (path.find(".nsb") == path.length() - 4) {
        binary(path);
        return;
    }

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
		free(buffer);
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

	if (argc <= 1) {
		NSGDX::Util::print("Usage: nsgdx [FILE]\n\n");
	} else {
		string path = argv[1];
		run(path);
	}

	return 0;
}
