#include <iostream>
#include <swss/producerstatetable.h>

int main()
{
    const std::string dbname = "APPL_DB";
    const std::string table_name = "PSEUDOTABLE";
    swss::DBConnector context(dbname, 0, true);

    swss::Table t(&context, table_name);
    swss::ProducerStateTable pst(&context, table_name);

    std::vector<swss::FieldValueTuple> fvs = {
        {"key0", "value0"},
        {"key1", "value1"}};
    pst.set("ENTRY0", fvs);
    pst.del("ENTRY0");

    std::vector<swss::KeyOpFieldsValuesTuple> vkco = {
        {"ENTRY1", "", {{"key0", "value0"}, {"key1", "value1"}}},
        {"ENTRY2", "", {{"key0", "value0"}, {"key1", "value1"}}},
    };
    std::vector<std::string> keys = {"ENTRY1", "ENTRY2"};
    pst.set(vkco);
    pst.del(keys);

    pst.set(vkco);
    pst.clear();

    vkco = {
        {"ENTRY0", "", {{"key0", "value0"}, {"key1", "value1"}, {"key2", "value2"}}},
        {"ENTRY1", "", {{"key0", "value0"}, {"key1", "value1"}}},
        {"ENTRY2", "", {{"key0", "value0"}, {"key1", "value1"}}},
    };
    for (auto &kco : vkco)
    {
        t.set(kfvKey(kco), kfvFieldsValues(kco));
    }

    pst.create_temp_view();
    vkco = {
        {"ENTRY0", "", {{"key0", "value0"}, {"key1", "value11"}, {"key3", "value3"}}},
        {"ENTRY3", "", {{"key0", "value0"}, {"key1", "value1"}}},
    };
    pst.set(vkco);
    pst.apply_temp_view();

    pst.clear();

    keys.push_back("ENTRY0");
    for (auto &key : keys)
    {
        t.del(key);
    }
    return 0;
}
