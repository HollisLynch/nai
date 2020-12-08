#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <random>
#include <sstream>
#include <stdlib.h>

using namespace std;

const double pi = 3.14159265358979323846;
const double e = 2.71828182845904523536;

random_device r;
default_random_engine e1(r());

namespace ranges
{
    template<typename Range, typename Generator>
    void generate(Range& range, Generator generator)
    {
        return std::generate(begin(range), end(range), generator);
    }
}

auto randomNumberBetween = [](int low, int high)
{
    auto randomFunc = [distribution_ = std::uniform_int_distribution<int>(low, high),
            random_engine_ = std::mt19937{ std::random_device{}() }]() mutable
    {
        return distribution_(random_engine_);
    };
    return randomFunc;
};

auto randomize = []() {
    vector<int> xy(128);
    ranges::generate(xy, randomNumberBetween(0, 1));

    return xy;
};

auto fitness = [](auto goal, vector<double> x) {
    return 1/1+goal(x);
};

//auto fit = [](auto goal) {
//    vector<int> x;
//    x = randomize();
//    auto current_solution = x;
//    for (int iteration = 0; iteration < 1000000; iteration++) {
//        x = randomize();
//        auto next_sol = x;
//        if (1/1+goal(x) < 1/1+goal(x)) {
//            current_solution = next_sol;
//        }
//    }
//    return current_solution;
//};

//auto genetic_alg = [](auto fitness, auto generate_init_pop, auto selection, auto crossover, auto mutation) {
//    auto population = generate_init_pop();
//    population = calculate_pop_fitness(population);
//    for (int iteration = 0; iteration < 100000; iteration++)
//    {
//        auto parents = selection(population);
//        auto offspring = crossover(parents);
//        offspring = mutation(offspring);
//        population = calculate_pop_fitness(offspring);
//    }
//    return population;
//};

auto selection = [](auto population) {
    return population;
};

auto crossover = [](auto population) {
    return population;
};

auto mutation = [](auto population) {
    return population;
};

int main(int argc, char** argv) {

    auto ackley = [](vector<double> d){
        double x = d.at(0);
        double y = d.at(1);
        return -20*exp(-0.2*sqrt(0.5*(x*x+y*y))) - exp(0.5*(cos(2*pi*x) + cos(2*pi*y))) + e + 20;
    };

    auto himmelblau = [](vector<double> d){
        double x = d.at(0);
        double y = d.at(1);
        return pow((x*x + y - 11), 2) + pow((x + y*y - 7), 2);
    };

    auto holder_table = [](vector<double> d){
        double x = d.at(0);
        double y = d.at(1);
        return -abs(sin(x)*cos(y)*exp(abs(1 - (sqrt(x*x+y*y))/pi)));
    };

    auto randomize = []() {
        vector<int> xy(128);
        ranges::generate(xy, randomNumberBetween(0, 1));

        return xy;
    };

    auto decode = [](vector<int> v) {
        vector<int> v1;
        vector<int> v2;
        copy(v.begin(), v.begin() + v.size()/2, back_inserter(v1));
        copy(v.begin() + v.size()/2, v.end(), back_inserter(v2));

        ostringstream oss;
        ostringstream oss1;
        if (!v.empty())
        {
            copy(v1.begin(), v1.end()-1,ostream_iterator<int>(oss, ""));
            oss << v1.back();
            copy(v2.begin(), v2.end()-1,ostream_iterator<int>(oss1, ""));
            oss1 << v2.back();
        }

        string x_bin = oss.str();
        string y_bin = oss.str();

        vector<double> res;
        res.push_back(strtol(x_bin.c_str(), nullptr, 2)/pow(10,18) - 5);
        res.push_back(strtol(y_bin.c_str(), nullptr, 2)/pow(10,18));
        return res;
    };



    auto population = randomize();
    auto xy = decode(population);

    auto goal = ackley;
    auto gx = fitness(goal, xy);
    printf("Holder_table | f(8, 9) = 19\nf(%f,%f) = %f\n", xy.at(0), xy.at(1), gx);

    string binary_x = bitset<64>(xy.at(0)+pow(10,18)).to_string();
    cout << "x: " << binary_x << "\n";
    string binary_y = bitset<64>(xy.at(1)*pow(10,18)).to_string();
    cout << "y: " <<binary_y << "\n";

    return 0;
}
