#include <iostream>

#include <swss/producerstatetable.h>
#include <swss/consumerstatetable.h>

template <template <typename, typename> typename T, typename M, typename N>
void print_vkco(const T<M, N> &vkco)
{
    static int index = 0;
    std::cout << std::endl
              << "----------------------------" << std::endl;
    std::cout << ++index << "th pop..." << std::endl;
    for (auto &kco : vkco)
    {
        std::cout << kfvKey(kco) << std::endl;
        std::cout << kfvOp(kco) << std::endl;
        for (auto &fv : kfvFieldsValues(kco))
        {
            std::cout << '\t' << fv.first << ": " << fv.second << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl
              << "----------------------------" << std::endl;
}

void print_table_entry(swss::Table &t, const std::string entry)
{
    std::vector<swss::FieldValueTuple> values;
    bool key_existed = t.get(entry, values);
    std::cout << "The table content for " << t.getKeyName(entry) << ":" << std::endl;
    std::cout << std::endl
              << "----------------------------" << std::endl;
    std::cout << entry << std::endl;
    if (key_existed)
    {
        for (auto &fv : values)
        {
            std::cout << '\t' << fv.first << ": " << fv.second << std::endl;
        }
    }
    else
    {
        std::cout << "NOT EXIST..." << std::endl;
    }

    std::cout << std::endl
              << "----------------------------" << std::endl;
}

int main()
{
    const std::string db_name = "APPL_DB";
    const std::string table_name = "PSEUDOTABLE";
    swss::DBConnector context(db_name, 0, true);

    swss::Table t(&context, table_name);
    swss::ProducerStateTable pst(&context, table_name);
    swss::ConsumerStateTable cst0(&context, table_name);
    swss::ConsumerStateTable cst1(&context, table_name);

    std::vector<swss::KeyOpFieldsValuesTuple> vkco_input = {
        {"ENTRY0", "", {{"key0", "value0"}, {"key1", "value1"}, {"key2", "value2"}}}};
    std::deque<swss::KeyOpFieldsValuesTuple> vkco_output;
    std::vector<std::string> keys;

    for (auto &kco : vkco_input)
    {
        t.set(kfvKey(kco), kfvFieldsValues(kco));
    }
    print_table_entry(t, "ENTRY0");

    std::cout << "\nAfter first set: " << std::endl;
    vkco_input = {{"ENTRY0", "", {{"key0", "value0"}, {"key1", "value111"}, {"key3", "value3"}}}};
    pst.set(vkco_input);
    cst0.pops(vkco_output);
    print_vkco(vkco_output);
    print_table_entry(t, "ENTRY0");

    std::cout << "\nAfter first del: " << std::endl;
    keys = {"ENTRY0"};
    pst.del(keys);
    cst0.pops(vkco_output);
    print_vkco(vkco_output);
    print_table_entry(t, "ENTRY0");

    std::cout << "\nAfter second set: " << std::endl;
    pst.set(vkco_input);
    cst0.pops(vkco_output);
    print_vkco(vkco_output);
    cst1.pops(vkco_output);
    print_vkco(vkco_output);

    t.getKeys(keys);
    for (auto &key : keys)
    {
        t.del(key);
    }
    pst.clear();
    return 0;
}
