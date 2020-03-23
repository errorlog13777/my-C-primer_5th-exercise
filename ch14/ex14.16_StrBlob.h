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

/*

    StrBlob - custion vector<string>

*/

class StrBlob{
    using size_type = vector<string>::size_type;
    friend class StrBlobPtr;
    friend class ConstStrBlobPtr;
    friend bool operator==(const StrBlob &, const StrBlob &);
    friend bool operator!=(const StrBlob &, const StrBlob &);
public:
    StrBlob() : data(std::make_shared<vector<string>>()) {}
    StrBlob(std::initializer_list<string> il) : data(std::make_shared<vector<string>>(il)) {}

    StrBlob(const StrBlob &sb) : data(std::make_shared<vector<string>>(*sb.data)) {}
    StrBlob &operator=(const StrBlob &);

    StrBlob(StrBlob &&rhs) : data(std::move(rhs.data)) {}
    StrBlob &operator=(StrBlob &&) noexcept;

    StrBlobPtr begin();
    StrBlobPtr end();

    ConstStrBlobPtr cbegin() const;  // should add const
    ConstStrBlobPtr cend() const;

    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }

    void push_back(const string &t) { data->push_back(t); }
    void push_back(string &&s) { data->push_back(std::move(s)); }

    inline void pop_back(void){
        check(0, "pop_back on empty StrBlob");
        data->pop_back();
    }

    inline string& front(){
        check(0, "front on empty StrBlob");
        return data->front();
    }

    inline string& back(){
        check(0, "back on empty StrBlob");
        return data->back();
    }

    inline const string& front() const{
        check(0, "front on empty StrBlob");
        return data->front();
    }

    inline const string& back() const{
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

bool operator==(const StrBlob &, const StrBlob &);
bool operator!=(const StrBlob &, const StrBlob &);

/*

    StrBlobPtr - custom iterator of StrBlob

*/

class StrBlobPtr{
    friend bool operator==(const StrBlobPtr &, const StrBlobPtr &);
    friend bool operator!=(const StrBlobPtr &, const StrBlobPtr &);
public:
    StrBlobPtr(void) : curr(0) {}
    StrBlobPtr(StrBlob &a, size_t sz = 0) : wptr(a.data), curr(sz) {}

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

bool operator==(const StrBlobPtr &, const StrBlobPtr &);
bool operator!=(const StrBlobPtr &, const StrBlobPtr &);

/*

    ConstStrBlobPtr - custom const_iterator of StrBlob

*/

class ConstStrBlobPtr{
    friend bool operator==(const ConstStrBlobPtr &, const ConstStrBlobPtr &);
    friend bool operator!=(const ConstStrBlobPtr &, const ConstStrBlobPtr &);

public:
    ConstStrBlobPtr() : curr(0) {}
    ConstStrBlobPtr(const StrBlob &s, size_t sz = 0) : wptr(s.data), curr(sz) {}

    inline const string& deref() const
    {
        auto p = check(curr, "dereference past end");
        return (*p)[curr];
    }
    
    inline ConstStrBlobPtr& incr(){
        check(curr, "increment past end of ConstStrBlobPtr");
        ++curr;
        return *this;
    }

private:
    std::shared_ptr<vector<string>> check(size_t, const string &) const;
    std::weak_ptr<vector<string>> wptr;
    size_t curr;
};

std::shared_ptr<vector<string>> ConstStrBlobPtr::check(size_t i, const string& msg) const{
        std::shared_ptr<vector<string>> ret = wptr.lock();
        if(!ret)
            throw std::runtime_error("unbound ConstStrBlobPtr");
        if(i >= ret->size())
            throw std::out_of_range(msg);
        return ret;
    }

bool operator==(const ConstStrBlobPtr &, const ConstStrBlobPtr &);
bool operator!=(const ConstStrBlobPtr &, const ConstStrBlobPtr &);

#endif