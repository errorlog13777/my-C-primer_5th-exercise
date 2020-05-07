#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
using namespace std;

int main(void)
{
    vector<int> vec1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    vector<int> vec2{14, 7, 49, 35};
    int num;
    cin >> num;
    modulus<int> mod;
    auto predicator = [&](int i) { return 0 == mod(num, i); };
    bool is_divisible = any_of(vec2.begin(), vec2.end(), predicator);    // 如果範圍內都符合會返回 true, 否則返回 false

    cout << (is_divisible ? "yes" : "no") << endl;

    return 0;
}