#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

string str_to_lower(const string &s)
{
    string res;
    transform(s.cbegin(), s.cend(), back_inserter(res), [](char c) { return tolower(c); });
    return res;
}

int main()
{

    vector<string> words = {"vector", "string", "set", "string", "sEt", "string", "Set", "vector"};
    set<string> wordset;
    transform(words.begin(), words.end(), inserter(wordset, wordset.begin()), str_to_lower);

    map<string, int> wordmap;
    for (const string &word : words) {
        ++wordmap[str_to_lower(word)];
    }

    for (const auto [word, count] : wordmap) {
        cout << word << ": " << count << endl;
    }
    return 0;
}