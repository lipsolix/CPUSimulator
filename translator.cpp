#include <set>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#define MAXMEMORYADDRESS 8192
#define MINMEMORYADDRESS 1024
typedef unsigned long long CMD;
using namespace std;

map<string, pair<int, int>> VarNameToSizeAndMemory;
set<int> UsedMemory;

class MEMORYOBJECT {
    set<int> UsedMemory;
    map<string, pair<int, int>> VarNameToSizeAndMemory;
    private:
        int allocMemory(int size) {
            //int address = (MINMEMORYADDRESS + rand()) % MAXMEMORYADDRESS;
            int address = MINMEMORYADDRESS;

            int freeMemoryCells = 0;
            do {
                if (UsedMemory.count(address+freeMemoryCells) == 0) {
                    freeMemoryCells++;
                } else {
                    address += freeMemoryCells > 0 ? freeMemoryCells : 1;
                    freeMemoryCells = 0;
                }
            } while (freeMemoryCells < size && address < MAXMEMORYADDRESS);
            for (int i = address; i < address + size; i++) {
                UsedMemory.insert(i);
            }
            return address;
        }

        int freeMemory(int address, int size) {
            for (int i = address; i < address + size; i++) {
                UsedMemory.erase(i);
            }
            return size;
        }

    public:
        int& operator[](string varName) {
            if (VarNameToSizeAndMemory.count(varName) != 0)
                return VarNameToSizeAndMemory[varName].second;
            else {
                cout << "Undefined variable " << '"' << varName << '"' << endl;
                exit(-1);
            }
        }

        int defineVar(string varName, int size) {
            freeVar(varName);
            pair<int, int> &item = VarNameToSizeAndMemory[varName];
            item.first = size;
            item.second = allocMemory(size);
            return item.second;
        }

        int freeVar(string varName) {
            pair<int, int> &item = VarNameToSizeAndMemory[varName];
            freeMemory(item.second, item.first);
            VarNameToSizeAndMemory.erase(varName);
            return item.first;
        }

        void showVarToAddress() {
            for (auto p : VarNameToSizeAndMemory) 
                cout << p.first << " " << p.second.first << " " << p.second.second <<  endl;
        }
        
        void showAllocMemSet() {
            for (auto p : UsedMemory) 
                cout << p <<  endl;
        }
};

MEMORYOBJECT MEMORY;

int DEFINEVAR(string varName, int size) {
    return MEMORY.defineVar(varName, size);
}

int DEFINELABEL(string varName, int size) {
    return MEMORY.defineVar(varName, 1);
}

int FREEVAR(string varName, int _) {
    MEMORY.freeVar(varName);
    return 0;
}

int parseOperator(string OPERATOR) {
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
    } else if (OPERATOR.compare("LD") == 0) {
        return 8;
    } else if (OPERATOR.compare("HLT") == 0) {
        return 9;
    } else if (OPERATOR.compare("SAY") == 0) {
        return 13;
    } else if (OPERATOR.compare("LET") == 0) {
        return 100;
    } else if (OPERATOR.compare("LAB") == 0) {
        return 101;
    } else if (OPERATOR.compare("CLR") == 0) {
        return 102;
    } else
        return 0;
}

using mem_func_t = int(*)(string, int);
mem_func_t MEMOPS[] = {DEFINEVAR, DEFINELABEL, FREEVAR};

CMD createCmd(int OPERATOR, bool isLiteralL, bool isLiteralR, bool isPtrL, bool isPtrR, int L, int R) {
    CMD tmp = ((CMD)OPERATOR << 56) | ((CMD)isLiteralL << 55) | ((CMD)isLiteralR << 54) | ((CMD)isPtrR << 53) | ((CMD)isPtrR << 52) | ((CMD)L << 26) | (CMD)R;
    //cout << OPERATOR << " " << isPtrL << " " << isPtrR << " " << L << " " << R << endl;
    //cout << tmp << endl;
    return tmp;
}

void parseCommand(string Command, int& Operator, bool& isLiteralL, bool& isLiteralR, bool& isPtrL, bool& isPtrR, int& L, int& R) {
    int splitter;
    string operatorString;
    string leftString;
    string rightString;
    Command = Command.substr(Command.find_first_not_of(' '));
    splitter = Command.find_first_of(' ');
    operatorString = Command.substr(0, splitter);
    Command = Command.substr(splitter + 1);
    Command = Command.substr(Command.find_first_not_of(' '));
    isPtrL = false;
    isLiteralL = false;
    splitter = Command.find_first_of(' ');
    if (Command.at(0) == '[') {
        isPtrL = true;
        leftString = Command.substr(1, splitter - 2);
    } else if (Command.at(0) == '(') {
        isLiteralL = true;
        leftString = Command.substr(1, splitter - 2);
    } else{
        leftString = Command.substr(0, splitter);
    }
    Command = Command.substr(splitter+1);
    Command = Command.substr(Command.find_first_not_of(' '));
    isPtrR = false;
    isLiteralR = false;
    if (Command.at(0) == '[') { 
        isPtrR = true;
        rightString = Command.substr(1, Command.find_first_of(']')-1);
    } else if (Command.at(0) == '(') {
        isLiteralR = true;
        rightString = Command.substr(1, Command.find_first_of(')')-1);
    } else {
        rightString = Command.substr(0);
    }
    if ('0' <= rightString.at(0) && rightString.at(0) <= '9') {
        R = atoi(rightString.c_str());
    } else {
        R = MEMORY[rightString];
    }

    Operator = parseOperator(operatorString);
    if (Operator < 100) {
        if ('0' <= leftString.at(0) && leftString.at(0) <= '9') {
            L = atoi(leftString.c_str());
        } else {
            L = MEMORY[leftString];
        }
    } else {
        int address = MEMOPS[Operator - 100](leftString, R);
        Operator = 8; // loads IP value to address
        isPtrL = false;
        isPtrR = false;
        isLiteralL = 0;
        isLiteralR = 0;
        L = address;
        R = 1; // first cell address stores IP
    }

    cout << "'" << operatorString << "'->'" << leftString << "'->'" << rightString << "'" << endl;
    cout << Operator << " " << isPtrL << " " << isPtrR << " " << isLiteralL << " " << isLiteralR << " " << L << " " << R << endl;
}


void showBinary(CMD cmd) {
    string s;
    string s2;
    while (cmd != 0) {
        s.append(cmd%2 ? "1" : "0");
        cmd >>= 1;
    }
    while (s.length()) {
        s2.push_back(s.back());
        s.pop_back();
    }
    cout << s2 << endl;
}



CMD* parseCMDs(string PROG, int& cmdCount) {
    cmdCount = 0;
    for (char c : PROG) 
        if (c == ';')
            cmdCount++;
    //cout << cmdCount << endl;
    CMD* cmd = new CMD[cmdCount];
    int L, R, OPERATOR;
    bool isL, isR, isLiteralL, isLiteralR;
    for (int i = 0; i < cmdCount; i++) {
        parseCommand(PROG.substr(0, PROG.find(';')), OPERATOR, isLiteralL, isLiteralR, isL, isR, L, R);
        cmd[i] = createCmd(OPERATOR, isLiteralL, isLiteralR, isL, isR, L, R);
        cout << cmd[i] << endl;
        showBinary(cmd[i]);
        PROG = PROG.substr(PROG.find(';')+1);
    }
    return cmd;
}

string parseProgFromFile(string filename) {
    ifstream file(filename);
    string PROG;
    string tmp;
    while (!file.eof()) {
        getline(file, tmp);
        tmp = tmp.substr(0, tmp.find(";")+1);
        PROG.append(tmp);
    }
    return PROG;
}

void saveCmdsToFile(string filename, CMD* cmds, int cmdCount) {
    ofstream file(filename, ios::binary);
    for (int i = 0; i < cmdCount; i++) {
        for (int k = 0; k < 8; k++) {
            file << *(((char*)(cmds + i)) + k);
        }
    }
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
        showBinary(cmds[i]);
    }
    return cmds;
}

int main() {
    int cmdCount;
    string PROG = parseProgFromFile("prog.oleg");
    CMD* cmds = parseCMDs(PROG, cmdCount);
    saveCmdsToFile("prog.ovm", cmds, cmdCount);
    CMD* c = getCmdsFromFile("prog.ovm", cmdCount);
    cout << *c << endl;


    return 0;
}




