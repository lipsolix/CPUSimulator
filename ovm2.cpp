#define AX REGISTER[0]
#define BX REGISTER[1]
#define CX REGISTER[2]
#define DX REGISTER[3]
#define SI REGISTER[4]
#define DI REGISTER[5]
#define SP REGISTER[6]
#define BP REGISTER[7]
#define IP REGISTER[8]
#define CF FLAG[0]
#define PF FLAG[2]
#define AF FLAG[4]
#define ZF FLAG[6]
#define SF FLAG[7]
#define LREG REGISTER[L]
#define RREG REGISTER[R]
#define LMEM MEMORY[L]
#define RMEM MEMORY[R]

#define BOOL bool
#define WORD unsigned int
#define REGINDEX unsigned int
#define MEMINDEX unsigned int


#include <iostream>
using namespace std;



WORD REGISTER[16] = {0};
BOOL FLAG[16] = {0};
WORD MEMORY[1024] = {0};

/* MOVE */

void MOVRR(REGINDEX L, REGINDEX R) {
    LREG = RREG;
    ++IP;
}

void MOVRM(REGINDEX L, MEMINDEX R) {
    LREG = RMEM;
    ++IP;
}

void MOVMR(MEMINDEX L, REGINDEX R) {
    LMEM = RREG;
    ++IP;
}

void MOVMM(MEMINDEX L, MEMINDEX R) {
    LMEM = RMEM;
    ++IP;
}

/* ADDITION */

void ADDRR(REGINDEX L, REGINDEX R) {
    LREG += RREG;
    ++IP;
}

void ADDRM(REGINDEX L, MEMINDEX R) {
    LREG += RMEM;
    ++IP;
}

void ADDMR(MEMINDEX L, REGINDEX R) {
    LMEM += RREG;
    ++IP;
}

void ADDMM(MEMINDEX L, MEMINDEX R) {
    LMEM += RMEM;
    ++IP;
}

void ADDRL(REGINDEX L, WORD value) {
    LREG += value;
    ++IP;
}

void ADDML(MEMINDEX L, WORD value) {
    LMEM += value;
    ++IP;
}

/* SUBSTRACTION */

void SUBRR(REGINDEX L, REGINDEX R) {
    LREG -= RREG;
    ++IP;
}

void SUBRM(REGINDEX L, MEMINDEX R) {
    LREG -= RMEM;
    ++IP;
}

void SUBMR(MEMINDEX L, REGINDEX R) {
    LMEM -= RREG;
    ++IP;
}

void SUBMM(MEMINDEX L, MEMINDEX R) {
    LMEM -= RMEM;
    ++IP;
}

void SUBRL(REGINDEX L, WORD value) {
    LREG -= value;
    ++IP;
}

void SUBML(MEMINDEX L, WORD value) {
    LMEM -= value;
    ++IP;
}

/* LOAD */

void LDRL(REGINDEX L, WORD value) {
    LREG = value;
    ++IP;
}

void LDML(MEMINDEX L, WORD value) {
    LMEM = value;
    ++IP;
}

/* INC */

void INCR(REGINDEX L, WORD _) {
    ++LREG;
    ++IP;
}

void INCM(REGINDEX L, WORD _) {
    ++LMEM;
    ++IP;
}

/* DEC */

void INCR(REGINDEX L, WORD _) {
    --LREG;
    ++IP;
}

void INCM(REGINDEX L, WORD _) {
    --LMEM;
    ++IP;
}