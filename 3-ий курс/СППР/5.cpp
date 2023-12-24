#include <iostream>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <iomanip>

double f(double x, double y)
{
    return pow(x * x + y - 11, 2) + pow(x + y * y - 7, 2);
}

struct Vector2D
{
    double x;
    double y;

    void operator=(const Vector2D& other)
    {
        x = other.x;
        y = other.y;
    }

    friend std::ostream& operator<<(std::ostream& to, const Vector2D& from)
    {
        return to << "( " << from.x << " ; " << from.y << " )";
    }

    double Distance(const Vector2D& other) const
    {
        return sqrt(pow(other.x - x, 2) + pow(other.y - y, 2));
    }
};

struct Point : Vector2D
{
    double Calculate() const { return f(x, y); };

    friend std::ostream& operator<<(std::ostream& to, const Point& from)
    {
        return to << "f" << Vector2D(from) << " = " << from.Calculate();
    }
};

double rn() { return double(rand()) / RAND_MAX; }

double rn(const double& min, const double& max) {return min + (max - min) * rn(); }
double rn(const int& min, const int& max) { return min + double(max - min) * rn(); }

struct Bee : Point
{
    using Vector2D::operator=;

    Bee() {};
    Bee(const Point& coords) : Point(coords) {};

    std::vector<Bee> CreateScanners(int, double) const;

    static std::vector<Bee> CreateColony(int, double, const Point& offset = {0, 0});
};

void sort(std::vector<Bee>& colony)
{
    std::sort(colony.begin(), colony.end(), [](const Bee& a, const Bee& b)
    {
        return a.Calculate() < b.Calculate();
    });
}

void merge(std::vector<Bee>& a, const std::vector<Bee>& b)
{
    a.insert(a.end(), b.begin(), b.end());
}

// void merge_intersections(std::vector<Bee>& colony, double r)
// {
//     for(int i = 0; i < colony.size(); i++)
//     {
//         for(int j = i + 1; j < colony.size(); j++)
//         {
//             double distance = colony[i].Distance(colony[j]);
//             if(distance <= r)
//             {
//                 if(colony[i].Calculate() <= colony[j].Calculate())
//                 {

//                 }
//                 else
//                 {
                    
//                 }
//             }
//         }
//     }
// }

std::vector<Bee> Bee::CreateScanners(int n, double r) const
{
    return CreateColony(n, r, {x, y});
}

std::vector<Bee> Bee::CreateColony(int n, double r, const Point& offset)
{
    std::vector<Bee> colony(n);
    for(int i = 0; i < n; i++)
        colony[i] = {.x = offset.x + rn(-r, r), .y = offset.y + rn(-r, r)};
    return colony;
}

int main()
{
    srand(time(0));
    system("clear");

    const int S = 100;
    const double R = 3;

    constexpr int N = 10;
    constexpr int M = 30;

    const int K = 20;

    auto colony = Bee::CreateColony(S, 50);

    for(int i = 0; i < K; i++)
    {
        sort(colony);

        std::cout << "Итерация №" << i << ":" << std::endl;
        for(int j = 0; j < N; j++) std::cout << colony[j] << std::endl;
        std::cout << std::endl;

        std::vector<Bee> best(N);
        std::vector<Bee> possible(M);

        std::vector<Bee> new_colony;

        for(int j = 0; j < N; j++)
            best[j] = colony[j];

        for(int j = 0; j < M; j++)
            possible[j] = colony[N + j + 1];


        for(const auto& bee : best)
            merge(new_colony, bee.CreateScanners(N, R));

        for(const auto& bee : possible)
            merge(new_colony, bee.CreateScanners(M, R));

        colony = new_colony;
    }

    return 0;
}