#include "S_Point.hpp"

int main()
{
    system("clear");

    int k = 0;
    
    const double eps = LIM_0;

    Point point{ 100, 100 };

    while(norm(point) > eps)
    {
        Point vec_grad = df::first_partial(point);
        point -= get_step(point) * vec_grad;

        std::cout << std::setfill(' ') << std::setw(20) << ("Iter# " + std::to_string(k++)) << std::setw(20) << "x1" << std::setw(20) << "x2" << std::setw(20) << "f(x1, x2)"<< std::endl << std::endl;
        std::cout << std::setw(20) << "point" << std::setw(20) << point.x << std::setw(20) << point.y << std::setw(20) << point.calc() << std::endl;
        std::cout << std::setw(20) << "gradient" << std::setw(20) << vec_grad.x << std::setw(20) << vec_grad.y << std::setw(20) << "---" << std::endl;
        std::cout << std::setfill('-') << std::setw(82) << " " << std::endl;
    }

    return 0;
}