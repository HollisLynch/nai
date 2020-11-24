#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <random>
#include <iostream>
#include <fstream>
using namespace std;

random_device r;
default_random_engine e1(r());
const double pi = 3.14159265358979323846;
const double e = 2.71828182845904523536;

auto randomize = [](double min, double max) {
    vector<double> x(2);
    uniform_real_distribution<double> uniform_dist(min,max);
    x[0] = uniform_dist(e1);
    x[1] = uniform_dist(e1);

    return x;
};

auto optimalize = [](auto goal, double min, double max, auto callback) {
    vector<double> x(2);
    x = randomize(min, max);
    ofstream myfile;
    myfile.open ("example.txt");
    auto current_solution = x;

    for (int iteration = 0; iteration < 1000000; iteration++) {
        x = randomize(min, max);
        auto next_sol = x;
            if (goal(current_solution) > goal(next_sol)) {
            current_solution = next_sol;
            }
            myfile << iteration << " ";
            myfile << current_solution.at(0);
            callback(iteration, current_solution);
        }
    myfile.close();
    return current_solution;
};

auto hill_climb = [](auto goal, double min, double max, auto get_random_neighbours, int max_iterations) {
    vector<double> x(2);
    x = randomize(min, max);

    auto current_solution = x;

    for (int iteration = 0; iteration < max_iterations; iteration++) {
        auto next_solutions = get_random_neighbours(current_solution);
        uniform_int_distribution<int> uniform_dist(0,next_solutions.size()-1);
        auto next_sol = next_solutions.at(uniform_dist(e1));
        if (goal(current_solution) > goal(next_sol)) {
            current_solution = next_sol;
        }
    }
    return current_solution;
};

int main() {
    auto min = 0.0;
    auto max = 0.0;
    int f;

    auto beale = [](vector<double> d){
        double x = d.at(0);
        double y = d.at(1);
        return pow(1.5 - x + x*y, 2) + pow((2.25 - x + x * pow(y,2)), 2) + pow((2.625 - x + x * pow(y, 3)), 2);
    };

    auto eggholder = [](vector<double> d){
        double x = d.at(0);
        double y = d.at(1);
        return -(y + 47) * sin(sqrt(abs(x/2 + (y+47)))) - x * sin(sqrt(abs(x-(y+47))));
    };

    auto easom = [](vector<double> d){
        double x = d.at(0);
        double y = d.at(1);
        return -cos(x) * cos(y) * exp(-((pow(x-pi, 2)) + pow(y-pi, 2)));
    };

    auto holder_table = [](vector<double> d){
        double x = d.at(0);
        double y = d.at(1);
        return -abs(sin(x)*cos(y)*exp(abs(1 - (sqrt(x*x+y*y))/pi)));
    };

    auto matyas = [](vector<double> d){
        double x = d.at(0);
        double y = d.at(1);
        return 0.26 * (x*x + y*y) - 0.48*x*y;
    };

    auto booth = [](vector<double> d){
        double x = d.at(0);
        double y = d.at(1);
        return pow(x+2*y-7, 2) + pow(2*x+y-5, 2);
    };

    auto ackley = [](vector<double> d){
        double x = d.at(0);
        double y = d.at(1);
        return -20*exp(-0.2*sqrt(0.5*(x*x+y*y))) - exp(0.5*(cos(2*pi*x) + cos(2*pi*y))) + e + 20;
    };

    auto neighbours = [](vector<double> x, double dx = 0.001) {
        vector<vector<double>> ret;
        for (int i = 0; i < x.size(); i++) {
            auto nx = x;
            nx[i] += dx;
            ret.push_back(nx);
            nx[i] -= 2.0 * dx;
            ret.push_back(nx);
        }
        return ret;
    };

        cout << "Funkcja: \n";
        cout << "1 - beale        "
                "| f(3, 0.5) = 0\n2 - easom       "
                " | f(pi, pi) = -1\n3 - eggholder    "
                "| f(512, 404) = -959\n4 - holder table "
                "| f(8, 9) = -19\n"
                "5 - matyas       "
                "| f(0, 0) = 0\n"
                "6 - booth        "
                "| f(1, 3) = 0\n";
        cin >> f;

        auto goal = *beale;

        if (f == 1) {
            min = -4.5;
            max = 4.5;
        }
        if (f == 2) {
            min = -100;
            max = 100;
            goal = easom;
        }
        if (f == 3) {
            min = -512;
            max = 512;
            goal = eggholder;
        }
        if (f == 4) {
            min = -10;
            max = 10;
            goal = holder_table;
        }
        if (f == 5) {
            min = -10;
            max = 10;
            goal = matyas;
        }
        if (f == 6) {
            min = -10;
            max = 10;
            goal = booth;
        }
        if (f == 7) {
            min = -5;
            max = 5;
            goal = ackley;
        }

        //auto solution = hill_climb(goal, min, max, neighbours, 100000);
        auto solution = optimalize(goal, min, max,
                                   [goal](int i, vector<double> v){
                                    cout << i << " " << goal(v) << endl;
        });
        cout << "result: [ ";
        for (auto e: solution)
            cout << e << " ";
        cout << "] -> " << goal(solution) << endl;

        return 0;

}
