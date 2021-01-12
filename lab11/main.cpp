#include <iostream>
#include <vector>
#include <random>
#include <cmath>

std::random_device r;
std::mt19937 rg(r());

using namespace std;

class Activation {
public:
    static double f_tanh(double x) {
        return tanh(x);
    };
};

class Neuron {
public:
    vector<double> weights;
    vector<double> inputs;

    Neuron();
    double fprim(double x);
    vector<double> set_weights(int n);
    void train(int epochs);
    void test(vector<double> weights);
};

Neuron::Neuron() {
}

vector<double> Neuron::set_weights(int n) {

    uniform_real_distribution<double> distribution(0.0,1.0);
    for (int i = 0; i < n; i++)
    {
        weights.push_back(distribution(rg));
    }
    return weights;
}

double Neuron::fprim(double x) {
    return pow(1-Activation::f_tanh(x), 2);
}

void Neuron::train(int epochs) {
    for (int i = 0; i<epochs; i++) {
    }
}

void Neuron::test(vector<double> weights) {
}

int main() {
    return 0;
}
