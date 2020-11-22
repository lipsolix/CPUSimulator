#include <iostream>
#include <fstream>

using namespace std;

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

int main() {
    string PROG = parseProgFromFile("prog.oleg");
    cout << PROG << endl;
    return 0;
}