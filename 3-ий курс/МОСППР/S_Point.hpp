#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>

#define LIM_0 sqrtf(FLT_EPSILON)
#define BASE_POINT Point(10, 10)

struct Point;

double f(double x1, double x2)
{
    //5
    //return 4 * x1 * x1 + 4 * x1 * x2 + 6 * x2 * x2 - 17 * x1;

    //6
    return 2.0 * x1 * x1 - 2.0 * x1 * x2 + 3.0 * x2 * x2 + x1 - 3.0 * x2;

    //11
    //return x1 * x1 + 2 * x2 * x2 + exp(x1 * x1 + x2 * x2) - x1 + 2 * x2;

    //8
    //return x1 * x1 - 2 * x1 * x2 + 6 * x2 * x2 + x1 - x2;
}

namespace Matrix
{
    typedef std::vector<std::vector<double>> type;

    const type Unit = { {1, 0}, {0, 1} };

    double det(const type&);

    type multiply(const type&, const type&);
    type transpose(const type&);
    type cofactor(const type&);
    type inverse(const type&);

    type gesse(const Point&);
}

namespace df
{
    //Vector of partial derivatives ( gradient )
    Point first_partial(const Point& p);

    //Vector of partial second derivatives
    Point second_partial(const Point& p);

    //Mixed second derivative
    double second_mixed(const Point& p);
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

    // void operator-=(const Matrix::type& other)
    // {
    //     x -= other[0][0];
    //     y -= other[1][0];
    // }

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

//Vector of partial derivatives ( gradient )
Point df::first_partial(const Point& p)
{
    double dx = ( f(p.x + LIM_0, p.y) - f(p.x, p.y) ) / LIM_0;
    double dy = ( f(p.x, p.y + LIM_0) - f(p.x, p.y) ) / LIM_0;

    return {dx, dy};
}

//Vector of partial second derivatives
Point df::second_partial(const Point& p)
{
    double u1_x = f(p.x + LIM_0, p.y);
    double u3_x = f(p.x - LIM_0, p.y);

    double temp = u1_x + u3_x;

    double u1_y = f(p.x, p.y + LIM_0);
    double u3_y = f(p.x, p.y - LIM_0);

    double u2 = f(p.x, p.y);

    double ddx = (u1_x - 2.0 * u2 + u3_x) / (LIM_0 * LIM_0);
    double ddy = (u1_y - 2.0 * u2 + u3_y) / (LIM_0 * LIM_0);

    return {ddx, ddy};
}

//Mixed second derivative
double df::second_mixed(const Point& p)
{
    double u1 = f(p.x, p.y);
    double u2 = f(p.x - LIM_0, p.y);
    double u3 = f(p.x, p.y - LIM_0);
    double u4 = f(p.x - LIM_0, p.y - LIM_0);

    return (u1 - u2 - u3 + u4) / (LIM_0 * LIM_0);
}

Matrix::type Matrix::multiply(const type& a, const type& b)
{
    type res(a.size());
    for(int i = 0; i < a.size(); i++)
        res[i].resize(b.size());

    for(int i = 0; i < a.size(); i++)
    {
        for(int j = 0; j < b[0].size(); j++)
        {
            res[i][j] = 0;
            for(int k = 0; k < a[0].size(); k++)
            {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return res;
}

Matrix::type Matrix::gesse(const Point& p)
{
    auto vec_ddf = df::second_partial(p);
    auto mixed_ddf = df::second_mixed(p);

    return 
    {
        {vec_ddf.x, mixed_ddf},
        {mixed_ddf, vec_ddf.y},
    };
}

double Matrix::det(const type& m)
{
    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

Matrix::type Matrix::transpose(const type& m)
{
    return 
    {
        {m[0][0], m[1][0]},
        {m[0][1], m[1][1]},
    };
}

Matrix::type Matrix::cofactor(const type& m)
{
    return
    {
        {m[1][1], -m[1][0]},
        {-m[0][1], m[0][0]},
    };
}

Matrix::type Matrix::inverse(const type& m)
{
    auto d = det(m);
    auto c = cofactor(m);

    return
    {
        {c[0][0] / d, c[0][1] / d},
        {c[1][0] / d, c[1][1] / d},
    };
}

double norm(const Point& p)
{
    Point vec_grad = df::first_partial(p);
    return sqrt(vec_grad.x * vec_grad.x + vec_grad.y * vec_grad.y);
}