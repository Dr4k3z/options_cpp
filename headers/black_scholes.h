//
// Created by matte on 10/24/2024.
//

#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H

#include "options_european.h"
#include "calendar.h"

#include <vector>

enum RootFindingAlgo {
    BisectionMethod,
    NewtonRaphson
};

namespace BlackScholes{
    float d1(const Option& option, float S, float sigma, float rate, float tmt=-1.0);
    float d2(const Option& option, float S, float sigma, float rate, float tmt=-1.0);

    // Greeks
    float delta(const Option& option, float S, float sigma, float rate, float tmt=-1.0);
    float gamma(const Option& option, float S, float sigma, float rate, float tmt=-1.0);
    float theta(const Option& option, float S, float sigma, float rate, float tmt=-1.0);
    float rho(const Option& option, float S, float sigma, float rate, float tmt=-1.0);
    float vega(const Option& option, float S, float sigma, float rate, float tmt=-1.0);

    // Pricing and volatility
    float price(const Option& option, float S, float sigma, float rate, float tmt=-1.0);
    // I dont like the double default value, I should find a way to avoid it
    float impliedVolatility(const Option& option, float S, float rate, float tmt=-1.0, float marketPrice=0.0, RootFindingAlgo algo=NewtonRaphson); // non-calendarized time to maturity
    std::vector<float> calibrate(const OptionChain* chain, float S, float rate);
};

#endif //BLACK_SCHOLES_H
