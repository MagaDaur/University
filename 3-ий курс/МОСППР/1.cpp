#include "S_Point.hpp"

float f(float x, float y)
{
    return 2 * x * x - 2 * x * y + 3 * y * y + x - 3 * y;
}

Point get_center(const std::vector<Point>& points, const std::vector<Point>::iterator& exclude)
{
    Point center{};
    for(auto it = points.begin(); it != points.end(); it++)
        if(it != exclude)
            center += *it;

    if(exclude == points.end()) center *= 1.f / points.size();
    else center *= 1.f / (points.size() - 1.f);

    return center;
}

float sigma(std::vector<Point>& points)
{
    float center_value = get_center(points, points.end()).calc();
    float sum = 0;

    for(auto& p : points)
        sum += powf(p.calc() - center_value, 2.f);

    return sqrtf(sum / points.size());
}

int main()
{
    system("clear");

    int k = 0;

    const int n = 2;
    const float m = 0.75f;
    const float eps = 0.00001f;

    const float b = 1.85f;
    const float y = 0.1f;

    const float o1 = m * ( (sqrtf(n + 1) - 1) / (n * M_SQRT2) );
    const float o2 = m * ( (sqrtf(n + 1) + n - 1) / (n * M_SQRT2) );

    std::vector<Point> points;
    points.push_back({ 0, 0 });

    Point& base_point = points[0];

    Point point_1{base_point.x + o1, base_point.y + o2};
    Point point_2{base_point.x + o2, base_point.y + o1};

    points.push_back(point_1);
    points.push_back(point_2);

    while(sigma(points) > eps)
    {
        std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) { return a > b; });

        std::cout << std::setfill(' ') << "Iter #" << k++ << std::endl << std::endl;
        std::cout << std::setw(20) << "x1" << std::setw(20) << "x2" << std::setw(20) << "f(x1, x2)" << std::endl;
        for(const auto point : points)
            std::cout << std::setw(20) << point.x << std::setw(20) << point.y << std::setw(20) << point.calc() << std::endl;
        std::cout << std::setfill('-') << std::setw(82) << " " << std::endl;

        auto best_point = points.end() - 1;
        auto good_point = best_point - 1;
        auto worst_point = points.begin();

        Point center = get_center(points, worst_point);
        Point refracted_point = center * 2 - *worst_point;
        Point extended_point = center + b * (refracted_point - center);
        Point shrinked_point = center + y * (*worst_point - center);

        if(refracted_point < *worst_point)
        {
            if(refracted_point < *best_point)
            {
                if(extended_point < refracted_point)
                {
                    points.erase(worst_point);
                    points.push_back(extended_point);

                    continue;
                }
                else if(*good_point < refracted_point && refracted_point < *worst_point)
                {   
                    if(shrinked_point < refracted_point)
                    {
                        points.erase(worst_point);
                        points.push_back(shrinked_point);

                        continue;
                    }
                }
            }
        }

        *worst_point = *best_point + 0.5f * (*worst_point - *best_point);
        *good_point  = *best_point + 0.5f * (*good_point  - *best_point);
    }

    return 0;
}