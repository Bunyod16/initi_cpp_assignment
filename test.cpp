#include <string>
#include <vector>
#include <utility>
#include <chrono>
#include <iostream>
#include <fstream>
#include <set>
#include "map.hpp"

using namespace std;
using namespace chrono;

using write_sequence = vector<string>;

using test_pair = pair<uint64_t, string>;
using modify_sequence = vector<test_pair>;
using read_sequence = vector<test_pair>;

#define TEST_TYPE full
#define VALUE(string) #string
#define TO_LITERAL(string) VALUE(string)

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
        _data.insert(ft::make_pair(_str, 0));

    }

    void erase(uint64_t _index)
    {
        _data.eraseByIndex(_index);
    }

    const string& get(uint64_t _index)
    {
        
        return (_data.findByIndex(_index)->first);
    }

private:
    ft::map<string, short>    _data;
};

int main()
{
    cout << "TEST TYPE: " << TO_LITERAL(TEST_TYPE) << endl;
    write_sequence write =  get_sequence<write_sequence>(string("test_files/write_").append(TO_LITERAL(TEST_TYPE)).append(".txt"));
    modify_sequence modify =  get_sequence<modify_sequence>(string("test_files/modify_").append(TO_LITERAL(TEST_TYPE)).append(".txt"));
    read_sequence read = get_sequence<read_sequence>(string("test_files/read_").append(TO_LITERAL(TEST_TYPE)).append(".txt"));

    storage st;

    std::cout << "inserting\n";
    int ct = 0;
    for (const string& item : write)
    {
        ct++;
        st.insert(item);
        std::cout << "inserting " << ct << endl;
    }
    std::cout << "done inserting\n";

    uint64_t progress = 0;
    uint64_t percent = modify.size() / 100;

    time_point<system_clock> time;
    nanoseconds total_time(0);

    modify_sequence::const_iterator mitr = modify.begin();
    read_sequence::const_iterator ritr = read.begin();
    ct = 0;
    for (; mitr != modify.end() && ritr != read.end(); ++mitr, ++ritr)
    {
        time = system_clock::now();
        st.erase(mitr->first);
        std::cout << "erased in " << (time - system_clock::now()).count() << endl;
        time = system_clock::now();
        st.insert(mitr->second);
        std::cout << "inserted in " << (time - system_clock::now()).count() << endl;
        const string& str = st.get(ritr->first);
        total_time += system_clock::now() - time;

        if (ritr->second != str)
        {
            cout << "test failed" << endl;
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
