// StrVec.cpp
#include <iostream>
#include <string>
#include <initializer_list>
#include <memory>
#include <utility>
#include <algorithm>
using namespace std;

class StrVec{
public:
    StrVec() : elements(nullptr), first_free(nullptr), cap(nullptr) {}  // allocator 成員進行默認初始化
    StrVec(initializer_list<string> il);
    StrVec(const StrVec &);             // copy constructor
    StrVec &operator=(const StrVec &);  // copy assignment operator
    StrVec(StrVec &&rs)noexcept;                // move constructor
    StrVec operator=(StrVec &&)noexcept;        // move assignment operator
    ~StrVec();                          // destructor

    
    
    size_t size() const { return first_free - elements; }   // 兩個指針相減會得到兩個指針相差的距離(必為正)
    size_t capacity() const { return cap - elements; }
    string *begin() const { return elements; }
    string *end() const { return first_free; }

    void push_back(const string&);      // copy element(s)
    void reserve(size_t new_cap);
    void resize(size_t count, const string& s = " ");
    
    std::string& at(size_t pos) { return *(elements + pos); }
    const std::string& at(size_t pos) const { return *(elements + pos); }

private:
    static allocator<string> alloc; // 分配元素
    // 被添加元素的函數所使用
    inline void chk_n_alloc() { 
        if(size() == capacity())
            reallocate();
    }
    // 工具函數, 被 copy constructor,  assignment operator, destructor 所使用
    pair<string *, string *> alloc_n_copy(const string *, const string *);
    void range_initialize(const string *first, const string *last);
    void alloc_n_move(size_t new_cap);  // 給定參數分配空間

    void free();        // 銷毀元素並釋放內存
    void reallocate();  // 獲得更多內存並拷貝已有元素
    string *elements;   // 指向數組第一個元素的指針
    string *first_free; // 指向數組第一個空閒元素的指針
    string *cap;        // 指向數組尾後位置的指針
};

void StrVec::push_back(const string& s)
{
    chk_n_alloc();    // 確保有空間容納新元素
    // 在 first_free 指向的元素中構造 s 的副本
    alloc.construct(first_free++, s);
}

pair<string*, string*> StrVec::alloc_n_copy(const string *b, const string *e)
{
    // 分配空間保存給定範圍中的元素
    auto data = alloc.allocate(e - b);
    // 初始化並返回一個 pair, 該 pair 由 data 和 uninitialized_copy 的返回值構成
    return {data, uninitialized_copy(b, e, data)};
}

void StrVec::free()
{
    // 不能傳遞給 deallocate 一個空指針，如果 elements 為 0，函數什麼也不做
    if(elements){
        // 逆序銷毀舊元素
        for(auto p = first_free; p != elements;)
            alloc.destroy(--p);
        alloc.deallocate(elements, cap - elements);
    }
    
    if(elements){
        for_each(elements, first_free, [this](string &rhs){ alloc.destroy(&rhs); });
    }
    
}

StrVec::StrVec(const StrVec &s)
{
    // 調用 alloc_n_copy 分配空間以容納與 s 中一樣多的元素
    auto newdata = alloc_n_copy(s.begin(), s.end());
    elements = newdata.first;
    first_free = cap = newdata.second;    // newdata.second 賦值給 cap, cap 賦值給 first_free
}

StrVec::~StrVec() { free(); }

StrVec &StrVec::operator=(const StrVec &rhs)
{
    // 調用 alloc_n_copy 分配內存，大小與 rhs 中元素占用空間一樣多
    auto data = alloc_n_copy(rhs.begin(), rhs.end());
    free();
    elements = data.first;
    first_free = cap = data.second;
    return *this;
}

StrVec::StrVec(StrVec &&rs) noexcept : elements(rs.elements), first_free(rs.first_free), cap(rs.cap) // 移動 string 類的 move constructor
{
    // leave rs in a state in which it is safe to run the destructor
    rs.elements = rs.first_free = rs.cap = nullptr;
}

StrVec StrVec::operator=(StrVec &&rs) noexcept
{
    if(this != &rs){
        free();
        elements = rs.elements;     // 使用 string 類的 move constructor
        first_free = rs.first_free; // 使用 string 類的 move constructor
        cap = rs.cap;               // 使用 string 類的 move constructor
        
        // leave rs in a state in which it is safe to run the destructor
        rs.elements = rs.first_free = rs.cap = nullptr;
    }

    return *this;
}

void StrVec::reallocate()
{
    // 我們分配當前大小兩倍的內存空間
    auto newcapacity = size() ? 2 * size() : 1;
    // 分配新內存
    auto newdata = alloc.allocate(newcapacity);
    // 將數據從舊內存移動到新內存
    auto dest = newdata;    // 指向新數組中下一個空閒位置
    auto elem = elements;   // 指向舊數組中下一個元素
    
    for(size_t i = 0; i != size(); ++i){
        alloc.construct(dest++, std::move(*elem++));
    }
    free();    // 一旦我們移動完元素就釋放舊內存空間
    // 更新我們的數據結構，執行新元素
    elements = newdata;
    first_free = dest;
    cap = elements + newcapacity;    // 尾後元素 = 第一個元素 + 新容量
}

void StrVec::alloc_n_move(size_t new_cap)
{
    // 因為 reallocate 沒有支援給定容量，所以寫另外一個可給定值的函數
    auto newdata = alloc.allocate(new_cap);
    auto dest = newdata;
    auto elem = elements;
    for (size_t i = 0; i != size(); ++i)
        alloc.construct(dest++, std::move(*elem++));
    free();
    elements = newdata;
    first_free = dest;
    cap = elements + new_cap;
}

void StrVec::reserve(size_t new_cap)
{
    if(capacity() < new_cap)
        return;
    alloc_n_move(new_cap);
}

void StrVec::resize(size_t count, const string& s)
{
    /*
    if(capacity() < new_cap)  {
        alloc_n_move(new_cap * 2);
    }
    for (size_t i = size(); i != new_cap; ++i){   // 不論有無重新分配空間，最後都需要將 size() == N 
        this->push_back(s);
    }
    */
    if(count > size()){
        if(count > capacity())
            reserve(count * 2);
        for (size_t i = size(); i != count; ++i)
            alloc.construct(first_free++, s);
    }
    else if(count < size()){
        while(first_free != elements + count)
            alloc.destroy(--first_free);
    }
}


void StrVec::range_initialize(const string *first, const string *last)
{
    auto newdata = alloc_n_copy(first, last);
    elements = newdata.first;
    first_free = cap = newdata.second;
}

StrVec::StrVec(initializer_list<string> il)
{
    range_initialize(il.begin(), il.end());
}