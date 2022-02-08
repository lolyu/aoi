#include "student.h"

using namespace std;

void set(Student &sa, int n);
const int pupils = 3;
const int quizzes = 5;

int main()
{
    Student ada[pupils] = {Student(quizzes), Student(quizzes), Student(quizzes)};
    for (int i = 0; i < pupils; ++i)
    {
        set(ada[i], quizzes);
    }
    cout << endl
         << "Student List:" << endl;
    for (int i = 0; i < pupils; ++i)
    {
        cout << ada[i].Name() << endl;
    }
    cout << endl
         << "Results:";
    for (int i = 0; i < pupils; ++i)
    {
        cout << endl;
        cout << ada[i];
        cout << "Average: " << ada[i].Average() << endl;
    }
    cout << "Done." << endl;
    return 0;
}

void set(Student &sa, int n)
{
    cout << "Please enter student name: ";
    getline(cin, sa);
    cout << "Please enter " << n << " quiz scores:" << endl;
    for (int i = 0; i < n; ++i)
    {
        cin >> sa[i];
    }
    while (cin.get() != '\n')
    {
        continue;
    }
}
