#include <iostream>
#include <random>
#include <map>
#include <set>
using namespace std;

#define MAXMEMORYADDRESS 1024
typedef unsigned long long CMD;


class MEMORYOBJECT {
    set<int> ALLOCMEMSET;
    map<int, int> MEMMAP;
    map<string, pair<int, int>> VARTOSIZEADDRESSMAP;
    private:
        int allocMemory(int size) {
            //int address = rand() % MAXMEMORYADDRESS;
            int address = 0;

            int freeMemoryCells = 0;
            do {
                if (ALLOCMEMSET.count(address+freeMemoryCells) == 0) {
                    freeMemoryCells++;
                } else {
                    address += freeMemoryCells > 0 ? freeMemoryCells : 1;
                    freeMemoryCells = 0;
                }
            } while (freeMemoryCells < size);
            for (int i = address; i < address + size; i++) {
                ALLOCMEMSET.insert(i);
            }
            return address;
        }

        int freeMemory(int address, int size) {
            for (int i = address; i < address + size; i++) {
                ALLOCMEMSET.erase(i);
            }
            return size;
        }

    public:
        int& operator[](int memoryAddress) {
            if (ALLOCMEMSET.count(memoryAddress) != 0)
                return MEMMAP[memoryAddress];
            else {
                cout << "Access to address " << memoryAddress << endl; 
                cout << "Segmentation fault" << endl;
                exit(-1);
            }
        }

        int& operator[](string varName) {
            if (VARTOSIZEADDRESSMAP.count(varName) != 0)
                return (*this)[VARTOSIZEADDRESSMAP[varName].second];
            else {
                cout << "Undefined variable " << '"' << varName << '"' << endl;
                exit(-1);
            }
        }

        int defineVar(string varName, int size) {
            freeVar(varName);
            pair<int, int> &item = VARTOSIZEADDRESSMAP[varName];
            item.first = size;
            item.second = allocMemory(size);
            return item.second;
        }

        int freeVar(string varName) {
            pair<int, int> &item = VARTOSIZEADDRESSMAP[varName];
            freeMemory(item.second, item.first);
            VARTOSIZEADDRESSMAP.erase(varName);
            return item.first;
        }

        void showVarToAddress() {
            for (auto p : VARTOSIZEADDRESSMAP) 
                cout << p.first << " " << p.second.first << " " << p.second.second <<  endl;
        }
        
        void showAllocMemSet() {
            for (auto p : ALLOCMEMSET) 
                cout << p <<  endl;
        }        

        void showMemory() {
            for (auto p : MEMMAP) 
                cout << p.first << " " << p.second << endl;
        }
};

MEMORYOBJECT MEMORY;

int IPointer = 0;
int IStack[100] = {0};
int IStackPointer = 0;

int NOP(int& A, int& B) {
    IPointer++;
    return 0;
}

int MOV(int& A, int& B) {
    IPointer++;
    MEMORY[A] = MEMORY[B];
    return 0;
}

int ADD(int& A, int& B) {
    IPointer++;
    MEMORY[A] += MEMORY[B];
    return 0;
}

int SUB(int& A, int& B) {
    IPointer++;
    MEMORY[A] -= MEMORY[B];
    return 0;
}

int NOT(int& A, int& B) {
    IPointer++;
    MEMORY[A] = ~MEMORY[A];
    return 0;
}

int OR(int& A, int& B) {
    IPointer++;
    MEMORY[A] |= MEMORY[B];
    return 0;
}

int AND(int& A, int& B) {
    IPointer++;
    MEMORY[A] &= MEMORY[B];
    return 0;
}

int XOR(int& A, int& B) {
    IPointer++;
    MEMORY[A] ^= MEMORY[B];
    return 0;
}

int LD(int& A, int& B) {
    IPointer++;
    MEMORY[A] = B;
    return 0;
}

int HLT(int& A, int& B) {
    IPointer = -1;
    return 0;
}

int LET(string varName, int size) {
    IPointer++;
    return MEMORY.defineVar(varName, size);
}

int LAB(string labelName, int _) {
    IPointer++;
    return MEMORY.defineVar(labelName, IPointer);
}

int REM(string varName, int _) {
    IPointer++;
    return MEMORY.freeVar(varName);
}

using func_t = int (*)(int&, int&);
func_t OPERATION[] = {NOP, MOV, ADD, SUB, NOT, OR, AND, XOR, LD, HLT};

using mem_func_t = int (*)(string, int);
mem_func_t MEMOPERATION[] = {LET, LAB, REM};

void DOOP(CMD cmd) {
    int R = cmd % 256;
    cmd /= 256;
    int L = cmd % 256;
    cmd /= 256;
    int PROPS = cmd % 256;
    cmd /= 256;
    int OPERATOR = cmd % 256;
    //cout << "-" << OPERATOR << " " << L << " " << R << endl;    
    if (PROPS % 16 > 0) 
        R = MEMORY[R];
    PROPS /= 16;
    if (PROPS > 0) 
        L = MEMORY[L];
    //cout << "+" << OPERATOR << " " << L << " " << R << endl;
    OPERATION[OPERATOR](L, R);
}

int parseOperatorFromString(string OPERATOR) {
    //cout << OPERATOR << endl;
    if (OPERATOR.compare("NOP") == 0) {
        return 0;
    } else if (OPERATOR.compare("MOV") == 0) {
        return 1;
    } else if (OPERATOR.compare("ADD") == 0) {
        return 2;
    }  else if (OPERATOR.compare("SUB") == 0) {
        return 3;
    } else if (OPERATOR.compare("NOT") == 0) {
        return 4;
    } else if (OPERATOR.compare("OR") == 0) {
        return 5;
    } else if (OPERATOR.compare("AND") == 0) {
        return 6;
    } else if (OPERATOR.compare("XOR") == 0) {
        return 7;
    } else if (OPERATOR.compare("LD") == 0) {
        return 8;
    } else if (OPERATOR.compare("SAY") == 0) {
        return 9;
    } else if (OPERATOR.compare("HLT") == 0) {
        return 10;
    } else if (OPERATOR.compare("LET") == 0) { //MEMORY OPS
        return 100;
    } else if (OPERATOR.compare("LAB") == 0) {
        return 101;
    } else if (OPERATOR.compare("REM") == 0) {
        return 102;
    } else 
        return 0;
}

void parseCMDold(string CMD, int &OPERATOR, bool &isPtrL, bool &isPtrR, int &L, int &R) {
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
    if (CMD.at(startIndex) <= '9' && '0' <= CMD.at(startIndex)) {
        L = atoi(CMD.substr(startIndex, splitter).c_str());
    } else {
        L = MEMORY[CMD.substr(startIndex, splitter)];
        cout << "var as " << endl;
    }
    
    CMD = CMD.substr(splitter+1);
    startIndex = 0;
    isPtrR = false;
    if (CMD[0] == '%') { 
        isPtrR = true;
        startIndex = 1;
    }

    if (CMD.at(startIndex) <= '9' && '0' <= CMD.at(startIndex)) {
        R = atoi(CMD.substr(startIndex).c_str());
    } else {
        R = MEMORY[CMD.substr(startIndex, splitter)];
        cout << "var as R" << endl;
    }
}


void parseCMD(string CMD, int &OPERATOR, bool &isPtrL, bool &isPtrR, int &L, int &R) {
    
    int splitter;
    string leftString, rightString;
    bool isLeftLiteral, isRightLiteral;
    CMD = CMD.substr(CMD.find_first_not_of(" "));
    splitter = CMD.find_first_of(" ");
    OPERATOR = parseOperatorFromString(CMD.substr(0, splitter));
    //cout << CMD << endl;
    CMD = CMD.substr(splitter+1);
    CMD = CMD.substr(CMD.find_first_not_of(" "));
    //cout << "before L " << CMD << endl;
    isPtrL = false;
    if (CMD.at(0) == '*') {
        isPtrL = true;
        CMD = CMD.substr(1);
    }
    splitter = CMD.find_first_of(" ");
    //cout << CMD << endl;
    isLeftLiteral = false;
    if (CMD.at(0) <= '9' && '0' <= CMD.at(0)) {
        isLeftLiteral = true;
    }
    leftString = CMD.substr(0, splitter);

    CMD = CMD.substr(CMD.find_first_of(" "));
    CMD = CMD.substr(CMD.find_first_not_of(" "));

    //cout << "before R " << CMD << endl;
    isPtrR = false;
    if (CMD.at(0) == '*') { 
        isPtrR = true;
        CMD = CMD.substr(1);
    }
    isRightLiteral = false;
    if (CMD.at(0) <= '9' && '0' <= CMD.at(0)) {
        isRightLiteral = true;
    }
    rightString = CMD.substr(0, splitter);

    if (OPERATOR >= 100) {
        MEMOPERATION[OPERATOR-100](leftString, atoi(rightString.c_str()));
        OPERATOR = 0;
        isPtrL = false;
        isPtrR = false;
        L = 0;
        R = 0;
    } else {
        if (isLeftLiteral) {
            L = atoi(leftString.c_str());
        } else {
            L = MEMORY[leftString];
        }
        if (isRightLiteral) {
            R = atoi(rightString.c_str());
        } else {
            R = MEMORY[rightString];
        }
    }
}

CMD createCmd(int OPERATOR, bool isPtrL, bool isPtrR, int L, int R) {
    CMD tmp = (OPERATOR << 24) | (isPtrL << 20) | (isPtrR << 16) | (L << 8) | R;
    cout << OPERATOR << " " << isPtrL << " " << isPtrR << " " << L << " " << R << endl;
    //cout << tmp << endl;
    return tmp;
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
        cout << cmd[i] << endl;
        PROG = PROG.substr(PROG.find(';')+1);
    }
    return cmd;
}

int main() {
    string prog = "LET KEK 1; HLT 0 0;";
    CMD* PROG = parseCMDs(prog);
    while (IPointer != -1) {
        DOOP(PROG[IPointer]);
    }
    return 0;
}