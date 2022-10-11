#include <iostream>
#include <swss/consumertable.h>
#include <swss/producertable.h>

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

int main()
{
    const std::string db_name = "APPL_DB";
    const std::string table_name = "PSEUDOTABLE";
    swss::DBConnector context(db_name, 0, true);

    swss::Table t(&context, table_name);
    swss::ProducerTable pt(&context, table_name);
    swss::ConsumerTable ct(&context, table_name);

    std::vector<swss::FieldValueTuple> fvs;

    std::deque<swss::KeyOpFieldsValuesTuple> vkco;

    fvs = {{"key0", "value0"}, {"key1", "value1"}};
    pt.set("ENTRY0", fvs);
    fvs = {{"key1", "value11"}, {"key2", "value2"}};
    pt.set("ENTRY0", fvs);
    ct.pops(vkco);
    print_vkco(vkco);

    std::vector<std::string> keys;
    t.getKeys(keys);
    for (auto &key : keys)
    {
        t.del(key);
    }

    return 0;
}
