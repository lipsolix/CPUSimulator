#include <iostream>
using namespace std;

typedef unsigned int DW;
typedef unsigned long long CMD;


DW MEMORY[256];

void NOP(int dst, int src) {}

void MOV(int dst, int src) {
    MEMORY[dst] = MEMORY[src];
}

void ADD(int dst, int src) {
    MEMORY[dst] += MEMORY[src];
}

void SUB(int dst, int src) {
    MEMORY[dst] -= MEMORY[src];
}

void OR(int dst, int src) {
    MEMORY[dst] |= MEMORY[src];
}

void AND(int dst, int src) {
    MEMORY[dst] &= MEMORY[src];
}

void XOR(int dst, int src) {
    MEMORY[dst] ^= MEMORY[src];
}

void NOT(int dst, int src) {
    MEMORY[dst] = ~MEMORY[dst];
}

void RET(int dst, int src) {
    if (dst == 80) {
        cout << MEMORY[0] << endl;
        return;
    }
}

using func_t = void (*)(int, int);
func_t OP[] = {NOP, MOV, ADD, SUB, NOT, OR, AND, XOR, RET};


void DOOP(CMD cmd) {
    DW R = cmd % 256;
    cmd /= 256;

    DW L = cmd % 256;
    cmd /= 256;

    DW PROPS = cmd % 256;
    cmd /= 256;

    DW OPERATOR = cmd % 256;
    
    cout << "-" << OPERATOR << " " << L << " " << R << endl;
    

    if (PROPS % 16 > 0) 
        R = MEMORY[R];

    PROPS /= 16;

    if (PROPS > 0) 
        L = MEMORY[L];

    cout << "+" << OPERATOR << " " << L << " " << R << endl;


    OP[OPERATOR](L, R);
}

CMD createCmd(int OPERATOR, bool isPtrL, bool isPtrR, int L, int R) {
    CMD tmp = (OPERATOR << 24) | (isPtrL << 20) | (isPtrR << 16) | (L << 8) | R;
    cout << tmp << endl;
    return tmp;
}

CMD parseCmd(string cmdString) {
    //TODO
}

CMD PROG[] = {
    createCmd(1, 1, 1, 6, 8),
    createCmd(1, 1, 1, 0, 6),
    createCmd(8, 0, 0, 80, 0)
    };

int main() {
    MEMORY[6] = 10;
    MEMORY[8] = 11;
    int n = 3;
    for (int i = 0; i < n; i++) {
        DOOP(PROG[i]);
    }
    

    return 0;
}