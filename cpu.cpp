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

using func_t = void (*)(int, int);
func_t OP[] = {NOP, MOV, ADD, SUB, NOT, OR, AND, XOR, RET};

CMD PROG[] = {(2 << 16) | (6 << 8) | 8};

int main() {
    MEMORY[6] = 6;
    MEMORY[8] = 8;
    int n = 1;
    for (int i = 0; i < n; i++) {
        DW R = PROG[i] % (2 << 8);
        PROG[i] >>= 8;
        DW L = PROG[i] % (2 << 8);
        PROG[i] >>= 8;
        DW OPERATOR = PROG[i];
        cout << OPERATOR << " " << L << " " << R << endl;
        OP[OPERATOR](L, R);
    }
    cout << MEMORY[6] << endl;
    

    return 0;
}