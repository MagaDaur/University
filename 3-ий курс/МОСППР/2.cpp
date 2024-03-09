#include "S_Point.hpp"

float f(float x, float y)
{
    return 2 * x * x - 2 * x * y + 3 * y * y + x - 3 * y;
}

Point grad(const Point& p)
{
    float dx = 4.f * p.x - 2.f * p.y + 1.f;
    float dy = 6.f * p.y - 2.f * p.x - 3.f;
    return {dx, dy};
}

float norm(const Point& p)
{
    Point p_grad = grad(p);
    return sqrtf(p_grad.x * p_grad.x + p_grad.y * p_grad.y);
}

Point second_deriv(const Point& p)
{
    float u1_x = f(p.x + DFX, p.y);
    float u3_x = f(p.x - DFX, p.y);

    float u1_y = f(p.x, p.y + DFX);
    float u3_y = f(p.x, p.y - DFX);

    float u2 = f(p.x, p.y);

    float ddx = (u1_x - 2 * u2 + u3_x) / (DFX * DFX);
    float ddy = (u1_y - 2 * u2 + u3_y) / (DFX * DFX);

    return {ddx, ddy};
}

float second_deriv_both(const Point& p)
{
    float u1 = f(p.x, p.y);
    float u2 = f(p.x - DFX, p.y);
    float u3 = f(p.x, p.y - DFX);
    float u4 = f(p.x - DFX, p.y - DFX);

    return (u1 - u2 - u3 + u4) / (DFX * DFX);
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

float get_step(const Point& p)
{
    auto p_grad = grad(p);
    auto sec_deriv = second_deriv(p);

    matrix gesse_matrix = 
    {
        {sec_deriv.x, second_deriv_both(p)},
        {second_deriv_both(p), sec_deriv.y},
    };

    matrix grad_matrix_hor =
    {
        {p_grad.x, p_grad.y},
    };

    matrix grad_matrix_vert =
    {
        {p_grad.x},
        {p_grad.y},
    };

    matrix tmp = matrix_multiply(gesse_matrix, grad_matrix_vert);
    tmp = matrix_multiply(grad_matrix_hor, tmp);

    float val = tmp[0][0];

    tmp = matrix_multiply(grad_matrix_hor, grad_matrix_vert);
    return tmp[0][0] / val;
}

int main()
{
    system("clear");

    int k = 0;
    
    const float eps = 0.00001;

    Point point{1, 1};

    while(norm(point) > eps)
    {
        float h = get_step(point);

        Point p_grad = grad(point);
        point -= h * p_grad;

        std::cout << std::setfill(' ') << std::setw(20) << ("Iter# " + std::to_string(k++)) << std::setw(20) << "x" << std::setw(20) << "y" << std::setw(20) << "f(x, y)"<< std::endl << std::endl;
        std::cout << std::setw(20) << "point" << std::setw(20) << point.x << std::setw(20) << point.y << std::setw(20) << point.calc() << std::endl;
        std::cout << std::setw(20) << "gradient" << std::setw(20) << p_grad.x << std::setw(20) << p_grad.y << std::setw(20) << "---" << std::endl;
        std::cout << std::setfill('-') << std::setw(82) << " " << std::endl;
    }

    return 0;
}