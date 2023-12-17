#include <algorithm>
#include <random>
#include <iostream>
#include <vector>
#include <numeric>
#include <ctime>

struct SwapIter { int from; int to; };

double rd() { return double(rand()) / RAND_MAX; }

std::vector<int> SetupPorbabilities(int k)
{
    std::vector<int> probabilities(k);
    for(int i = 0; i < k; i++)
        probabilities[i] = (rd() * 100);
    return probabilities;
}

std::vector<SwapIter> SetupSwapTable(int n, int k)
{
    std::vector<SwapIter> swap_table(k);
    for(int i = 0; i < k; i++)
        swap_table[i] = {1 + (rand() % (n - 2)), 1 + (rand() % (n - 2))};
    return swap_table;
}

std::vector<std::vector<int>> SetupEdges(int n)
{
    std::vector<std::vector<int>> l(n);
    for(int i = 0; i < n; i++)
    {
        l[i].reserve(n);
        for(int j = 0; j < n; j++)
        {
            if(j < i)
            {
                l[i][j] = l[j][i];
                continue;
            }
            else if(j == i)
            {
                l[i][j] = 0;
                continue;
            }

            l[i][j] = 50 * (rd() + 0.1);
        }
    }
    return l;
}

int f(const std::vector<int>& path, const std::vector<std::vector<int>> &l)
{
    int res = 0;
    for(int i = 1; i < path.size(); i++)
        res += l[path[i - 1]][path[i]];
    return res;
}

int main()
{
    system("clear");
    srand(time(0));

    const int n = 10;
    const int k = 20;
    const double y = 0.5;
    double t = 100.0;

    std::vector<std::vector<int>> l = SetupEdges(n);
    std::vector<int> probabilities = SetupPorbabilities(k);
    std::vector<SwapIter> swap_table = SetupSwapTable(n, k);

    std::vector<int> v(n); std::iota(v.begin(), v.end(), 0);
    std::shuffle(v.begin(), v.end(), std::default_random_engine(time(0)));
    v.push_back(v[0]);

    std::cout << "\nДлины граней:\n";
    for(int i = 0; i < n; i++)
        for(int j = i + 1; j < n; j++)
            std::cout << i + 1 << " -> " << j + 1 << " = " << l[i][j] << "\n";

    std::cout << "\nВероятности: {";
    for(int prob : probabilities)
        std::cout << " " << prob;
    std::cout << " }\n";

    std::cout << "\nЗамены:\n";
    for(const auto& iter : swap_table)
        std::cout << iter.from + 1 << " <> " << iter.to + 1 << "\n";

    int best_s = f(v, l);

    std::cout << "\nНачальный Маршрут: {";
    for(auto i : v)
        std::cout << " " << i;
    std::cout << " } S = " << best_s << "\n";

    for(int i = 0; i < k; i++)
    {
        std::cout << "\nИтерация " << i + 1 << "\n\n";

        std::cout << "T = " << t << "\n";
        
        std::vector<int> u = v;
        SwapIter swap_iter = swap_table[i];

        std::cout << "Замена = " << swap_iter.from + 1 << " <> " << swap_iter.to + 1 << "\n";

        std::swap(u[swap_iter.from], u[swap_iter.to]);

        int s = f(u, l);
        double ds = s - best_s;
        double p = 100.0 * exp(-ds / t);

        std::cout << "Вероятность P = " << p << " <> " << probabilities[i] << "\n";

        std::cout << "Новый Маршрут: {";
        for(auto idx : u)
            std::cout << " " << idx;
        std::cout << " } S = " << s << "\n";

        if(ds < 0 || p > probabilities[i])
        {
            best_s = s;
            v = u;

            if(ds < 0)
                std::cout << "Маршрут принят т.к. dS = " << ds << " < 0.\n\n";
            else
                std::cout << "Маршрут принят т.к. P = " << p << " > " << probabilities[i] <<".\n\n";
        }
        else
            std::cout << "Маршрут не принят.\n\n";

        t *= y;
    }

    std::cout << "\nЛучший маршрут {";
    for(auto idx : v)
        std::cout << " " << idx;
    std::cout << " }, где S = " << best_s << ".\n";

    return 0;
}