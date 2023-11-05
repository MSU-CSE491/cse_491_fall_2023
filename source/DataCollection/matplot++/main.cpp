//
// Created by Yones on 10/26/2023.
//
// main.cpp
#include <iostream>
#include "/matplotplus.hpp"

int main() {
    MatplotPlusPlus plotter;
    std::vector<double> x = {1, 2, 3, 4, 5};
    std::vector<double> y = {2, 4, 6, 8, 10};

    plotter.plot(x, y);

    return 0;
}