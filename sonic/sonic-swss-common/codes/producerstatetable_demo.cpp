#include <hiredis/hiredis.h>

#include <iostream>
#include <string>
#include <vector>

#include "swss/producerstatetable.h"
#include "swss/rediscommand.h"

using namespace std;

int main()
{
    swss::DBConnector dbConnector("CONFIG_DB", 0, true);
    swss::ProducerStateTable producer(&dbConnector, "EMPLOYEE");
    std::vector<swss::FieldValueTuple> values = {{"name", "alice"}, {"age", "18"}};
    for (auto &fv : values) {
        std::cout << fvField(fv) << ": " << fvValue(fv) << std::endl;
    }
    producer.set("ALICE", values);
    return 0;
}
