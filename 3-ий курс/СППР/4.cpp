#include <iostream>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <iomanip>

struct Vector2D
{
    double x{};
    double y{};

    void operator=(const Vector2D& other)
    {
        x = other.x;
        y = other.y;
    }

    friend std::ostream& operator<<(std::ostream& to, const Vector2D& from)
    {
        return to << "( " << from.x << " ; " << from.y << " )";
    }

    double distance(const Vector2D& other)
    {
        return sqrt(pow(other.x - x, 2) + pow(other.y - y, 2));
    }
};

struct Point : Vector2D
{

};

double rn() { return double(rand()) / RAND_MAX; }

double rn(const double& min, const double& max) {return min + (max - min) * rn(); }
double rn(const int& min, const int& max) { return min + double(max - min) * rn(); }

const int max_ants = 10;
const int max_cities = 15;
const int max_distance = 100;

const double alpha = 2.5;
const double beta = 2.0;
const double rho = 0.5;
const double qval = 100.0;

const double default_pheromone = 1.0 / max_cities;

struct Ant
{
    int city_index;

    bool seen_cities[max_cities];
    std::vector<int> path;

    double tour_length;
    double prev_tour_length;

    void SelectNextCity();
    bool Simulate();

    static double Product(int, int);

    friend std::ostream& operator<<(std::ostream& s, const Ant& ant)
    {
        s << ant.path[0];
        for(int i = 1; i < ant.path.size(); i++)
            s << " -> " << ant.path[i];
        s << " = " << ant.tour_length << "; dS = " << ant.tour_length - ant.prev_tour_length;
        return s;
    }
};

Point cities[max_cities];
Ant ants[max_ants];

double distances[max_cities][max_cities];
double pheromones[max_cities][max_cities];
bool obstacles[max_cities][max_cities];

double Ant::Product(int from, int to)
{
    return pow(pheromones[from][to], alpha) * pow(1.0 / distances[from][to], beta);
}

void Ant::SelectNextCity()
{
    double denom = 0.0;

    for(int to = 0; to < max_cities; to++)
        if(!seen_cities[to])
            denom += Product(city_index, to);

    assert(denom != 0.0);

    int to = 0;
    while(1)
    {
        to = (to + 1) % max_cities;
        if(seen_cities[to]) continue;

        double p = Product(city_index, to) / denom;
        if(p > rn()) break;
    }

    tour_length += distances[city_index][to];
    city_index = to;
    seen_cities[to] = true;
    path.push_back(to);
}

bool Ant::Simulate()
{
    if(path.size() == max_cities + 1) return true;

    SelectNextCity();

    if(path.size() == max_cities)
    {
        path.push_back(path[0]);
        tour_length += distances[city_index][path[0]];
        return true;
    }

    return false;
}

void ResetAnts()
{
    for(int i = 0; i < max_ants; i++)
    {
        ants[i].prev_tour_length = ants[i].tour_length;

        ants[i].city_index = i % max_cities;
        ants[i].tour_length = 0.0;

        for(int j = 0; j < max_cities; j++)
            ants[i].seen_cities[j] = false;

        ants[i].seen_cities[ants[i].city_index] = true;

        ants[i].path.clear();
        ants[i].path.push_back(ants[i].city_index);
    }
}

void Init()
{
    for(int i = 0; i < max_cities; i++)
    {
        cities[i].x = rn(0, max_distance);
        cities[i].y = rn(0, max_distance);
    }

    for(int i = 0; i < max_cities; i++)
    {
        for(int j = i + 1; j < max_cities; j++)
        {
            double dst = cities[i].distance(cities[j]);

            distances[i][j] = dst;
            distances[j][i] = dst;

            pheromones[i][j] = default_pheromone;
            pheromones[j][i] = default_pheromone;
        }
    }

    // int from_obstacle = rn(0, max_cities);
    // int to_obstacle;
    // do
    // {
    //     to_obstacle = rn(0, max_cities);
    // } while (to_obstacle == from_obstacle);
    
    // obstacles[from_obstacle][to_obstacle] = true;
    // obstacles[to_obstacle][from_obstacle] = true;

    ResetAnts();
}

void UpdateTrails()
{
    for(int i = 0; i < max_cities; i++)
    {
        for(int j = i + 1; j < max_cities; j++)
        {
            pheromones[i][j] *= (1.0 - rho);
            if(pheromones[i][j] <= 0.1)
                pheromones[i][j] = default_pheromone;

            pheromones[j][i] = pheromones[i][j];
        }
    }

    for(const auto& ant : ants)
    {
        for(int i = 0; i < ant.path.size() - 1; i++)
        {
            int from = ant.path[i];
            int to = ant.path[i + 1];

            pheromones[from][to] += (qval / ant.tour_length);
            pheromones[to][from] = pheromones[from][to];
        }
    }

    for(int i = 0; i < max_cities; i++)
        for(int j = 0; j < max_cities; j++)
            pheromones[i][j] *= rho;
}

int main()
{
    system("clear");
    //srand(time(0));

    const int k = 10;

    Init();

    for(int i = 0; i < k; i++)
    {
        std::cout << "Итерация №" << i + 1 << std::endl;
        while(std::count_if(std::cbegin(ants), std::cend(ants), [](const Ant& ant) { return ant.path.size() == max_cities + 1; }) < max_ants)
            for(auto& ant : ants)
                if(ant.Simulate())
                    UpdateTrails();

        for(const auto& ant : ants)
            std::cout << ant << std::endl;
        
        std::cout << "\n\n";

        ResetAnts();
    }

    return 0;
}