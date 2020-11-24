#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <random>
using namespace std;

random_device r;
default_random_engine e1(r());
const double pi = 3.14159265358979323846;

auto randomize = [](double min, double max) {
    vector<double> x(2);
    uniform_real_distribution<double> uniform_dist(min,max);
    x[0] = uniform_dist(e1);
    x[1] = uniform_dist(e1);

    return x;
};
//auto optimalize = [](auto goal, double min, double max) {
//    vector<double> x(2);
//   // x = randomize(min, max);
//    const double epsilon = 1e-5;
//
//    auto current_solution = x;
//
//    for (int iteration = 0; iteration < 1000000; iteration++) {
//        x = randomize(min, max);
//        auto next_sol = x;
//        if (abs(next_sol.at(0) - next_sol.at(1)) == 0) {
//            current_solution = next_sol;
//        }
//
//    }
//    return current_solution;
//};
auto optimalize = [](auto goal, double min, double max) {
    vector<double> x(2);
    x = randomize(min, max);

    auto current_solution = x;

    for (int iteration = 0; iteration < 1000000; iteration++) {
        x = randomize(min, max);
        auto next_sol = x;
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

    cout << "Funkcja: \n";
    cout << "1 - beale     | f(3, 0.5) = 0\n2 - easom     | f(pi, pi) = -1\n3 - eggholder | f(512, 404) = -959\n";
    cin >> f;

    auto goal = *beale;

    if (f==1) {
        min = -4.5;
        max = 4.5;
    }

    if (f==2) {
        min = -100;
        max = 100;
        goal = easom;
    }

    if (f==3) {
        min = -512;
        max = 512;
        goal = eggholder;
    }

    auto solution = optimalize(goal, min, max);
    cout << "result: [ ";
    for (auto e: solution)
        cout << e << " ";
    cout << "] -> " << goal(solution) << endl;

    return 0;
}
