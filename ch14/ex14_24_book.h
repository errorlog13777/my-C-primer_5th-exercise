#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>

class Book{
    friend std::istream &operator>>(std::istream &, Book &);
    friend std::ostream &operator<<(std::ostream &, Book &);
    friend bool operator==(const Book &, const Book &);
    friend bool operator!=(const Book &, const Book &);
    friend bool operator<(const Book &, const Book &);
    friend bool operator>(const Book &, const Book &);
    friend Book operator+(const Book &, const Book &);
public:
    Book() = default;
    Book(unsigned no, std::string name, std::string author, std::string pubdate) : no_(no), name_(name), author_(author), pubdate_(pubdate) {}
    Book(std::istream &in) { in >> *this; }

    Book &operator=(const Book &);
    Book &operator=(Book &&) noexcept;
    Book &operator=(const unsigned &);

private:
    unsigned no_;
    std::string name_;
    std::string author_;
    std::string pubdate_;
};

std::istream &operator>>(std::istream &, Book &);
std::ostream &operator<<(std::ostream &, Book &);
bool operator==(const Book &, const Book &);
bool operator!=(const Book &, const Book &);
bool operator<(const Book &, const Book &);
bool operator>(const Book &, const Book &);

#endif