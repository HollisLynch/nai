#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <random>
#include <sstream>

using namespace std;

const double pi = 3.14159265358979323846;
const double e = 2.71828182845904523536;

random_device r;
default_random_engine e1(r());
mt19937 rand_gen(192987432);

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
    vector<int> xy(100);
    ranges::generate(xy, randomNumberBetween(0, 1));

    return xy;
};

struct specimen_t
{
    vector<int> chromosome;
    double fit;
    specimen_t(int n = 0)
    {
        chromosome.resize(n);
        fit = -1;
    }
    void randomize()
    {
        ranges::generate(chromosome, randomNumberBetween(0, 1));
        fit = -1;
    }
    void print()
    {
        for (auto e : chromosome)
            cout << e;
    }
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

auto ackley = [](vector<double> d){
    double x = d.at(0);
    double y = d.at(1);
    return -20*exp(-0.2*sqrt(0.5*(x*x+y*y))) - exp(0.5*(cos(2*pi*x) + cos(2*pi*y))) + e + 20;
};

auto fitness = []( vector<double> d) {
    double x = d.at(0);
    double y = d.at(1);
    auto goal = -20*exp(-0.2*sqrt(0.5*(x*x+y*y))) - exp(0.5*(cos(2*pi*x) + cos(2*pi*y))) + e + 20;
    return 1/1+goal;
};

auto decode = [](specimen_t v) {
    vector<int> v1;
    vector<int> v2;
    copy(v.chromosome.begin(), v.chromosome.begin() + v.chromosome.size()/2, back_inserter(v1));
    copy(v.chromosome.begin() + v.chromosome.size()/2, v.chromosome.end(), back_inserter(v2));

    ostringstream oss;
    ostringstream oss1;
    if (!v.chromosome.empty())
    {
        copy(v1.begin(), v1.end()-1,ostream_iterator<int>(oss, ""));
        oss << v1.back();
        copy(v2.begin(), v2.end()-1,ostream_iterator<int>(oss1, ""));
        oss1 << v2.back();
    }

    string x_bin = oss.str();
    string y_bin = oss1.str();

    // Convert string to long integer
    vector<double> res;
    res.push_back(strtol(x_bin.c_str(), nullptr, 2)/pow(10,18)-6);
    res.push_back(strtol(y_bin.c_str(), nullptr, 2)/pow(10,18));
    return res;
};

vector<specimen_t> calculate_pop_fitness(vector<specimen_t> pop)
{
    using namespace std;
    std::vector<specimen_t> ret;
    ret.reserve(pop.size());
    for (auto e : pop)
    {
        e.fit = fitness(decode(e));
        ret.push_back(e);
    }

    return ret;
}

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
    decltype(population) selected;
    uniform_int_distribution<int> dist(0, population.size() - 1);
    for (int c = 0; c < population.size(); c++)
    {
        decltype(population) tournament;
        for (int t = 0; t < 2; t++)
        {
            tournament.push_back(population.at(dist(rand_gen)));
        }
        sort(tournament.begin(), tournament.end(), [](auto a, auto b) {
            return fitness(decode(a)) > fitness(decode(b));
        });
        selected.push_back(tournament.at(0));
    }
    return selected;
};

auto crossover_one_point = [](auto population, double p = 0.9) {
    decltype(population) crossed;
    uniform_real_distribution<double> rr(0.0, 1.0);
    for (int i = 0; i< (population.size()-1); i+=2) {
        auto c_1 = population.at(i);
        auto c_2 = population.at(i+1);
        if (rr(rand_gen) < p) {
            uniform_int_distribution<int> dist(0, c_1.chromosome.size() - 1);
            auto cross_point = dist(rand_gen);

            for (int g = cross_point; g < c_1.chromosome.size(); g++)
            {
                swap(c_1.chromosome[g], c_2.chromosome[g]);
            }
        }
        crossed.push_back(c_1);
        crossed.push_back(c_2);
    }
    return crossed;
};

auto mutate = [](auto population, double p = 0.1) {
    decltype(population) mutated;
    uniform_real_distribution<double> rr(0.0, 1.0);
    for (int i = 0; i< population.size(); i++) {
        auto c = population.at(i);
        for (int j = 0; j<c.chromosome.size(); j++) {
            if (rr(rand_gen) < p) {
                c.chromosome[j] = 1 - c.chromosome[j];
            }
        }
        mutated.push_back(c);
    }
    return mutated;
};

int main(int argc, char** argv) {

    auto generate_init_pop = []() {
        vector<specimen_t> xy;
        for (int i = 0; i < 4; i++)
        {
            xy.push_back(130);
            xy[i].randomize();
        }
        return xy;
    };

    auto population = generate_init_pop();

    cout << "population:\n";
    for (int i = 0; i< population.size(); i++) {
        population.at(i).print();
        cout << endl;
    }
    printf("[1] x:%f, y:%f, goal = %f\n", decode(population.at(0)).at(0), decode(population.at(0)).at(1), fitness(decode(population.at(0))));
    printf("[2] x:%f, y:%f, goal = %f\n", decode(population.at(1)).at(0), decode(population.at(1)).at(1), fitness(decode(population.at(1))));
    printf("[3] x:%f, y:%f, goal = %f\n", decode(population.at(2)).at(0), decode(population.at(2)).at(1), fitness(decode(population.at(2))));
    printf("[4] x:%f, y:%f, goal = %f\n", decode(population.at(3)).at(0), decode(population.at(3)).at(1), fitness(decode(population.at(3))));

    auto parents = selection(population);
    cout << "parents:\n";
    for (int i = 0; i< parents.size(); i++) {
        parents.at(i).print();
        cout << endl;
    }
    printf("goal: %f\n", fitness(decode(parents.at(0))));
    printf("goal: %f\n", fitness(decode(parents.at(1))));
    printf("goal: %f\n", fitness(decode(parents.at(2))));
    printf("goal: %f\n", fitness(decode(parents.at(3))));

    auto crossover = crossover_one_point(parents);
    cout << "crossed:\n";
    for (int i = 0; i< crossover.size(); i++) {
        crossover.at(i).print();
        cout << endl;
    }
    printf("goal: %f\n", fitness(decode(crossover.at(0))));
    printf("goal: %f\n", fitness(decode(crossover.at(1))));
    printf("goal: %f\n", fitness(decode(crossover.at(2))));
    printf("goal: %f\n", fitness(decode(crossover.at(3))));

    auto mutation = mutate(crossover);
    cout << "mutated:\n";
    for (int i = 0; i< mutation.size(); i++) {
        mutation.at(i).print();
        cout << endl;
    }
    printf("goal: %f\n", fitness(decode(mutation.at(0))));
    printf("goal: %f\n", fitness(decode(mutation.at(1))));
    printf("goal: %f\n", fitness(decode(mutation.at(2))));
    printf("goal: %f\n", fitness(decode(mutation.at(3))));

    auto chromosome1 = decode(mutation.at(0));
    auto chromosome2 = decode(mutation.at(1));

    printf("Holder_table | f(8, 9) = 19\nf(%f,%f) = %f\n", chromosome1.at(0), chromosome1.at(1), fitness(chromosome1));
    printf("f(%f,%f) = %f\n", chromosome2.at(0), chromosome2.at(1), fitness(chromosome2));

    return 0;
}
