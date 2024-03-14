#include "S_Point.hpp"

Point get_raphson_step(const Point& p)
{
    auto df = df::first_partial(p);

    auto gesse = Matrix::gesse(p);
    auto gesse_det = Matrix::det(gesse);
    
    if(gesse_det < 0) 
        gesse =
        {
            { 1, 0 },
            { 0, 1 }
        };

    auto step = get_step(p);
    auto gesse_inv = Matrix::inverse(gesse);

    Matrix::type grad_matrix_hor =
    {
        {df.x},
        {df.y},
    };

    auto matrix_step = step * Matrix::multiply(gesse_inv, grad_matrix_hor);
    return { matrix_step[0][0], matrix_step[1][0] };
}

int main()
{
    Point point = {100, 100};

    const double eps = LIM_0;
    int k = 0;

    while(norm(point) > eps)
    {
        auto step = get_raphson_step(point);

        point -= step;

        std::cout << std::setfill(' ') << std::setw(20) << ("Iter# " + std::to_string(k++)) << std::setw(20) << "x1" << std::setw(20) << "x2" << std::setw(20) << "f(x1, x2)"<< std::endl << std::endl;
        std::cout << std::setw(20) << "point" << std::setw(20) << point.x << std::setw(20) << point.y << std::setw(20) << point.calc() << std::endl;
        std::cout << std::setw(20) << "step" << std::setw(20) << step.x << std::setw(20) << step.y << std::setw(20) << "---" << std::endl;
        std::cout << std::setfill('-') << std::setw(82) << " " << std::endl;
    }

    return 0;
}