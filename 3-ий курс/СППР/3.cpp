#include <vector>
#include <iostream>

struct Point
{
    double x;
    double y;
};

double rn() { return double(rand()) / RAND_MAX; }

double rn(double min, double max) {return min + (max - min) * rn(); }

double f(double x, double y)
{
    return pow(x * x + 6 - 11, 2) + pow(x + y * y - 7, 2);
}

std::vector<Point> GeneratePoints(int n)
{
    std::vector<Point> points(n);
    for(int i = 0; i < n; i++)
        points[i] = { rn(-5, 5), rn(-5, 5) };
    return points;
}

std::vector<Point> GenerateVelocity(int n)
{
    std::vector<Point> velocity(n);
    for(int i = 0; i < n; i++)
        velocity[i] = { rn(-10, 10), rn(-10, 10) };
    return velocity;
}

std::vector<Point> GenerateR(int k)
{
    std::vector<Point> r(k);
    for(int i = 0; i < k; i++)
        r[i] = { rn(), rn() };
    return r;
}

int main()
{
    srand(time(0));

    const double W = 0.6;
    const double C = 2;

    const int n = 4;
    const int k = 3;

    auto points = GeneratePoints(n);
    auto r = GenerateR(k);

    for(const auto& point : points)
    {
        std::cout << point.x << " " << point.y << " " << f(point.x, point.y) << std::endl;
    }
}