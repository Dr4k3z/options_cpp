//
// Created by matte on 10/24/2024.
//

#include "../headers/math.h"

#include <cmath>
#include <stdexcept>
#include <iostream>
#include <numeric>

float Math::normalPDF(float value, float mean, float std){
    return 1/(std::sqrt(2*M_PI)*std)*std::exp(-0.5*std::pow((value-mean)/std,2));
}

float Math::normalCDF(float value, float mean, float std){
    return 0.5 * (1.0 + std::erf((value - mean) / (std * std::sqrt(2.0))));
}

std::vector<float> Math::linspace(float start, float end, float step = 1.0){
    std::vector<float> seq;

    if (start < end && step > 0){
        for (float i = start; i <= end; i += step){
            seq.push_back(i);
        }
    }else if (start > end && step < 0){
        for (float i = start; i >= end; i += step){
            seq.push_back(i);
        }
    }else{
        throw std::runtime_error("Something went wrong in linspace function");
    }

    return seq;
}

template<typename T>
float Math::bisectionMethod(std::function<T(T)> func, T x0, T xmax, T tol){
    // Bisection method to find the root of a function
    unsigned long maxIter = 10*9;

    T fa = func(x0);
    T fb = func(xmax);

    // I dont like this way of handling errors and warnings
    if (fa*fb > 0){
        std::cout << "Warning! The function has the same sign at the endpoints of the interval\n";
        return 0;
    }

    //assert(fa*fb < 0 && "The function has the same sign at the endpoints of the interval");

    T c = x0;
    unsigned long iter = 0;
    while ((xmax-x0) > tol && iter < maxIter){
        c = (x0+xmax)/2.0;

        if (std::fabs(func(c)) < tol){
            break;
        }

        if (func(x0)*func(c) < 0){
            xmax = c;
        }else{
            x0 = c;
        }
        iter++;
    }

    //std::cout << "Bisection method converged in " << iter << " iterations\n";

    if (iter >= maxIter){
        //throw warning("Bisection method did not converge");
        std::cout << "Warning! Bisection did not converge\n";
    }
    return c;
}

std::vector<float> Math::cumsum(const std::vector<float>& vec) {
    // Function to compute the cumulative sum
    std::vector<float> result(vec.size());
    std::partial_sum(vec.begin(), vec.end(), result.begin());
    return result;
}

// I need a LINALG library no cap
float Math::vectorCumSum(const std::vector<float>& v1, const std::vector<float>& v2){
    if (v1.size() != v2.size()){
        throw std::runtime_error("Vectors must be of the same size");
    }

    size_t n = v1.size();
    float sum = 0.0;
    for (size_t i=0; i<n;i++){
        sum += v1[i]+v2[i];
    }
    return sum;
}

template<typename T>
void Math::print(std::vector<T> v){
    for (auto c : v){
        std::cout << c << " ";
    }
    std::cout << std::endl;
}

template float Math::bisectionMethod<float>(std::function<float(float)> func, float x0, float xmax, float tol);
template void Math::print<float>(std::vector<float> v);