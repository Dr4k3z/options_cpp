//
// Created by matte on 10/24/2024.
//

#ifndef MATH_H
#define MATH_H

#include <vector>
#include <functional>


namespace Math{
    // Probability distribution and comulative functions
    float normalPDF(float value, float mean=0.0, float std = 1.0);
    float normalCDF(float value, float mean=0.0, float std = 1.0);

    // Linear Algebra
    std::vector<float> linspace(float start, float end, float step);
    std::vector<float> cumsum(const std::vector<float>& vec);
    float vectorCumSum(const std::vector<float>& v1, const std::vector<float>& v2);
    template<typename T> void print(std::vector<T> v);

    // Solving non-linear equations
    template<typename T> float bisectionMethod(std::function<T(T)> func, T x0, T xmax, T tol);
}

#endif //MATH_H
