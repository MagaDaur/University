#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>

#define LIM_0 sqrt(__DBL_EPSILON__)

typedef std::vector<std::vector<double>> matrix;

double f(double x1, double x2)
{
    return 2.0 * x1 * x1 - 2.0 * x1 * x2 + 3.0 * x2 * x2 + x1 - 3.0 * x2;
}

struct Point
{
    double x, y;

    Point() : x(0), y(0) {};
    Point(double x, double y) : x(x), y(y) {};

    double calc() const { *(const_cast<double*>(&val)) = f(x, y); return val; }

    friend Point operator-(const Point& p, const Point& other) 
    {
        return Point(p.x - other.x, p.y - other.y);
    }
    friend Point operator+(const Point& p, const Point& other)
    {
        return Point(p.x + other.x, p.y + other.y);
    }
    
    friend Point operator*(const Point& p, double mult)
    {
        return Point(p.x * mult, p.y * mult);
    }
    friend Point operator*(double mult, const Point& p)
    {
        return Point(p.x * mult, p.y * mult);
    }

    void operator-=(const Point& other)
    {
        x -= other.x;
        y -= other.y;
    }
    void operator+=(const Point& other)
    {
        x += other.x;
        y += other.y;
    }
    
    void operator*=(double mult)
    {
        x *= mult;
        y *= mult;
    }

    friend bool operator<(const Point& a, const Point& b)
    {
        return a.calc() < b.calc();
    }

    friend bool operator>(const Point& a, const Point& b)
    {
        return a.calc() > b.calc();
    }
private:
    double val; // for debug only!
};