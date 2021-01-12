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
    double value;

    Neuron();
    vector<double> set_weights(int n);
    void train(int epochs);
    void test(vector<double> weights);
    void count_value();
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

void Neuron::train(int epochs) {
    for (int i = 0; i<epochs; i++) {
    }
}

void Neuron::test(vector<double> weights) {
}

void Neuron::count_value() {
    for (int i = 0; i<inputs.size(); i++) {
        value = weights[i]*inputs[i];
    }
}

int main() {
    return 0;
}
