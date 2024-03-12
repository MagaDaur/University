#include "S_Point.hpp"

//Vector of partial derivatives ( gradient )
Point df_dx_dy(const Point& p)
{
    double dx = ( f(p.x + LIM_0, p.y) - f(p.x, p.y) ) / LIM_0;
    double dy = ( f(p.x, p.y + LIM_0) - f(p.x, p.y) ) / LIM_0;

    return {dx, dy};
}

//Vector of partial second derivatives
Point ddf_dx_dy(const Point& p)
{
    double u1_x = f(p.x + LIM_0, p.y);
    double u3_x = f(p.x - LIM_0, p.y);

    double u1_y = f(p.x, p.y + LIM_0);
    double u3_y = f(p.x, p.y - LIM_0);

    double u2 = f(p.x, p.y);

    double ddx = (u1_x - 2.0 * u2 + u3_x) / (LIM_0 * LIM_0);
    double ddy = (u1_y - 2.0 * u2 + u3_y) / (LIM_0 * LIM_0);

    return {ddx, ddy};
}

//Mixed second derivative
double ddf_dxdy(const Point& p)
{
    double u1 = f(p.x, p.y);
    double u2 = f(p.x - LIM_0, p.y);
    double u3 = f(p.x, p.y - LIM_0);
    double u4 = f(p.x - LIM_0, p.y - LIM_0);

    return (u1 - u2 - u3 + u4) / (LIM_0 * LIM_0);
}

double norm(const Point& p)
{
    Point vec_grad = df_dx_dy(p);
    return sqrt(vec_grad.x * vec_grad.x + vec_grad.y * vec_grad.y);
}

matrix matrix_multiply(matrix a, matrix b)
{
    matrix res(a.size());
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

double get_step(const Point& p)
{
    auto vec_grad = df_dx_dy(p);
    auto vec_ddf = ddf_dx_dy(p);
    auto mixed_ddf = ddf_dxdy(p);

    matrix gesse_matrix = 
    {
        {vec_ddf.x, mixed_ddf},
        {mixed_ddf, vec_ddf.y},
    };

    matrix grad_matrix_hor =
    {
        {vec_grad.x, vec_grad.y},
    };

    matrix grad_matrix_vert =
    {
        {vec_grad.x},
        {vec_grad.y},
    };

    matrix num = matrix_multiply(grad_matrix_hor, grad_matrix_vert);
    matrix denom = matrix_multiply(grad_matrix_hor, matrix_multiply(gesse_matrix, grad_matrix_vert));

    return num[0][0] / denom [0][0];
}

int main()
{
    system("clear");

    int k = 0;
    
    const double eps = LIM_0;

    Point point{ 1, 1 };

    while(norm(point) > eps)
    {
        Point vec_grad = df_dx_dy(point);
        point -= get_step(point) * vec_grad;

        std::cout << std::setfill(' ') << std::setw(20) << ("Iter# " + std::to_string(k++)) << std::setw(20) << "x1" << std::setw(20) << "x2" << std::setw(20) << "f(x1, x2)"<< std::endl << std::endl;
        std::cout << std::setw(20) << "point" << std::setw(20) << point.x << std::setw(20) << point.y << std::setw(20) << point.calc() << std::endl;
        std::cout << std::setw(20) << "gradient" << std::setw(20) << vec_grad.x << std::setw(20) << vec_grad.y << std::setw(20) << "---" << std::endl;
        std::cout << std::setfill('-') << std::setw(82) << " " << std::endl;
    }

    return 0;
}