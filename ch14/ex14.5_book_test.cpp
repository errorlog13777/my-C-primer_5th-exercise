#include "book.cpp"

int main(void)
{
    Book book1(123, "Kevin's love", "Kevin Zhang", "1987");
    Book book2(123, "Kevin's love", "Kevin Zhang", "1987");

    if(book1 == book2)
        std::cout << book1 << std::endl;

    return 0;
}