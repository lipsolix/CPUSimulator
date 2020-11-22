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

WORD* REGISTER;
WORD* MEMORY;


/* MOV */

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

/* ADD */

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

/* SUB */

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

/* XOR */

void XORRR(REGINDEX L, REGINDEX R) {
    LREG ^= RREG;
    ++IP;
}

void XORRM(REGINDEX L, MEMINDEX R) {
    LREG ^= RMEM;
    ++IP;
}

void XORMR(MEMINDEX L, REGINDEX R) {
    LMEM ^= RREG;
    ++IP;
}

void XORMM(MEMINDEX L, MEMINDEX R) {
    LMEM ^= RMEM;
    ++IP;
}

void XORRL(REGINDEX L, WORD value) {
    LREG ^= value;
    ++IP;
}

void XORML(MEMINDEX L, WORD value) {
    LMEM ^= value;
    ++IP;
}

/* OR */

void ORRR(REGINDEX L, REGINDEX R) {
    LREG |= RREG;
    ++IP;
}

void ORRM(REGINDEX L, MEMINDEX R) {
    LREG |= RMEM;
    ++IP;
}

void ORMR(MEMINDEX L, REGINDEX R) {
    LMEM |= RREG;
    ++IP;
}

void ORMM(MEMINDEX L, MEMINDEX R) {
    LMEM |= RMEM;
    ++IP;
}

void ORRL(REGINDEX L, WORD value) {
    LREG |= value;
    ++IP;
}

void ORML(MEMINDEX L, WORD value) {
    LMEM |= value;
    ++IP;
}

/* AND */

void ANDRR(REGINDEX L, REGINDEX R) {
    LREG &= RREG;
    ++IP;
}

void ANDRM(REGINDEX L, MEMINDEX R) {
    LREG &= RMEM;
    ++IP;
}

void ANDMR(MEMINDEX L, REGINDEX R) {
    LMEM &= RREG;
    ++IP;
}

void ANDMM(MEMINDEX L, MEMINDEX R) {
    LMEM &= RMEM;
    ++IP;
}

void ANDRL(REGINDEX L, WORD value) {
    LREG &= value;
    ++IP;
}

void ANDML(MEMINDEX L, WORD value) {
    LMEM &= value;
    ++IP;
}

/* PUSH */

void PUSHR(REGINDEX L, WORD _) {
    SP -= 1;
    MEMORY[SP] = LREG;
    ++IP;
}

void PUSHM(MEMINDEX L, WORD _) {
    SP -= 1;
    MEMORY[SP] = LMEM;
    ++IP;
}

/* POP */

void POPR(REGINDEX L, WORD _) {
    LREG = MEMORY[SP];
    SP += 1;
    ++IP;
}

void POPM(MEMINDEX L, WORD _) {
    LMEM = MEMORY[SP];
    SP += 1;
    ++IP;
}

/* JMP */

void JMPR(REGINDEX L, WORD _) {
    IP = LREG;
}

void JMPM(MEMINDEX L, WORD _) {
    IP = LMEM;
}

/* GO */

void GOR(REGINDEX L, WORD _) {
    MEMORY[SP-1] = IP;
    MEMORY[SP-2] = BP;
    SP = BP;
}

/* RET */

void RETR(REGINDEX L, WORD __) {
    IP = MEMORY[BP+1];
    MEMORY[BP+1] = LREG;
    BP = MEMORY[BP];
    SP = BP;
}

void RETR(MEMINDEX L, WORD __) {
    IP = MEMORY[BP+1];
    MEMORY[BP+1] = LMEM;
    BP = MEMORY[BP];
    SP = BP;
}
