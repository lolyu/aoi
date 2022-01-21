#include <iostream>
#include "tabtenn0.h"

using namespace std;

void show(const TableTennisPlayer &rt);

int main()
{
    TableTennisPlayer player1("Tara", "Boomdea", false);
    RatedPlayer rplayer1(1140, "Mallory", "Duck", true);
    rplayer1.Name();
    if (rplayer1.hasTable())
    {
        cout << ": has a table." << endl;
    }
    else
    {
        cout << ": hasn't a table." << endl;
    }
    player1.Name();
    if (player1.hasTable())
    {
        cout << ": has a table." << endl;
    }
    else
    {
        cout << ": hasn't a table." << endl;
    }
    cout << "Name: ";
    rplayer1.Name();
    cout << "; Rating: " << rplayer1.Rating() << endl;
    RatedPlayer rplayer2(1212, player1);
    cout << "Name: ";
    rplayer2.Name();
    cout << "; Rating: " << rplayer2.Rating() << endl;

    // could use a pinter or reference to base class to reference derived class objects
    show(player1);
    show(rplayer1);
    TableTennisPlayer player2(rplayer1);
    show(player2);
    TableTennisPlayer player3 = rplayer2;
    show(player3);
    return 0;
}

void show(const TableTennisPlayer &rt)
{
    cout << "Name: ";
    rt.Name();
    cout << endl;
}