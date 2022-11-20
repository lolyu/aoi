#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    streambuf *backup;
    std::ofstream filestr;
    filestr.open("test.txt");

    backup = cout.rdbuf(filestr.rdbuf());

    std::cout << "This is written to the file";

    std::cout.rdbuf(backup);

    filestr.close();

    return 0;
}
