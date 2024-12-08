//
// Created by matte on 10/24/2024.
//

#include "../headers/black_scholes.h"
#include "../headers/math.h"

//BlackScholes namespace
float BlackScholes::d1(const Option& option, float S, float sigma, float rate, float tmt){
       float K = option.getStrike();
       return (std::log(S/K)+(rate+0.5*std::pow(sigma,2))*tmt)/(sigma*std::sqrt(tmt));
}

float BlackScholes::d2(const Option& option, float S, float sigma, float rate, float tmt){
       return d1(option,S,sigma,rate,tmt)-sigma*std::sqrt(tmt);
}

float BlackScholes::vega(const Option &option, float S, float sigma, float rate, float tmt) {
       if (tmt == -1.0) {
              tmt = option.time2maturity();
       }

       const float d1 = BlackScholes::d1(option, S, sigma, rate, tmt);
       return S*Math::normalCDF(d1)*sqrt(tmt);
}

float newton_raphson_method(const Option& option, float S, float marketPrice, float rate, float tmt) {
       if (tmt == -1.0) {
              tmt = option.time2maturity();
       }

       float x_old = 0.0; // initial guess
       float toll = 1e-5;
       constexpr unsigned int max_iter = 1000;
       auto diff = [&option,S,tmt,rate,marketPrice](float x){return BlackScholes::price(option,S,x,rate,tmt)-marketPrice;};

       unsigned int iter = 0;
       float x_new = x_old+1;
       while (iter < max_iter and diff(x_new) > toll) {
              x_old = x_new;
              x_new = x_old - diff(x_old)/BlackScholes::vega(option, S,x_old, rate, tmt);
              iter++;
       }

       if (iter == max_iter) {
              std::cout << "Warning! max iterations reached\n";
       }

       return x_new;
}

float BlackScholes::price(const Option& option, float S, float sigma, float rate, float tmt){
       // is it necessary?
       if (option.getClass() != European){
              std::cerr << "This function can only price european options!" << std::endl;
       }

       if (tmt == -1.0){
              tmt = option.time2maturity();
       }

       float K  = option.getStrike();
       float d1 = BlackScholes::d1(option,S,sigma,rate,tmt);
       float d2 = BlackScholes::d2(option,S,sigma,rate,tmt);

       // Why does this work?
       if (const EuropeanCallOption* callOption = dynamic_cast<const EuropeanCallOption*>(&option)){
              return Math::normalCDF(d1)*S-Math::normalCDF(d2)*K*exp(-rate*tmt);
       }else if (const EuropeanPutOption* putOption = dynamic_cast<const EuropeanPutOption*>(&option)){
              return K*std::exp(-rate*tmt)*Math::normalCDF(-d2) - S*Math::normalCDF(-d1);
       }else{
              throw std::runtime_error("price function unknown input parameters: only pass EuropeanOption objects");
       }
}

float BlackScholes::impliedVolatility(const Option& option, float S, float rate, float tmt, float marketPrice, RootFindingAlgo algo){
       if (marketPrice == 0){
              marketPrice = option.getPrice();
       }

       if (tmt == -1.0) {
              tmt = option.time2maturity();
       }

       auto func = [&option,S,tmt,rate,marketPrice](float x){return BlackScholes::price(option,S,x,rate,tmt)-marketPrice;};

       float impliedVol;
       switch (algo) {
              case BisectionMethod:
                     impliedVol = Math::bisectionMethod<float>(func,0.0,1.0,0.0001);
                     break;
              case NewtonRaphson:
                     impliedVol = newton_raphson_method(option, S, marketPrice, rate, tmt);
                     break;
              default:
                     impliedVol = 0.0;
                     std::cerr << "unknown method for computing volatility\n";
       }
       return impliedVol;
}

std::vector<float> BlackScholes::calibrate(const OptionChain* chain, float S, float rate){
       std::vector<float> impliedVol;
       float tmt = chain->getTime2Maturity();

       for (const auto& option : chain->getOptions()){
              float marketPrice = option->getPrice();
              float implied = BlackScholes::impliedVolatility(*option,S,rate,tmt,marketPrice);
              impliedVol.push_back(implied);
       }
       return impliedVol;
}

float delta(const Option& option, float S, float sigma, float rate, float tmt=-1.0){
       if (tmt == -1.0){
              tmt = option.time2maturity();
       }

       float d1 = BlackScholes::d1(option,S,sigma,rate,tmt);

       // Why does this work?
       if (const EuropeanCallOption* callOption = dynamic_cast<const EuropeanCallOption*>(&option)){
              return Math::normalCDF(d1);
       }else if (const EuropeanPutOption* putOption = dynamic_cast<const EuropeanPutOption*>(&option)){
              return Math::normalCDF(d1)-1;
       }else{
              throw std::runtime_error("price function unknown input parameters: only pass EuropeanOption objects");
       }
}

float gamma(const Option& option, float S, float sigma, float rate, float tmt=0.0){
       if (tmt == 0.0){
              tmt = option.time2maturity();
       }

       float d1 = BlackScholes::d1(option,S,sigma,rate,tmt);
       return Math::normalPDF(d1)/(S*sigma*std::sqrt(tmt));
}

float rho(const Option& option, float S, float sigma, float rate, float tmt=0.0){
       if (tmt == 0.0){
              tmt = option.time2maturity();
       }

       float d2 = BlackScholes::d2(option,S,sigma,rate,tmt);
       if (option.getType() == Call){
              return tmt*std::exp(-rate*tmt)*Math::normalCDF(d2);
       }else{
              return -tmt*std::exp(-rate*tmt)*Math::normalCDF(-d2);
       }
}

float theta(const Option& option, float S, float sigma, float rate, float tmt=0.0){
       if (tmt == 0.0){
              tmt = option.time2maturity();
       }

       float d1 = BlackScholes::d1(option,S,sigma,rate,tmt);
       float d2 = BlackScholes::d2(option,S,sigma,rate,tmt);
       if (option.getType() == Call){
              return -S*Math::normalPDF(d1)*sigma/(2*std::sqrt(tmt))-rate*S*Math::normalCDF(d1)*std::exp(-rate*tmt);
       }else{
              return -S*Math::normalPDF(d1)*sigma/(2*std::sqrt(tmt))+rate*S*Math::normalCDF(-d1)*std::exp(-rate*tmt);
       }
}

float vega(const Option& option, float S, float sigma, float rate, float tmt=0.0){
       if (tmt == 0.0){
              tmt = option.time2maturity();
       }

       const float d1 = BlackScholes::d1(option,S,sigma,rate,tmt);
       return S*Math::normalPDF(d1)*std::sqrt(tmt);
}