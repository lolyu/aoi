#include <iostream>
#include <string>

using namespace std;

int main()
{
    string str = "elhelloworldhellojesuselloworld";
    const char* substr = "ello";
    string::size_type pos;
    pos = str.find(substr);
    cout << "Position: " << pos << endl;

    pos = str.find(substr, 6);
    cout << "Position: " << pos << endl;

    pos = str.find(substr, 0, 2);
    cout << "Position: " << pos << endl;

    pos = str.rfind(substr);
    cout << "Position: " << pos << endl;

    pos = str.find_first_of("wz");
    cout << "Position: " << pos << endl;

    pos = str.find_last_of("wz");
    cout << "Position: " << pos << endl;

    pos = str.find_first_not_of("aoei");
    cout << "Position: " << pos << endl;

    pos = str.find_last_not_of("wz");
    cout << "Position: " << pos << endl;

    pos = str.find("zzz");
    cout << "Position: " << pos << endl;

    return 0;
}