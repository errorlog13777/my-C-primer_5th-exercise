#include "StrBlob.h"
#include <algorithm>

//==================================================================
//
//		StrBlob - operators
//
//==================================================================

bool operator==(const StrBlob &lhs, const StrBlob &rhs)
{
    return *lhs.data == *rhs.data;
}

bool operator!=(const StrBlob &lhs, const StrBlob &rhs)
{
    return !(lhs == rhs);
}

bool operator< (const StrBlob &lhs, const StrBlob &rhs)
{
    return std::lexicographical_compare(lhs.data->begin(), lhs.data->end(), rhs.data->begin(), rhs.data->end());
}

bool operator> (const StrBlob &lhs, const StrBlob &rhs)
{
    return rhs < lhs;
}

bool operator<=(const StrBlob &lhs, const StrBlob &rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const StrBlob &lhs, const StrBlob &rhs)
{
    return !(lhs < rhs);
}

//================================================================
//
//		StrBlobPtr - operators
//
//================================================================

bool operator==(const StrBlobPtr &lhs, const StrBlobPtr &rhs)
{
    return lhs.curr == rhs.curr;
}

bool operator!=(const StrBlobPtr &lhs, const StrBlobPtr &rhs)
{
    return !(lhs == rhs);
}

bool operator< (const StrBlobPtr &x, const StrBlobPtr &y)
{
    return x.curr < y.curr;
}

bool operator>(const StrBlobPtr &x, const StrBlobPtr &y)
{
    return x.curr > y.curr;
}

bool operator<=(const StrBlobPtr &x, const StrBlobPtr &y)
{
    return x.curr <= y.curr;
}

bool operator>=(const StrBlobPtr &x, const StrBlobPtr &y)
{
    return x.curr >= y.curr;
}

//================================================================
//
//		ConstStrBlobPtr - operators
//
//================================================================

bool operator==(const ConstStrBlobPtr &lhs, const ConstStrBlobPtr &rhs)
{
    return lhs.curr == rhs.curr;
}

bool operator!=(const ConstStrBlobPtr &lhs, const ConstStrBlobPtr &rhs)
{
    return !(lhs == rhs);
}

bool operator< (const ConstStrBlobPtr &lhs, const ConstStrBlobPtr &rhs)
{
    return lhs.curr < rhs.curr;
}

bool operator>(const ConstStrBlobPtr &lhs, const ConstStrBlobPtr &rhs)
{
    return lhs.curr > rhs.curr;
}

bool operator<=(const ConstStrBlobPtr &lhs, const ConstStrBlobPtr &rhs)
{
    return lhs.curr <= rhs.curr;
}

bool operator>=(const ConstStrBlobPtr &lhs, const ConstStrBlobPtr &rhs)
{
    return lhs.curr >= rhs.curr;
}

//==================================================================
//
//		copy assignment operator and move assignment operator.
//
//==================================================================

StrBlob& StrBlob::operator=(const StrBlob &lhs)
{
    data = make_shared<vector<string>>(*lhs.data);
    return *this;
}

StrBlob& StrBlob::operator=(StrBlob &&rhs) NOEXCEPT
{
    if (this != &rhs) {
        data = std::move(rhs.data);
        rhs.data = nullptr;
    }

    return *this;
}

//==================================================================
//
//		members
//
//==================================================================

StrBlobPtr StrBlob::begin()
{
    return StrBlobPtr(*this);
}

StrBlobPtr StrBlob::end()
{
    return StrBlobPtr(*this, data->size());
}

ConstStrBlobPtr StrBlob::cbegin() const
{
    return ConstStrBlobPtr(*this);
}

ConstStrBlobPtr StrBlob::cend() const
{
    return ConstStrBlobPtr(*this, data->size());
}

#include <iostream>

int main()
{
    StrBlob sb1{ "a", "b", "c" };
    StrBlob sb2 = sb1;

    sb2[2] = "b";

    if (sb1 > sb2) {
        for (ConstStrBlobPtr iter = sb1.cbegin(); iter < sb1.cend(); iter.incr())
            std::cout << iter.deref() << " ";
        std::cout << std::endl;
    }

    StrBlobPtr iter(sb2);
    std::cout << iter[2] << std::endl;
}