#include "Book.h"

std::istream &operator>>(std::istream& is, Book &bk)
{
    return is >> bk.no_ >> bk.name_ >> bk.author_ >> bk.pubdate_;
}

std::ostream& operator<<(std::ostream&os, const Book&bk)
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

bool operator<(const Book &lhs, const Book &rhs)
{
    return lhs.no_ < rhs.no_;
}

bool operator>(const Book &lhs, const Book &rhs)
{
    return lhs.no_ > rhs.no_;
}

Book& Book::operator+=(const Book &rhs)
{
    if(*this == rhs)
        this->number_ += rhs.number_;
    return *this;
}

Book operator+(const Book &lhs, const Book &rhs)
{
    Book book = lhs;
    book += rhs;
    return book;
}

int main()
{
    Book cp5_1(12345, "CP5", "Lippmen", "2012", 2);
    Book cp5_2(12345, "CP5", "Lippmen", "2012", 4);

    std::cout << cp5_1 + cp5_2 << std::endl;
}