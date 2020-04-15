#include <iostream>
#include <cstring>
#include <memory>
#include <initializer_list>
#include <utility>
#include <algorithm>
using namespace std;

class String{
public:
    friend ostream &operator<<(ostream &, const String &);
    friend bool operator==(const String &, const String &);
    friend bool operator!=(const String &, const String &);

    String() : String("") {}
    String(const char *s);
    String(const String &rhs);
    String& operator=(const String &rhs);
    ~String();

    char& operator[](size_t n) { return elements[n]; }
    const char &operator[](size_t n) const { return elements[n]; }

    const char *c_str() const{ return elements; }
    inline size_t size(void) const { return cap - elements; }
    inline size_t length(void) const { return cap - elements - 1; } 
/*
    ostream& operator<<(ostream &output, const String &str)
    {
        output << str.elements;
        return output;
    }
*/
    void print(void);
    char *begin() const { return elements; }
    char *end() const { return cap; }

private:
    // pair<char *, char *> alloc_n_copy(const char *, const char *);
    pair<char *, char *> alloc_n_copy(const char *c);
    pair<char *, char *> alloc_n_copy(const char *b, const char *e);
    void range_initializer(const char *first, const char *last);
    void free();

private:
    char *elements;     // 第一個元素
    char *cap;          // 尾後
    allocator<char> ac;     // 如果類型是內置類型，則會出現 undefine reference 錯誤。
    // static allocator<string> acc;
};

pair<char*, char*> String::alloc_n_copy(const char *b, const char* e)
{
    auto str = ac.allocate(e - b);
    return {str, uninitialized_copy(b, e, str)};
}

pair<char*, char*> String::alloc_n_copy(const char *c)
{
    auto data = ac.allocate(strlen(c));
    return {data, uninitialized_copy(c, c + strlen(c), data)}; // c + strlen(c) = end
}

void String::range_initializer(const char *first, const char *last)
{
    auto newstr = alloc_n_copy(first, last);
    elements = newstr.first;
    cap = newstr.second;
}

String::String(const char *s)
{
    /*
    auto data = ac.allocate(strlen(c));
    // uninitialized_copy_n(c, c + strlen(c), data);
    elements = data;
    cap = elements + strlen(c);
    ac.construct(data++, *c++);*/

    char *b = const_cast<char*>(s);
    char* e = const_cast<char*>(s + strlen(s) + 1);
    auto newstr = ac.allocate(e - b);
    cap = uninitialized_copy(b, e, newstr);
    elements = newstr;

   //  cout << "*cap-1 = "<< *(cap-1) << " elements = " << *elements << endl;
    /*
    char *s1 = const_cast<char *>(s);
    while(*s1)
        ++s1;
    range_initializer(s, ++s1);*/
    /*
    auto newdata = alloc_n_copy(c);
    elements = newdata.first;
    cap = newdata.second;
*/
}
/*
String::String(const String& rhs)
{
    range_initializer(rhs.elements, rhs.cap);
    cout << "copy constructor" << endl;
}*/

String::String(const String &str)
{
    char *p;
    elements = p = ac.allocate(str.cap - str.elements);
    cap = uninitialized_copy(str.elements, str.cap, p); // begin, last, address
    cout << "copy constructor" << endl;
}

String& String::operator=(const String& rhs)
{
    
    free();
    // elements = rhs.elements; // this is error, must be like value not pointer.
    auto newstr = alloc_n_copy(rhs.elements, rhs.cap); // new space
    elements = newstr.first;
    cap = newstr.second;
    cout << "copy-assignment operator" << endl;

    return *this;
}

void String::free()
{
    /*
    if(elements){
        for (char *p = cap; p != elements;)
            ac.destroy(--p);
        ac.deallocate(elements, cap - elements);
    }
*/
    if(elements){
        for_each(elements, cap, [this](char &c) { ac.destroy(&c); });
        ac.deallocate(elements, cap - elements);
    }
}

String::~String()
{
    free();
}

void String::print(void)
{
    for (const char *it = elements; it != cap; ++it)
        cout << *it;

    for (size_t i = 0; i != size(); ++i)
        cout << elements[i];
}

ostream& operator<<(ostream& os, const String& s)
{
    /*
    for (const char *c = s.elements; c != s.cap; ++c){
        os << *c;
    }

    for (size_t i = 0; i != s.size(); ++i){
        os << s.elements[i];
    }
    */
   
    for(const char *c = s.begin(); c != s.end(); ++c){
        os << *c;
    }

    return os;
}

bool operator==(const String &lhs, const String &rhs)
{
    return (lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

bool operator!=(const String &lhs, const String &rhs)
{
    return !(lhs == rhs);
}

// Test reference to http://coolshell.cn/articles/10478.html

void foo(String x)
{
    std::cout << x.c_str() << std::endl;
}

void bar(const String& x)
{
    std::cout << x.c_str() << std::endl;
}

String baz()
{
    String ret("world");
    return ret;
}

#include <vector>
int main()
{
    /*
    char text[] = "world";

    String s0;
    String s1("hello");
    String s2(s0);
    String s3 = s1;
    String s4(text);
    s2 = s1;

    foo(s1);
    bar(s1);
    foo("temporary");
    bar("temporary");
    String s5 = baz();

    std::vector<String> svec;
    svec.reserve(8);
    svec.push_back(s0);
    svec.push_back(s1);
    svec.push_back(s2);
    svec.push_back(s3);
    svec.push_back(s4);
    svec.push_back(s5);
    svec.push_back(baz());
    svec.push_back("good job");

    for (const auto &s : svec) {
        std::cout << s.c_str() << std::endl;
    }
    */
   vector<String> v;
   v.push_back("Jason");
   v.push_back("zestar");

   String J("Kevin"), K("Kervin");
   if(J == K)
       cout << "true" << endl;
   else
       cout << "false" << endl;

   // cout << J;
   String s5("Jason");
   std::cout << s5[2] << std::endl;
}