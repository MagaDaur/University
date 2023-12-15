#include <vector>
#include <iostream>

double f(double, double);

struct Vector2D
{
    double x{};
    double y{};

    double calc() { return f(x, y); }

    void operator=(const Vector2D& other)
    {
        x = other.x;
        y = other.y;
    }

    friend std::ostream& operator<<(std::ostream& to, const Vector2D& from)
    {
        return to << "( " << from.x << " ; " << from.y << " )";
    }
};

struct Velocity : Vector2D
{

};

struct Point : Vector2D
{
    Vector2D local_min{};
    Velocity velocity{};

    void operator+=(const Velocity& other)
    {
        x += other.x;
        y += other.y;
    }
}; Point* global_min;

double rn() { return double(rand()) / RAND_MAX; }

double rn(double min, double max) {return min + (max - min) * rn(); }

double f(double x, double y)
{
    return pow(cos(x), 2) * sin(y);
}

std::vector<Velocity> GenerateVelocity(int n)
{
    std::vector<Velocity> velocity(n);
    for(int i = 0; i < n; i++)
        velocity[i] = { rn(-10, 10), rn(-10, 10) };
    return velocity;
}

std::vector<Point> GeneratePoints(int n)
{
    auto velocities = GenerateVelocity(n);

    std::vector<Point> points(n);
    for(int i = 0; i < n; i++)
    {
        auto& point = points[i];
        point = { rn(-5, 5), rn(-5, 5) };
 
        point.local_min = point;

        point.velocity = velocities[i];
    }

    return points;
}

Velocity CalcVelocity(double W, double C, Point& point, const Point* best_point)
{
    double R1 = rn(), R2 = rn();

    double x = W * point.velocity.x + C * R1 * (point.local_min.x - point.x) + C * R2 * (best_point->x - point.x);
    double y = W * point.velocity.y + C * R1 * (point.local_min.y - point.y) + C * R2 * (best_point->y - point.y);
    return {x, y};
}

int main()
{
    srand(time(0));

    const double W = 0.6;
    const auto C = 2;

    const int n = 10;
    const int k = 1000;

    auto points = GeneratePoints(n);
    global_min = &points[0];

    for(int i = 0; i < k; i++)
    {
        std::cout << "Итерация: #" << i << std::endl;

        for(auto& point : points)
        {
            point.velocity = CalcVelocity(W, C, point, global_min);
            point += point.velocity;

            double fxy = f(point.x, point.y);

            if(fxy <= point.local_min.calc())
            {
                point.local_min = point;
            }

            if(fxy <= global_min->calc())
            {
                global_min = &point;
            }

            std::cout << "\tЧастица: " << point << std::endl;
            std::cout << "\tCкорость: " << point.velocity << std::endl;
            std::cout << "\tЗначение функции: " << point.calc() << std::endl;
            std::cout << "\tЛокальный минимум: " << point.local_min << std::endl << std::endl; 
        }

        std::cout << "Глобальный минимум: f" << *global_min << " = " << global_min->calc() << std::endl << std::endl;;
    }

    return 0;
}