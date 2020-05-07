#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;

void a()
{
    vector<int> v{1, 10, 144, 1024, 10255, 45346, 2342, 4, 3245};
    // int count = count_if(v.cbegin(), v.cend(), bind(greater<int>(), placeholders::_1, 1024));
    int count = count_if(v.cbegin(), v.cend(), [](int i) { return greater<int>()(i, 1024); });

    cout << count << endl;
}

void b()
{
    // not_equal_to<Type>
    vector<string> v{"pooh", "pooh", "kevin", "pooh", "pooh"};
    vector<string>::const_iterator found = find_if(v.cbegin(), v.cend(), [](const string &str) { return not_equal_to<string>()(str, "pooh"); });
    cout << *found << endl;
}

void c()
{
    // multiplies<Type>
    vector<int> v{1, 10, 144, 1024, 10255, 45346, 2342, 4, 3245};
    transform(v.begin(), v.end(), v.begin(), [](int i) { return multiplies<int>()(i, 2); });

    for(int i : v)
        cout << i << " ";
    cout << endl;
}

int main(void)
{
    a();
    b();
    c();

    return 0;
}