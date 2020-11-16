#include <iostream>
#include <fstream>
#include <sstream>



#define NEXTCMDREG 511
using namespace std;

typedef unsigned int DW;
typedef unsigned long long CMD;


DW MEMORY[512];

void NOP(int dst, int src) {
    MEMORY[NEXTCMDREG]++;
}

void MOV(int dst, int src) {
    MEMORY[NEXTCMDREG]++;
    MEMORY[dst] = MEMORY[src];
}

void ADD(int dst, int src) {
    MEMORY[NEXTCMDREG]++;
    MEMORY[dst] += MEMORY[src];
}

void SUB(int dst, int src) {
    MEMORY[NEXTCMDREG]++;
    MEMORY[dst] -= MEMORY[src];
}

void OR(int dst, int src) {
    MEMORY[NEXTCMDREG]++;
    MEMORY[dst] |= MEMORY[src];
}

void AND(int dst, int src) {
    MEMORY[NEXTCMDREG]++;
    MEMORY[dst] &= MEMORY[src];
}

void XOR(int dst, int src) {
    MEMORY[NEXTCMDREG]++;
    MEMORY[dst] ^= MEMORY[src];
}

void NOT(int dst, int src) {
    MEMORY[NEXTCMDREG]++;
    MEMORY[dst] = ~MEMORY[dst];
}

void RET(int dst, int src) {
    MEMORY[NEXTCMDREG]++;
    if (dst == 80) {
        cout << MEMORY[0] << endl;
        return;
    }
}

void SAY(int dst, int src) {
    MEMORY[NEXTCMDREG]++;
    cout << "SAY " << MEMORY[dst] << endl;
}

void LD(int dst, int src) {
    MEMORY[NEXTCMDREG]++;
    MEMORY[dst] = src;
}

void JMP(int dst, int src) {
    MEMORY[NEXTCMDREG] += dst;
}

void JNZF(int dst, int src) {
    if (MEMORY[dst] != 0)
        MEMORY[NEXTCMDREG] += src;
    else 
        MEMORY[NEXTCMDREG]++;
}

void JZF(int dst, int src) {
    if (MEMORY[dst] == 0)
        MEMORY[NEXTCMDREG] += src;
    else 
        MEMORY[NEXTCMDREG]++;
}

void JNZB(int dst, int src) {
    if (MEMORY[dst] != 0)
        MEMORY[NEXTCMDREG] -= src;
    else 
        MEMORY[NEXTCMDREG]++;
}

void JZB(int dst, int src) {
    if (MEMORY[dst] == 0)
        MEMORY[NEXTCMDREG] -= src;
    else 
        MEMORY[NEXTCMDREG]++;
}

void HLT(int dst, int src) {
    MEMORY[NEXTCMDREG] = -1;
}

using func_t = void (*)(int, int);
func_t OP[] = {NOP, MOV, ADD, SUB, NOT, OR, AND, XOR, RET, LD, JMP, JNZF, JZF, SAY, HLT, JNZB, JZB};


void DOOP(CMD cmd) {
    DW R = cmd % 256;
    cmd /= 256;

    DW L = cmd % 256;
    cmd /= 256;

    DW PROPS = cmd % 256;
    cmd /= 256;

    DW OPERATOR = cmd % 256;
    
    //cout << "-" << OPERATOR << " " << L << " " << R << endl;
    

    if (PROPS % 16 > 0) 
        R = MEMORY[R];

    PROPS /= 16;

    if (PROPS > 0) 
        L = MEMORY[L];

    //cout << "+" << OPERATOR << " " << L << " " << R << endl;

    OP[OPERATOR](L, R);
}

CMD createCmd(int OPERATOR, bool isPtrL, bool isPtrR, int L, int R) {
    CMD tmp = (OPERATOR << 24) | (isPtrL << 20) | (isPtrR << 16) | (L << 8) | R;
    //cout << OPERATOR << " " << isPtrL << " " << isPtrR << " " << L << " " << R << endl;
    //cout << tmp << endl;
    return tmp;
}

int parseOperatorFromString(string OPERATOR) {
    //cout << OPERATOR << endl;
    if (OPERATOR.compare("NOP") == 0) {
        return 0;
    } else if (OPERATOR.compare("MOV") == 0) {
        return 1;
    } else if (OPERATOR.compare("ADD") == 0) {
        return 2;
    } else if (OPERATOR.compare("SUB") == 0) {
        return 3;
    } else if (OPERATOR.compare("NOT") == 0) {
        return 4;
    } else if (OPERATOR.compare("OR") == 0) {
        return 5;
    } else if (OPERATOR.compare("AND") == 0) {
        return 6;
    } else if (OPERATOR.compare("XOR") == 0) {
        return 7;
    } else if (OPERATOR.compare("RET") == 0) {
        return 8;
    } else if (OPERATOR.compare("LD") == 0) {
        return 9;
    } else if (OPERATOR.compare("JMP") == 0) {
        return 10;
    } else if (OPERATOR.compare("JNZF") == 0) {
        return 11;
    } else if (OPERATOR.compare("JZF") == 0) {
        return 12;
    } else if (OPERATOR.compare("SAY") == 0) {
        return 13;
    } else if (OPERATOR.compare("HLT") == 0) {
        return 14;
    } else if (OPERATOR.compare("JNZB") == 0) {
        return 15;
    } else if (OPERATOR.compare("JZB") == 0) {
        return 16;
    }
    return 0;
}

void parseCMD(string CMD, int &OPERATOR, bool &isPtrL, bool &isPtrR, int &L, int &R) {
    int splitter;
    CMD = CMD.substr(CMD.find_first_not_of(" "));
    splitter = CMD.find_first_of(" ");
    //cout << CMD << endl;

    OPERATOR = parseOperatorFromString(CMD.substr(0, splitter));
    //cout << OPERATOR << endl;
    CMD = CMD.substr(splitter+1);

    int startIndex = 0;
    isPtrL = false;
    if (CMD[0] == '%') {
        isPtrL = true;
        startIndex++;
    }
    splitter = CMD.find_first_of(" ");
    L = atoi(CMD.substr(startIndex, splitter).c_str());

    CMD = CMD.substr(splitter+1);
    startIndex = 0;
    isPtrR = false;
    if (CMD[0] == '%') { 
        isPtrR = true;
        startIndex = 1;
    }

    R = atoi(CMD.substr(startIndex).c_str());
}

CMD* parseCMDs(string PROG) {
    int cmdCount = 0;
    for (char c : PROG) 
        if (c == ';')
            cmdCount++;
    //cout << cmdCount << endl;
    CMD* cmd = new CMD[cmdCount];
    int L, R, OPERATOR;
    bool isL, isR;
    for (int i = 0; i < cmdCount; i++) {
        parseCMD(PROG.substr(0, PROG.find(';')), OPERATOR, isL, isR, L, R);
        cmd[i] = createCmd(OPERATOR, isL, isR, L, R);
        //cout << cmd[i] << endl;
        PROG = PROG.substr(PROG.find(';')+1);
    }
    return cmd;
}

void dumpMemory(int start, int size) {
    for (int i = start; i < start + size; i++)
        cout << i << "->" << MEMORY[i] << endl;
}

int main(int argc, char* args[]) {
    bool debugMode = false;
    string filename;
    if (argc >= 2)
        filename = args[1];
    else 
        return 1;

    if (argc == 3) 
        debugMode = true;

    ifstream file(filename);
    string PROG;
    string tmp;
    while (!file.eof()) {
        getline(file, tmp);
        if (debugMode)
            cout << tmp << endl;
        tmp = tmp.substr(0, tmp.find(";")+1);
        PROG.append(tmp);
    }

    if (debugMode) 
        cout << PROG << endl;
    CMD* cmds = parseCMDs(PROG);
    MEMORY[NEXTCMDREG] = 0;
    
    while (MEMORY[NEXTCMDREG] != -1) {
        if (debugMode) {
            cout << MEMORY[NEXTCMDREG] << endl;
            cout << cmds[MEMORY[NEXTCMDREG]] << endl;
        }
        DOOP(cmds[MEMORY[NEXTCMDREG]]);
    }
    //dumpMemory(100, 20);


    return 0;
}