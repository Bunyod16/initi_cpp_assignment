#include <string>
#include <vector>
#include <utility>
#include <chrono>
#include <iostream>
#include <fstream>
#include <set>
#include "rbtc.hpp"

using namespace std;
using namespace chrono;

using write_sequence = vector<string>;

using test_pair = pair<uint64_t, string>;
using modify_sequence = vector<test_pair>;
using read_sequence = vector<test_pair>;

#ifndef SIMPLE_TEST
    #define TEST_TYPE "full"
#else
    #define TEST_TYPE "simple"
#endif

ifstream& operator >> (ifstream& _is, test_pair& _value)
{
    _is >> _value.first;
    _is >> _value.second;

    return _is;
}

template <typename S>
S get_sequence(const string& _file_name)
{
    ifstream infile(_file_name);
    S result;

    typename S::value_type item;

    while (infile >> item)
    {
        result.emplace_back(move(item));
    }

    return result;
}

struct node
{
  string data;
  struct node *next;
};

class storage
{
public:
    void insert(const string& _str)
    {
        _data.insert(_str);
        _data.printTree();
    }

    void erase(uint64_t _index)
    {
        _data.deleteByIndex(_index);
        _data.printTree();
    }

    const string& get(uint64_t _index)
    {
        return (_data.find(_index)->data);
    }

private:
    RedBlackTree _data;
};

int main()
{
    cout << "TEST TYPE: " << TEST_TYPE << endl;
    write_sequence write =  get_sequence<write_sequence>(string("test_files/write_").append(TEST_TYPE).append(".txt"));
    modify_sequence modify =  get_sequence<modify_sequence>(string("test_files/modify_").append(TEST_TYPE).append(".txt"));
    read_sequence read = get_sequence<read_sequence>(string("test_files/read_").append(TEST_TYPE).append(".txt"));

    storage st;

    std::cout << "inserting\n";
    int ct = 0;
    for (const string& item : write)
    {
        ct++;
        st.insert(item);
        std::cout << std::endl << "---- INSERTING: " << ct << endl;
    }

    uint64_t progress = 0;
    uint64_t percent = modify.size() / 100;

    time_point<system_clock> time;
    nanoseconds total_time(0);

    modify_sequence::const_iterator mitr = modify.begin();
    read_sequence::const_iterator ritr = read.begin();
    ct = 0;
    std::cout << "=== TEST BEGIN ====" << std::endl;
    for (; mitr != modify.end() && ritr != read.end(); ++mitr, ++ritr)
    {
        time = system_clock::now();
        std::cout << std::endl << "----- ERASING ------ " << mitr->first << std::endl;
        st.erase(mitr->first);
        // std::cout << "erased in " << (time - system_clock::now()).count() << endl;
        time = system_clock::now();
        std::cout << std::endl << "+++++ INSERTING +++++ " << mitr->second << std::endl;
        st.insert(mitr->second);
        std::cout << std::endl << "===== DONE ====== " << ++ct << std::endl;

        // std::cout << "inserted in " << (time - system_clock::now()).count() << endl;
        const string& str = st.get(ritr->first);
        total_time += system_clock::now() - time;

        if (ritr->second != str)
        {
            cout << "test failed" << endl;
            cout << "expected: " << ritr->second << endl;
            cout << "received: " << str << endl;
            return 1;
        }

        if (++progress % (5 * percent) == 0)
        {
            cout << "time: " << duration_cast<milliseconds>(total_time).count()
                 << "ms progress: " << progress << " / " << modify.size() << "\n";
        }
    }

    return 0;
}
