#include <iostream>
#include <swss/notificationproducer.h>
#include <swss/notificationconsumer.h>

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
    const std::string channel_name = "PSEUDOCHANNEL";
    const std::string db_name = "APPL_DB";
    swss::DBConnector context(db_name, 0, true);
    swss::NotificationProducer np(&context, channel_name);
    swss::NotificationConsumer nc(&context, channel_name);

    std::vector<swss::FieldValueTuple> values = {{"key0", "value0"}, {"key1", "value1"}};
    std::deque<swss::KeyOpFieldsValuesTuple> vkco;

    np.send("hello", "world", values);
    np.send("hello", "world", values);

    nc.readData();
    nc.pops(vkco);
    print_vkco(vkco);
    return 0;
}
