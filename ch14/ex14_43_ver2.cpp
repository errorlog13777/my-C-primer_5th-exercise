#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;

int main(void)
{
    vector<int> v{3, 4, 5}; // 60
    int input;
    cin >> input;

    bool is_divisible = any_of(v.cbegin(), v.cend(), [&](int i) { return 0 == modulus<int>()(input, i); });
    cout << (is_divisible ? "Yes" : "No") << endl;

    return 0;
}