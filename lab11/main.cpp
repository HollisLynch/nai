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
    vector<double> get_weights();
    vector<double> set_inputs();
    double sum(vector<double> weights, vector<double> inputs);
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

double Neuron::sum(vector<double> weights, vector<double> inputs) {
    double sum = 0;
    for (int i = 0; i<inputs.size(); i++) {
        sum += inputs[i]*weights[i];
    }
    return sum;
}

void Neuron::count_value() {
    for (int i = 0; i<inputs.size(); i++) {
        value = Activation::f_tanh(sum(weights, inputs));
    }
}

vector<double> Neuron::get_weights() {
    return weights;
}

vector<double> Neuron::set_inputs() {
    inputs = {0, 0, 0};
}

int main() {
    return 0;
}
