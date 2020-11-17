#include <iostream>
#include <fstream>
#include <map>
using namespace std;

#define EAX MEMORY[0]
#define EBX MEMORY[1]
#define ECX MEMORY[2]
#define EDX MEMORY[3]
#define ESI MEMORY[4]
#define EDI MEMORY[5]
#define ESP MEMORY[6]
#define EBP MEMORY[7]
#define IP MEMORY[8]




#define REGISTERSSIZE 32
#define STACKSIZE 256
typedef unsigned long long CMD;
using func_t = void(*)(bool, bool, unsigned int, unsigned int);

map<unsigned int, unsigned int> virtualMemory;
unsigned int registerMemory[REGISTERSSIZE] = {0}; // EAX, EBX, ECX, EDX, ESI, EDI, ESP, EBP, IP, FLAGS
unsigned int stackMemory[STACKSIZE] = {0};

class MemoryController {
    public:
        unsigned int& operator[](unsigned int address) {
            if (address < REGISTERSSIZE) {
                return registerMemory[address];
            } else if (address < STACKSIZE) {
                return stackMemory[address];
            } else {
                return virtualMemory[address];
            }
        }
};

MemoryController MEMORY;

void NOP(bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    ++IP;
}

void MOV(bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    MEMORY[L] = MEMORY[R];
    ++IP;
}

void ADD(bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    if (!isLiteralR) {
        MEMORY[L] += MEMORY[R];
    } else {
        MEMORY[L] += R;
    }
    ++IP;
}

void SUB(bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    if (!isLiteralR) {
        MEMORY[L] -= MEMORY[R];
    } else {
        MEMORY[L] -= R;
    }
    ++IP;
}

void NOT(bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    MEMORY[L] = ~MEMORY[L];
    ++IP;
}

void OR(bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    if (!isLiteralR) {
        MEMORY[L] |= MEMORY[R];
    } else {
        MEMORY[L] |= R;
    }
    ++IP;
}

void AND(bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    if (!isLiteralR) {
        MEMORY[L] &= MEMORY[R];
    } else {
        MEMORY[L] &= R;
    }
    ++IP;
}

void XOR(bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    if (!isLiteralR) {
        MEMORY[L] ^= MEMORY[R];
    } else {
        MEMORY[L] ^= R;
    }
    ++IP;
}

void LD(bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    MEMORY[L] = R;
    ++IP;
}

void HLT(bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    IP = 0;
}

void SAY(bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    if (!isLiteralL) { 
        cout << MEMORY[L] << endl;
    } else {
        cout << L << endl;
    }
    ++IP;
}

void JMP(bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    if (!isLiteralL) {
        IP = MEMORY[L];
    } else {
        IP = L;
    }
}

func_t OPERATION[] = {NOP, MOV, ADD, SUB, NOT, OR, AND, XOR, LD, HLT, JMP, NOP, NOP, SAY}; 

void DOOP(CMD cmd) {
    bool isLiteralL = false;
    bool isLiteralR = false;

    unsigned int R = cmd % (1 << 16);
    cmd >>= 16;

    unsigned int L = cmd % (1 << 16);
    cmd >>= 16;

    unsigned int  PROPS = cmd % (16);
    cmd >>= 4;

    unsigned int OPERATOR = cmd;
    
    //cout << "-" << OPERATOR << " " << L << " " << R << endl;
    
    if (PROPS % 2) 
        R = MEMORY[R];

    PROPS >>= 1;

    if (PROPS % 2) 
        L = MEMORY[L];

    PROPS >>= 1;

    if (PROPS % 2) {
        isLiteralR = true;
    }

    PROPS >>= 1;

    if (PROPS % 2) {
        isLiteralL = true;
    }
    
    //cout << "+" << OPERATOR << " " << L << " " << R << endl;
    cout << OPERATOR << " " << isLiteralL << " " << isLiteralR << " " << L << " " << R << endl;
    OPERATION[OPERATOR](isLiteralL, isLiteralR, L, R);
}

void showBinary(CMD cmd) {
    CMD cpy = cmd;
    string s;
    string s2;
    int i = 0;
    while (i < 80) {
        s.append(cmd%2 ? "1" : "0");
        cmd >>= 1;
        ++i;
    }
    while (s.length()) {
        s2.push_back(s.back());
        s.pop_back();
    }
    cout << s2 << " " << cpy << endl;
}

CMD* getCmdsFromFile(string filename, int& cmdCount) {
    ifstream file(filename, ios::binary);
    string result;
    string tmp;
    while (!file.eof()) {
        getline(file, tmp);
        result.append(tmp);
        tmp.erase();
    }
    cmdCount = result.length() / 8;
    CMD* cmds = new CMD[cmdCount];
    const char* charSeq = result.c_str();
    for (int i = 0; i < cmdCount; i++) {
        cmds[i] = ((CMD*)charSeq)[i];
        //showBinary(cmds[i]);
    }
    return cmds;
}

int main() {
    int count;
    CMD* program = getCmdsFromFile("prog.ovm", count);

    cout << "---------------------INSTRUCTIONS--STARTED---------------------" << endl;
    for (int i = 0; i < count; i++) {
        showBinary(program[i]);
    }
    cout << "----------------------INSTRUCTIONS--ENDED----------------------" << endl;

    IP = 0;
    do {
        cout << "IP: " << IP << endl;
        DOOP(program[IP]);
    } while (IP < 10 && IP != 0);


    return 0;
}