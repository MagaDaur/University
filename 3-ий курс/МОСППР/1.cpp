#include "S_Point.hpp"
#include <string>

Point get_center(const std::vector<Point>& points, const std::vector<Point>::iterator& exclude)
{
    Point center{};
    for(auto it = points.begin(); it != points.end(); it++)
        if(it != exclude)
            center += *it;

    if(exclude == points.end()) center *= 1.0 / points.size();
    else center *= 1.0 / (points.size() - 1.0);

    return center;
}

double sigma(std::vector<Point>& points)
{
    double center_value = get_center(points, points.end()).calc();
    double sum = 0;

    for(auto& p : points)
        sum += pow(p.calc() - center_value, 2.0);

    return sqrt(sum / points.size());
}

int main()
{
    system("clear");

    int k = 0;

    const int n = 2;
    const double m = 0.75;
    const double eps = LIM_0;

    const double b = 2.0;
    const double y = 0.5;

    const double o1 = m * ( (sqrt(n + 1) - 1) / (n * M_SQRT2) ), o2 = m * ( (sqrt(n + 1) + n - 1) / (n * M_SQRT2) );

    std::vector<Point> points = { BASE_POINT };
    points.push_back({points[0].x + o1, points[0].y + o2});
    points.push_back({points[0].x + o2, points[0].y + o1});

    while(sigma(points) > eps)
    {
        std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) { return a > b; });

        std::cout << std::setfill(' ') << std::setw(20) << std::left << "Iter #" + std::to_string(k++) << std::endl << std::endl;
        std::cout << std::setw(20) << std::right << "x1" << std::setw(20) << "x2" << std::setw(20) << "f(x1, x2)" << std::endl;
        for(const auto point : points)
            std::cout << std::setw(20) << point.x << std::setw(20) << point.y << std::setw(20) << point.calc() << std::endl;
        std::cout << std::setfill('-') << std::setw(82) << " " << std::endl;

        auto best_point = points.end() - 1;
        auto good_point = best_point - 1;
        auto worst_point = points.begin();

        Point center = get_center(points, worst_point);
        Point refracted_point = center * 2.0 - *worst_point;
        Point extended_point = center + b * (refracted_point - center);
        Point shrinked_point = center + y * (*worst_point - center);

        std::cout << center.x << " " << center.y << std::endl;
        std::cout << refracted_point.x << " " << refracted_point.y << " " << refracted_point.calc() << std::endl;
        std::cout << extended_point.x << " " << extended_point.y << " " << extended_point.calc() << std::endl;
        std::cout << shrinked_point.x << " " << shrinked_point.y << " " << shrinked_point.calc() << std::endl;

        if(refracted_point < *worst_point && refracted_point < *best_point)
        {
            if(extended_point < refracted_point)
            {
                points.erase(worst_point);
                points.push_back(extended_point);

                continue;
            }
            else if(*good_point < refracted_point && refracted_point < *worst_point && shrinked_point < refracted_point)
            {   
                points.erase(worst_point);
                points.push_back(shrinked_point);

                continue;
            }
        }

        *worst_point = *best_point + 0.5 * (*worst_point - *best_point);
        *good_point  = *best_point + 0.5 * (*good_point  - *best_point);
    }

    return 0;
}