#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>

using namespace boost::uuids;
using namespace std;

int main()
{
    random_generator gen;
    unsigned char uuid_data[8];
    uuid id = gen();
    cout << id << endl;
    cout << id.size() << endl;
    cout << id.variant() << endl;
    cout << to_string(id) << endl;
    cout << sizeof(id.data) << endl;
    cout << id.data << '.' << endl;
    memcpy(uuid_data, id.data, sizeof(uuid_data));
    cout << uuid_data << '.' << endl;
    uuid id2;
    cout << memcmp(uuid_data, id2.data, sizeof(uuid_data)) << endl;
    cout << id2 << endl;
    uuid id3;
    cout << memcmp(uuid_data, id3.data, sizeof(uuid_data)) << endl;
    cout << id3 << endl;
    return 0;
}