#include <iostream>
#include <fstream>
#include <map>
using namespace std;


#define REGISTERSSIZE 32
#define STACKSIZE 256
typedef unsigned long long CMD;
using func_t = void(*)(unsigned int&, bool, bool, unsigned int, unsigned int);

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

void NOP(unsigned int& IP, bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    ++IP;
}

void MOV(unsigned int& IP, bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    if (!isLiteralR) {
        MEMORY[L] = MEMORY[R];
    } else {
        MEMORY[L] = R;
    }
    ++IP;
}

void ADD(unsigned int& IP, bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    if (!isLiteralR) {
        MEMORY[L] += MEMORY[R];
    } else {
        MEMORY[L] = R;
    }
    ++IP;
}

void SUB(unsigned int& IP, bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    if (!isLiteralR) {
        MEMORY[L] -= MEMORY[R];
    } else {
        MEMORY[L] = R;
    }
    ++IP;
}

void NOT(unsigned int& IP, bool isLiteralL, bool isLiteralR, unsigned int L, unsigned int R) {
    MEMORY[L] = ~MEMORY[L];
    ++IP;
}


func_t OPERATION[] = {NOP, MOV};
