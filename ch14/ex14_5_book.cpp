#include "book.h"

std::istream &operator>>(std::istream& is, Book &bk)
{
    return is >> bk.no_ >> bk.name_ >> bk.author_ >> bk.pubdate_;
}

std::ostream& operator<<(std::ostream&os, Book&bk)
{
    return os << bk.no_ << " " << bk.name_ << " " << bk.author_ << " " << bk.pubdate_;
}

bool operator==(const Book &lhs, const Book &rhs)
{
    return lhs.no_ == rhs.no_;
}

bool operator!=(const Book &lhs, const Book &rhs)
{
    return lhs.no_ != rhs.no_;
}