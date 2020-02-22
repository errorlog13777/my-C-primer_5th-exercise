#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class HasPtr{
public:
    friend void swap(HasPtr &, HasPtr &);
    friend bool operator<(const HasPtr &, const HasPtr &);

    HasPtr(const std::string &s = std::string()):
        ps(new std::string(s)), i(0) { }
    
    HasPtr(const HasPtr &p):
        ps(new std::string(*p.ps)), i(p.i) { }

    HasPtr& operator=(HasPtr rhs){ // 為何這裡的參數不能用引用類型？
        this->swap(rhs);
        return *this;
    };
    
    ~HasPtr() { delete ps; }

    inline void swap(HasPtr& rhs)   // for class internal
    {
        using std::swap;
        swap(this->ps, rhs.ps);
        swap(this->i, rhs.i);
        cout << "call swap(HasPtr &rhs)" << endl;
    }

    inline void print(void){
        cout << *this->ps << " " << this->i << endl;
    }

    void show() const{
        cout << *this->ps << endl;
    }

private:
    std::string *ps;
    int i;
};

bool operator<(const HasPtr &lhs, const HasPtr &rhs){
        return *lhs.ps < *rhs.ps;
    }

void swap(HasPtr &lhs, HasPtr& rhs) // for class external
{
    lhs.swap(rhs);
}

int main(void)
{
    HasPtr A("Jason"), B("Kevin"), C("Mumu"), D("Hiyajo");
    vector<HasPtr> vec{A, B, C, D};

/*
    vec.push_back(A);   // push_back using copy constructor
    vec.push_back(B);
    vec.push_back(C);
    vec.push_back(D);
*/
    //sort(vec.begin(), vec.end());

    for(auto const& elem : vec)
        elem.show();

    return 0;
}