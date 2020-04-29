#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

void elimDups(vector<string> &words)
{
    // 先排序後"刪除連續重複"的元素，最後將刪除元素後多出來的空間移除
    sort(words.begin(), words.end());
    auto end_unique = unique(words.begin(), words.end());
    words.erase(end_unique, words.end());   // words.resize(std::distance(words.begin(), end_unique))
}

string make_plural(size_t ctr, const string& word, const string &ending)
{
    return (ctr > 1) ? word + ending : word;
}

void biggies(vector<string> &words, vector<string>::size_type sz)
{
    elimDups(words);    // 將 words 按字典序排序，刪除重複單詞
    // 按長度排序，長度相同的單詞維持字典序
    stable_sort(words.begin(), words.end(), [](const string &a, const string &b) { return a.size() < b.size(); });
    // 獲取一個迭代器, 指向第一個滿足 size() >= sz 的元素
    auto wc = find_if(words.begin(), words.end(), [sz](const string &a) { return a.size() >= sz; });

    // 計算滿足 size >= sz 的元素的數目
    auto count = words.end() - wc;
    cout << count << " " << make_plural(count, "word", "s")
         << " of length " << sz << " or longer" << endl;

    // 打印長度大於等於給定值的單詞，每個單詞後面接一個空格
    for_each(wc, words.end(), [](const string &s) { cout << s << " "; });
    cout << endl;
}

int main(void)
{
    vector<string> vec{ "fox", "jumps", "over", "quick", "red", "red", "slow", "the", "turtle" };
    biggies(vec, 4);
}