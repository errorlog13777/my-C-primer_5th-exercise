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