#include "Sales_data.h"

Sales_data::Sales_data(std::istream &is)
{
    is >> bookNo >> units_sold >> revenue;
}

Sales_data& Sales_data::operator+=(const Sales_data &sd)
{
    units_sold += sd.units_sold;
    revenue += sd.revenue;
    return *this;
}

Sales_data& Sales_data::operator-=(const Sales_data &sd)
{
    units_sold -= sd.units_sold;
    revenue -= sd.revenue;
    return *this;
}

std::istream& operator>>(std::istream &is, Sales_data &sd)
{
    double price = 0.0;
    is >> sd.bookNo >> sd.units_sold >> price;
    if(is)
        sd.revenue = price * sd.units_sold;
    else
        sd = Sales_data();
    return is;
}

std::ostream& operator<<(std::ostream &os, const Sales_data &sd)
{
    os << sd.bookNo << " " << sd.units_sold << " " << sd.revenue;
    return os;
}

Sales_data operator+(const Sales_data &lhs, const Sales_data &rhs)
{
    Sales_data sum = lhs;   // temporary
    sum += rhs;
    return sum;
}

Sales_data operator-(const Sales_data &lhs, const Sales_data &rhs)
{
    Sales_data sum = lhs;   // temporary
    sum -= rhs;
    return sum;
}

int main()
{
  Sales_data s1("book1", 150, 10);
  Sales_data s2("book1", 200, 20);

  std::cout << s1 << std::endl;

  // Assignment
  s1 = s1 + s2;
  std::cout << s1 << std::endl;

  // Compound assignment
  s1 += s2;
  std::cout << s1 << std::endl;

  // Compound substraction
  s1 -= s2;
  std::cout << s1 << std::endl;

  // Substraction
  s1 = s1 - s2;
  std::cout << s1 << std::endl;
}