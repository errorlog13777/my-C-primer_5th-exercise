#include "book.h"

// member

Book &Book::operator=(const Book &bk)
{
    no_ = bk.no_;
    name_ = bk.name_;
    author_ = bk.author_;
    pubdate_ = bk.pubdate_;

    return *this;
}

Book &Book::operator=(Book &&bk) noexcept
{
    no_ = bk.no_;
    name_ = std::move(bk.name_);
    author_ = std::move(bk.author_);
    pubdate_ = std::move(bk.pubdate_);

    return *this;
}

Book &Book::operator=(const unsigned &no)
{
    no_ = no;

    return *this;
}

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
    return !(lhs == rhs);
}

bool operator<(const Book &lhs, const Book &rhs)
{
    return lhs.no_ < rhs.no_;
}

bool operator>(const Book &lhs, const Book &rhs)
{
    return lhs.no_ > rhs.no_;
}
