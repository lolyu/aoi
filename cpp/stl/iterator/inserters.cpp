#include <hiredis/hiredis.h>

#include <deque>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

template <typename OutputIterator>
void hgetall(OutputIterator output, redisContext *context, const std::string &key);

/*
back_inserter ---> back_insert_iterator
front_inserter ---> front_insert_iterator
inserter ---> insert_iterator
*/

int main()
{
    auto context = redisConnect("127.0.0.1", 6379);
    if (!context || context->err) {
        if (context) {
            std::cout << "Error: " << context->errstr << std::endl;
        } else {
            std::cout << "Can't allocate Redis context!" << std::endl;
        }
    }

    std::unordered_map<std::string, std::string> fields;
    // EMPLOYEE|ALICE is a hash
    hgetall(std::inserter(fields, fields.end()), context, "EMPLOYEE|ALICE");

    for (const auto &p : fields) {
        std::cout << p.first << ": " << p.second << std::endl;
    }

    std::deque<std::pair<std::string, std::string>> items;
    copy(fields.begin(), fields.end(), std::front_inserter(items));

    for (const auto &p : items) {
        std::cout << p.first << ": " << p.second << std::endl;
    }

    copy(fields.begin(), fields.end(), std::back_inserter(items));
    for (const auto &p : items) {
        std::cout << p.first << ": " << p.second << std::endl;
    }

    return 0;
}

template <typename OutputIterator>
void hgetall(OutputIterator output, redisContext *context, const std::string &key)
{
    redisReply *reply = reinterpret_cast<redisReply *>(redisCommand(context, "HGETALL %s", key.c_str()));
    for (size_t i = 0; i < reply->elements; i += 2) {
        *output = std::make_pair<std::string, std::string>(reply->element[i]->str, reply->element[i + 1]->str);
        ++output;
    }
    freeReplyObject(reply);
}