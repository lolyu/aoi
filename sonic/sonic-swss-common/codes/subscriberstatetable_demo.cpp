#include <iostream>
#include <string>
#include <swss/select.h>
#include <swss/subscriberstatetable.h>

void print_vkco(const std::deque<swss::KeyOpFieldsValuesTuple> &vkco, int index)
{
    std::cout << "----------------------------" << std::endl;
    std::cout << index << "th pop..." << std::endl;
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
    std::cout << "----------------------------" << std::endl;
}

int main()
{
    const std::string dbname = "CONFIG_DB";
    const std::string table_name = "PSEUDOTABLE";
    swss::DBConnector context(dbname, 0, true);

    swss::Table t(&context, table_name);

    std::vector<swss::FieldValueTuple> fvs = {
        {"key0", "value0"},
        {"key1", "value1"}};
    t.set("ENTRY0", fvs);

    std::shared_ptr<swss::Select> selector = std::make_shared<swss::Select>();
    swss::SubscriberStateTable st(&context, table_name);
    std::deque<swss::KeyOpFieldsValuesTuple> vkco;
    swss::Selectable *sel;

    selector->addSelectable(&st);

    selector->select(&sel, 10);
    if (sel)
    {
        st.pops(vkco);
        print_vkco(vkco, 0);
    }

    t.set("ENTRY0", fvs); // mutiple set trigger multiple events
    selector->select(&sel, 10);
    if (sel)
    {
        st.pops(vkco);
        print_vkco(vkco, 1);
    }

    t.set("ENTRY1", fvs);
    t.set("ENTRY2", fvs);
    t.set("ENTRY3", fvs);
    t.del("ENTRY0");
    selector->select(&sel, 10);
    if (sel)
    {
        st.pops(vkco);
        print_vkco(vkco, 2);
    }

    t.del("ENTRY0"); // multiple del only trigger one event
    selector->select(&sel, 10);
    if (sel)
    {
        st.pops(vkco);
        print_vkco(vkco, 3);
    }

    t.del("ENTRY1");
    t.del("ENTRY2");
    selector->select(&sel, 10);
    selector->select(&sel, 10);
    if (sel)
    {
        st.pops(vkco);
        print_vkco(vkco, 4);
    }

    t.del("ENTRY3");
    selector->select(&sel, 10);
    std::cout << bool(sel) << std::endl; // first select returns st as there is one key change
    selector->select(&sel, 10);
    std::cout << bool(sel) << std::endl; // second select will not return st

    return 0;
}
