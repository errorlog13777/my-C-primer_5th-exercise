#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

int main(void)
{
    // (a)
    vector<int> ivec{1, 10, 100, 1000, 1024, 10000, 1055, 7414, 1487};
    int count = count_if(ivec.cbegin(), ivec.cend(), bind(greater<int>(), placeholders::_1, 1024));    // placeholders::_1 代表 greater<int>() 的第一個參數，其所接受的是 count_if 所傳遞的參數
    cout << count << endl;

    // (b)
    vector<string> svec{"pooh", "pooh", "pooh", "Kevin", "pooh"};
    vector<string>::const_iterator found = find_if(svec.cbegin(), svec.cend(), bind(not_equal_to<string>(), placeholders::_1, "pooh"));
    cout << *found << endl;

    // (c)
    transform(ivec.begin(), ivec.end(), ivec.begin(), bind(multiplies<int>(), placeholders::_1, 2));
    for(int i : ivec)
        cout << i << " ";
    cout << endl;

    return 0;
}