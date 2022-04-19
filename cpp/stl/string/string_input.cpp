#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    ifstream fin;
    fin.open("input.txt");
    if (!fin.is_open()) {
        cerr << "Cannot open file. Bye!" << endl;
        exit(EXIT_FAILURE);
    }
    string line;
    int count = 0;
    while (fin) {
        getline(fin, line, '.');
        cout << ++count << ": " << line << endl;
    }
    cout << "Done" << endl;
    fin.close();
    return 0;
}