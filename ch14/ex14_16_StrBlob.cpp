#include "StrBlob.h"


// StrBlob
bool operator==(const StrBlob &lhs, const StrBlob &rhs)
{
    return *lhs.data == *rhs.data;
}
bool operator!=(const StrBlob &lhs, const StrBlob &rhs)
{
    return !(lhs == rhs);
}

// StrBlobPtr
bool operator==(const StrBlobPtr &lhs, const StrBlobPtr &rhs)
{
    return lhs.curr == rhs.curr;
}
bool operator!=(const StrBlobPtr &lhs, const StrBlobPtr &rhs)
{
    return !(lhs == rhs);
}

// ConstStrBlob
bool operator==(const ConstStrBlobPtr &lhs, const ConstStrBlobPtr &rhs)
{
    return lhs.curr == rhs.curr;
}
bool operator!=(const ConstStrBlobPtr &lhs, const ConstStrBlobPtr &rhs)
{
    return !(lhs == rhs);
}


// member

StrBlobPtr StrBlob::begin()
{
    return StrBlobPtr(*this);
}

StrBlobPtr StrBlob::end()
{
    return StrBlobPtr(*this, data->size());
}

ConstStrBlobPtr StrBlob::cbegin() const // should add const
{
    return ConstStrBlobPtr(*this);
}

ConstStrBlobPtr StrBlob::cend() const
{
    return ConstStrBlobPtr(*this, data->size());
}