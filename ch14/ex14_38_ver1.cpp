#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

class in_range{
public:
    in_range(const size_t lower, const size_t upper) : _lower(lower), _upper(upper) { }

    bool operator()(const string &str)
    {
        return _lower <= str.size() && _upper >= str.size();
    }

    size_t get_lower()
    {
        return _lower;
    }

    size_t get_upper()
    {
        return _upper;
    }

private:
    size_t _lower;
    size_t _upper;
};

int main(void)      // ex_14_38
{
    ifstream ifs("tt.txt");
    in_range range(1, 10);
    map<size_t, size_t> count_table;    // first = 1 ~ 10, second = count(s)

    for (size_t i = range.get_lower(); i <= range.get_upper(); ++i){
        count_table[i] = 0;
    }

    for (string word; ifs >> word;){
        if(range(word))
        ++count_table[word.size()];
    }

    for(pair<size_t, size_t> p : count_table){
        cout << "size " << p.first << ": " << p.second << endl;
    }

    return 0;
}

int main2(void)     // ex_14_39
{
    ifstream ifs("tt.txt");
    in_range range(1, 9);
    size_t ten_above = 0;
    map<size_t, size_t> count_table;    // first = 1 ~ 10, second = count(s)

    for (size_t i = range.get_lower(); i <= range.get_upper(); ++i){
        count_table[i] = 0;
    }

    for (string word; ifs >> word;){
        if(word.size() > 10)
            ++ten_above;
        else 
            ++count_table[word.size()];
    }

    for(pair<size_t, size_t> p : count_table){
        cout << "size " << p.first << ": " << p.second << endl;
    }
    cout << "above 10: " << ten_above;

    return 0;
}