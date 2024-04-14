#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

struct Employee
{
    std::vector<std::string> lunch_partners;
    std::string id;
    std::mutex m;

    Employee(const std::string &id) : id(id) {}
    std::string partners() const
    {
        std::string ret = "Employee " + id + " has lunch partners: ";
        for (int count{}; const auto &lp : lunch_partners)
        {
            ret += (count++ > 0 ? ", " : "") + lp;
        }
        return ret;
    }
};

void send_mail(Employee &, Employee &)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void assign_lunch_partner(Employee &e1, Employee &e2)
{
    static std::mutex io_mutex;
    {
        std::lock_guard<std::mutex> lk(io_mutex);
        std::cout << e1.id << " and " << e2.id << " are waiting for locks" << std::endl;
    }
    {
        std::scoped_lock lock(e1.m, e2.m);
        {
            std::lock_guard<std::mutex> lk(io_mutex);
            std::cout << e1.id << " and " << e2.id << " got locks" << std::endl;
        }
        e1.lunch_partners.push_back(e2.id);
        e2.lunch_partners.push_back(e1.id);
    }

    send_mail(e1, e2);
    send_mail(e2, e1);
}

int main()
{
    Employee alice("Alice"), bob("Bob"), christina("Christina"), dave("Dave");

    std::vector<std::thread> threads;
    threads.emplace_back(assign_lunch_partner, std::ref(alice), std::ref(bob));
    threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(bob));
    threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(alice));
    threads.emplace_back(assign_lunch_partner, std::ref(dave), std::ref(bob));

    for (auto &thread : threads)
    {
        thread.join();
    }

    std::cout << alice.partners() << std::endl;
    std::cout << bob.partners() << std::endl;
    std::cout << christina.partners() << std::endl;
    std::cout << dave.partners() << std::endl;

    return 0;
}
