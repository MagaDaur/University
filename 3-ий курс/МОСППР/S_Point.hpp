#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>

float f(float, float);

struct Point
{
    float x, y;

    Point() : x(0), y(0) {};
    Point(float x, float y) : x(x), y(y) {};

    float calc() const { *(const_cast<float*>(&val)) = f(x, y); return val; }

    friend Point operator-(const Point& p, const Point& other) 
    {
        return Point(p.x - other.x, p.y - other.y);
    }
    friend Point operator+(const Point& p, const Point& other)
    {
        return Point(p.x + other.x, p.y + other.y);
    }
    
    friend Point operator*(const Point& p, float mult)
    {
        return Point(p.x * mult, p.y * mult);
    }
    friend Point operator*(float mult, const Point& p)
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
    
    void operator*=(float mult)
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
    float val; // for debug only!
};