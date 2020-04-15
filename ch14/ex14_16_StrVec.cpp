#include "StrVec.h"
using namespace std;


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
        alloc.deallocate(elements, cap - elements); // 出錯地方
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
    if(new_cap <= capacity())
        return;
    alloc_n_move(new_cap);
}

void StrVec::resize(size_t count)
{
    resize(count, std::string());
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

bool operator==(const StrVec &lhs, const StrVec &rhs)
{
    return (lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

bool operator!=(const StrVec &lhs, const StrVec &rhs)
{
    return !(lhs == rhs);
}

int main()
{
    StrVec vec;
    vec.reserve(6);
    std::cout << "capacity(reserve to 6): " << vec.capacity() << std::endl;

    vec.reserve(4);
    std::cout << "capacity(reserve to 4): " << vec.capacity() << std::endl;

    vec.push_back("hello");
    vec.push_back("world");

    vec.resize(4);

    for (auto i = vec.begin(); i != vec.end(); ++i)
        std::cout << *i << std::endl;
    std::cout << "-EOF-" << std::endl;

    vec.resize(1);

    for (auto i = vec.begin(); i != vec.end(); ++i)
        std::cout << *i << std::endl;
    std::cout << "-EOF-" << std::endl;

    StrVec vec_list{ "hello", "world", "pezy" };

    for (auto i = vec_list.begin(); i != vec_list.end(); ++i)
        std::cout << *i << " ";
    std::cout << std::endl;

    // Test operator==

    const StrVec const_vec_list{ "hello", "world", "pezy" };
    if (vec_list == const_vec_list)
        for (const auto &str : const_vec_list)
            std::cout << str << " ";
    std::cout << std::endl;
}