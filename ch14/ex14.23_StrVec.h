#ifndef STRVEC_H
#define STRVEC_H

#include <iostream>
#include <string>
#include <initializer_list>
#include <memory>
#include <utility>
using namespace std;

class StrVec{
    friend bool operator==(const StrVec &, const StrVec &);
    friend bool operator!=(const StrVec &, const StrVec &);
public:
    StrVec() : elements(nullptr), first_free(nullptr), cap(nullptr) {}  // allocator 成員進行默認初始化
    StrVec(initializer_list<string> il);
    StrVec &operator=(std::initializer_list<std::string>);
    StrVec(const StrVec &);             // copy constructor
    StrVec &operator=(const StrVec &);  // copy assignment operator
    StrVec(StrVec &&) noexcept;         // move constructor
    StrVec &operator=(StrVec &&) noexcept; // move assignment operator
    ~StrVec();                          // destructor

    void push_back(const string&);      // copy element(s)
    size_t size() const { return first_free - elements; }   // 兩個指針相減會得到兩個指針相差的距離(必為正)
    size_t capacity() const { return cap - elements; }
    string *begin() const { return elements; }
    string *end() const { return first_free; }

    std::string& at(size_t pos) { return *(elements + pos); }
    const std::string& at(size_t pos) const { return *(elements + pos); }
    
    void reserve(size_t new_cap);
    void resize(size_t count);
    void resize(size_t count, const string&);

private:
    // 工具函數, 被 copy constructor,  assignment operator, destructor 所使用
    pair<string *, string *> alloc_n_copy(const string *, const string *);
    void free();        // 銷毀元素並釋放內存
    // 被添加元素的函數所使用
    inline void chk_n_alloc() { 
        if(size() == capacity())
            reallocate();
    }
    void reallocate();  // 獲得更多內存並拷貝已有元素
    void alloc_n_move(size_t new_cap);  // 給定參數分配空間
    void range_initialize(const string *first, const string *last);

private:
    string *elements;   // 指向數組第一個元素的指針
    string *first_free; // 指向數組第一個空閒元素的指針
    string *cap;        // 指向數組尾後位置的指針
    static std::allocator<std::string> alloc; // 分配元素
};
allocator<string> StrVec::alloc;

bool operator==(const StrVec &, const StrVec &);
bool operator!=(const StrVec &, const StrVec &);

#endif