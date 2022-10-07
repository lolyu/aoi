#include <iostream>
#include <string>
#include <algorithm>
#include <swss/table.h>

int main()
{
    std::string dbname = "CONFIG_DB";
    std::string table_name = "PORT";
    swss::DBConnector context(dbname, 0, true);
    swss::Table port_table(&context, table_name);

    std::vector<std::string> keys;
    port_table.getKeys(keys);

    std::cout << "Keys: " << std::endl;
    for (auto &key : keys)
    {
        std::cout << key << std::endl;
    }

    std::vector<swss::KeyOpFieldsValuesTuple> content0;
    swss::TableDump content1;
    port_table.getContent(content0);
    port_table.dump(content1);

    size_t show_size = content0.size() > 5 ? 5 : content0.size();
    for (int i = 0; i < show_size; ++i)
    {
        std::cout << kfvKey(content0[i]) << std::endl;
        std::cout << kfvOp(content0[i]) << std::endl;
        auto fields = kfvFieldsValues(content0[i]);
        for (auto &field : fields)
        {
            std::cout << '\t' << fvField(field) << ": " << fvValue(field) << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << content1.size() << std::endl;
    int i = 0;
    for (auto it = content1.cbegin(); it != content1.cend() && i < show_size; ++it, ++i)
    {
        std::cout << it->first << std::endl;
        for (auto &p : it->second)
        {
            std::cout << '\t' << p.first << ": " << p.second << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}
