#ifndef StrBlob_h
#define StrBlob_h

#include <vector>
#include <string>
#include <initializer_list>
#include <memory>
#include <stdexcept>
using std::string;
using std::vector;

class StrBlobPtr;    // 用於指向 StrBlob 的指針類，可以確保合法訪問 StrBlob 內的元素
class ConstStrBlobPtr;

class StrBlob{
public:
    using size_type = vector<string>::size_type;
    friend class StrBlobPtr;
    friend class ConstStrBlobPtr;

    StrBlobPtr begin();
    StrBlobPtr end();

    ConstStrBlobPtr begin() const;  // should add const
    ConstStrBlobPtr end() const;

    StrBlob() : data(std::make_shared<vector<string>>()) {}
    StrBlob(std::initializer_list<string> il) : data(std::make_shared<vector<string>>(il)) {}

    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }

    void push_back(const string &t) { data->push_back(t); }
    void push_back(string &&t) { data->push_back(std::move(t)); }
    void pop_back(void){
        check(0, "pop_back on empty StrBlob");
        data->pop_back();
    }

    string& front(){
        check(0, "front on empty StrBlob");
        return data->front();
    }

    string& back(){
        check(0, "back on empty StrBlob");
        return data->back();
    }

    const string& front() const{
        check(0, "front on empty StrBlob");
        return data->front();
    }

    const string& back() const{
        check(0, "back on empty StrBlob");
        return data->back();
    }

private:
    void check(size_type i, const string &msg) const{
        if(i >= data->size())
            throw std::out_of_range(msg);
    }

    std::shared_ptr<vector<string>> data;
};

class StrBlobPtr{
public:
    StrBlobPtr(void) : curr(0) {}
    StrBlobPtr(StrBlob &a, size_t sz = 0) : wptr(a.data), curr(sz) {}
    bool operator!=(const StrBlobPtr &p) { return p.curr != curr; }
    string& deref(void) const{
        auto p = check(curr, "dereference past end");
        return (*p)[curr];
    }
    StrBlobPtr& incr(void){
        check(curr, "increment past end of StrBlobPtr");
        ++curr;
        return *this;
    }

private:
    std::shared_ptr<vector<string>> check(size_t i, const string& msg) const{
        std::shared_ptr<vector<string>> ret = wptr.lock();
        if(!ret)
            throw std::runtime_error("unbound StrBlobPtr");
        if(i >= ret->size())
            throw std::out_of_range(msg);
        return ret;
    }
    std::weak_ptr<vector<string>> wptr;
    size_t curr;
};

StrBlobPtr StrBlob::begin()
{
    return StrBlobPtr(*this);
}

StrBlobPtr StrBlob::end()
{
    return StrBlobPtr(*this, data->size());
}

class ConstStrBlobPtr{
public:
    ConstStrBlobPtr() : curr(0) {}
    // 構造函數 curr 沒有讀入 sz 的值
    ConstStrBlobPtr(const StrBlob &a, size_t sz = 0) : wptr(a.data), curr(sz) {}

    std::shared_ptr<vector<string>> check(size_t i, const string& msg) const{
        std::shared_ptr<vector<string>> ret = wptr.lock();
        if(!ret)
            throw std::runtime_error("unbound ConstStrBlobPtr");
        if(i >= ret->size())
            throw std::out_of_range(msg);
        return ret;
    }

    bool operator!=(const ConstStrBlobPtr &p) { return p.curr != curr; }

    string& deref(void) const{
        auto p = check(curr, "dereference past end");
        return (*p)[curr];
    }
    
    ConstStrBlobPtr& incr(void){
        check(curr, "increment past end of ConstStrBlobPtr");
        ++curr;
        return *this;
    }

private:
    std::weak_ptr<vector<string>> wptr;
    size_t curr;
};

ConstStrBlobPtr StrBlob::begin() const // should add const
{
    return ConstStrBlobPtr(*this);
}

ConstStrBlobPtr StrBlob::end() const
{
    return ConstStrBlobPtr(*this, data->size());
}

#endif