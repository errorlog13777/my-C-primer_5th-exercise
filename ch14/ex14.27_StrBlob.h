#ifndef STRBLOB_H
#define STRBLOB_H

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <initializer_list>
using std::initializer_list;

#include <memory>
using std::make_shared; using std::shared_ptr;

#include <exception>

#ifndef _MSC_VER
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

class StrBlobPtr;
class ConstStrBlobPtr;

//=================================================================================
//
//		StrBlob - custom vector<string>
//
//=================================================================================

class StrBlob {
    using size_type = vector<string>::size_type;
    friend class ConstStrBlobPtr;
    friend class StrBlobPtr;
    friend bool operator==(const StrBlob&, const StrBlob&);
    friend bool operator!=(const StrBlob&, const StrBlob&);
    friend bool operator< (const StrBlob&, const StrBlob&);
    friend bool operator> (const StrBlob&, const StrBlob&);
    friend bool operator<=(const StrBlob&, const StrBlob&);
    friend bool operator>=(const StrBlob&, const StrBlob&);

public:
    StrBlob() : data(make_shared<vector<string>>()) { }
    StrBlob(initializer_list<string> il) : data(make_shared<vector<string>>(il)) { }

    StrBlob(const StrBlob &sb) : data(make_shared<vector<string>>(*sb.data)) { }
    StrBlob& operator=(const StrBlob&);

    StrBlob(StrBlob &&rhs) NOEXCEPT : data(std::move(rhs.data)) { }
    StrBlob& operator=(StrBlob &&)NOEXCEPT;

    StrBlobPtr begin();
    StrBlobPtr end();

    ConstStrBlobPtr cbegin() const;
    ConstStrBlobPtr cend() const;

    string& operator[](size_t n);
    const string& operator[](size_t n) const;

    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }

    void push_back(const string &t) { data->push_back(t); }
    void push_back(string &&s) { data->push_back(std::move(s)); }

    void pop_back();
    string& front();
    string& back();
    const string& front() const;
    const string& back() const;

private:
    void check(size_type, const string&) const;

    shared_ptr<vector<string>> data;
};

bool operator==(const StrBlob&, const StrBlob&);
bool operator!=(const StrBlob&, const StrBlob&);
bool operator< (const StrBlob&, const StrBlob&);
bool operator> (const StrBlob&, const StrBlob&);
bool operator<=(const StrBlob&, const StrBlob&);
bool operator>=(const StrBlob&, const StrBlob&);

inline void StrBlob::pop_back()
{
    check(0, "pop_back on empty StrBlob");
    data->pop_back();
}

inline string& StrBlob::front()
{
    check(0, "front on empty StrBlob");
    return data->front();
}

inline string& StrBlob::back()
{
    check(0, "back on empty StrBlob");
    return data->back();
}

inline const string& StrBlob::front() const
{
    check(0, "front on empty StrBlob");
    return data->front();
}

inline const string& StrBlob::back() const
{
    check(0, "back on empty StrBlob");
    return data->back();
}

inline void StrBlob::check(size_type i, const string &msg) const
{
    if (i >= data->size()) throw std::out_of_range(msg);
}

inline string& StrBlob::operator[](size_t n)
{
    check(n, "out of range");
    return data->at(n);
}

inline const string& StrBlob::operator[](size_t n) const
{
    check(n, "out of range");
    return data->at(n);
}

//=================================================================================
//
//		StrBlobPtr - custom iterator of StrBlob
//
//=================================================================================

class StrBlobPtr {
    friend bool operator==(const StrBlobPtr&, const StrBlobPtr&);
    friend bool operator!=(const StrBlobPtr&, const StrBlobPtr&);
    friend bool operator< (const StrBlobPtr&, const StrBlobPtr&);
    friend bool operator> (const StrBlobPtr&, const StrBlobPtr&);
    friend bool operator<=(const StrBlobPtr&, const StrBlobPtr&);
    friend bool operator>=(const StrBlobPtr&, const StrBlobPtr&);

public:
    StrBlobPtr() : curr(0) { }
    StrBlobPtr(StrBlob &s, size_t sz = 0) : wptr(s.data), curr(sz) { }

    string& deref() const;
    StrBlobPtr& incr();

    string& operator[](size_t n);
    const string& operator[](size_t n) const;

    StrBlobPtr &operator++();
    StrBlobPtr &operator--();

    StrBlobPtr operator++(int);
    StrBlobPtr operator--(int);

    StrBlobPtr operator+=(size_t);
    StrBlobPtr operator-=(size_t);

    StrBlobPtr operator+(size_t) const;
    StrBlobPtr operator-(size_t) const;

private:
    shared_ptr<vector<string>> check(size_t, const string&) const;

    std::weak_ptr<vector<string>> wptr;
    size_t curr;
};

bool operator==(const StrBlobPtr&, const StrBlobPtr&);
bool operator!=(const StrBlobPtr&, const StrBlobPtr&);
bool operator< (const StrBlobPtr&, const StrBlobPtr&);
bool operator> (const StrBlobPtr&, const StrBlobPtr&);
bool operator<=(const StrBlobPtr&, const StrBlobPtr&);
bool operator>=(const StrBlobPtr&, const StrBlobPtr&);

inline string& StrBlobPtr::deref() const
{
    auto p = check(curr, "dereference past end");
    return (*p)[curr];
}

inline StrBlobPtr& StrBlobPtr::incr()
{
    check(curr, "increment past end of StrBlobPtr");
    ++curr;
    return *this;
}

inline shared_ptr<vector<string>> StrBlobPtr::check(size_t i, const string &msg) const
{
    auto ret = wptr.lock();
    if (!ret) throw std::runtime_error("unbound StrBlobPtr");
    if (i >= ret->size()) throw std::out_of_range(msg);
    return ret;
}

inline string& StrBlobPtr::operator[](size_t n)
{
    auto p = check(n, "dereference out of range.");
    return (*p)[n];
}

inline const string& StrBlobPtr::operator[](size_t n) const
{
    auto p = check(n, "dereference out of range.");
    return (*p)[n];
}

inline StrBlobPtr& StrBlobPtr::operator++()
{
    // 如果 curr 已經指向了容器的尾後位置，則無法遞增它
    check(curr, "increment past end of StrBlobPtr");
    ++curr;    // 將 curr 在當前狀態下向前移動一個元素
    return *this;
}

inline StrBlobPtr& StrBlobPtr::operator--()
{
    // 如果 curr 是 0，則繼續遞減它將產生一個無效下標
    --curr;    // 將 curr 在當前狀態下向後移動一個元素
    check(curr, "decrement past begin of StrBlobPtr");
    return *this;
}

inline StrBlobPtr StrBlobPtr::operator++(int)
{
    // 此處無須檢查有效性，調用前置遞增運算時才需要檢查
    StrBlobPtr ret = *this;    // 紀錄當前值
    ++*this;                   // 向前移動一個元素，前置++需要檢查遞增的有效性
    return ret;                // 返回之前紀錄的狀態
}

inline StrBlobPtr StrBlobPtr::operator--(int)
{
    // 此處無須檢查有效性，調用前置遞增運算時才需要檢查
    StrBlobPtr ret = *this;    // 紀錄當前值
    --*this;                   // 向後移動一個元素，前置--需檢查遞減的有效性
    return ret;                // 返回之前紀錄的狀態
}

inline StrBlobPtr StrBlobPtr::operator+=(size_t n)
{
    curr += n;
    check(curr, "increment past end of StrBlobPtr");
    return *this;
}

inline StrBlobPtr StrBlobPtr::operator-=(size_t n)
{
    curr -= n;
    check(curr, "decrement past begin of StrBlobPtr");
    return *this;
}

inline StrBlobPtr StrBlobPtr::operator+(size_t n) const
{
    StrBlobPtr ret = *this;
    ret += n;
    return ret;
}

inline StrBlobPtr StrBlobPtr::operator-(size_t n) const
{
    StrBlobPtr ret = *this;
    ret -= n;
    return ret;
}

//=================================================================================
//
//		ConstStrBlobPtr - custom const_iterator of StrBlob
//
//=================================================================================

class ConstStrBlobPtr {
    friend bool operator==(const ConstStrBlobPtr&, const ConstStrBlobPtr&);
    friend bool operator!=(const ConstStrBlobPtr&, const ConstStrBlobPtr&);
    friend bool operator< (const ConstStrBlobPtr&, const ConstStrBlobPtr&);
    friend bool operator> (const ConstStrBlobPtr&, const ConstStrBlobPtr&);
    friend bool operator<=(const ConstStrBlobPtr&, const ConstStrBlobPtr&);
    friend bool operator>=(const ConstStrBlobPtr&, const ConstStrBlobPtr&);

public:
    ConstStrBlobPtr() : curr(0) { }
    ConstStrBlobPtr(const StrBlob &s, size_t sz = 0) : wptr(s.data), curr(sz) { }

    const string& deref() const;
    ConstStrBlobPtr& incr();

    ConstStrBlobPtr& operator++();
    ConstStrBlobPtr& operator--();
    ConstStrBlobPtr operator++(int);
    ConstStrBlobPtr operator--(int);
    ConstStrBlobPtr& operator+=(size_t);
    ConstStrBlobPtr& operator-=(size_t);
    ConstStrBlobPtr operator+(size_t) const;
    ConstStrBlobPtr operator-(size_t) const;

    const string& operator[](size_t n) const;

private:
    std::shared_ptr<vector<string>> check(size_t, const string&) const;

    std::weak_ptr<vector<string>> wptr;
    size_t curr;
};

bool operator==(const ConstStrBlobPtr&, const ConstStrBlobPtr&);
bool operator!=(const ConstStrBlobPtr&, const ConstStrBlobPtr&);
bool operator< (const ConstStrBlobPtr&, const ConstStrBlobPtr&);
bool operator> (const ConstStrBlobPtr&, const ConstStrBlobPtr&);
bool operator<=(const ConstStrBlobPtr&, const ConstStrBlobPtr&);
bool operator>=(const ConstStrBlobPtr&, const ConstStrBlobPtr&);

inline const string& ConstStrBlobPtr::deref() const
{
    auto p = check(curr, "dereference past end");
    return (*p)[curr];
}

inline ConstStrBlobPtr& ConstStrBlobPtr::incr()
{
    check(curr, "increment past end of StrBlobPtr");
    ++curr;
    return *this;
}

inline std::shared_ptr<vector<string>> ConstStrBlobPtr::check(size_t i, const string &msg) const
{
    auto ret = wptr.lock();
    if (!ret) throw std::runtime_error("unbound StrBlobPtr");
    if (i >= ret->size()) throw std::out_of_range(msg);
    return ret;
}

inline const string& ConstStrBlobPtr::operator[](size_t n) const
{
    auto p = check(n, "dereference out of range.");
    return (*p)[n];
}

inline ConstStrBlobPtr& ConstStrBlobPtr::operator++()
{
    check(curr, "increment past end of ConstStrBlobPtr");
    ++curr;
    return *this;
}

inline ConstStrBlobPtr& ConstStrBlobPtr::operator--()
{
    --curr;
    check(curr, "decrement past begin of ConstStrBlobPtr");
    return *this;
}

inline ConstStrBlobPtr ConstStrBlobPtr::operator++(int)
{
    ConstStrBlobPtr ret = *this;
    ++*this;
    return ret;
}

inline ConstStrBlobPtr ConstStrBlobPtr::operator--(int)
{
    ConstStrBlobPtr ret = *this;
    --*this;
    return ret;
}

inline ConstStrBlobPtr& ConstStrBlobPtr::operator+=(size_t n)
{
    curr += n;
    check(curr, "increment past end of ConstStrBlobPtr");
    return *this;
}

inline ConstStrBlobPtr& ConstStrBlobPtr::operator-=(size_t n)
{
    curr -= n;
    check(curr, "decrement past begin of ConstrBlobPtr");
    return *this;
}

inline ConstStrBlobPtr ConstStrBlobPtr::operator+(size_t n) const
{
    ConstStrBlobPtr ret = *this;
    ret += n;
    return ret;
}

inline ConstStrBlobPtr ConstStrBlobPtr::operator-(size_t n) const
{
    ConstStrBlobPtr ret = *this;
    ret -= n;
    return ret;
}

#endif //CP5_STRBLOB_H_