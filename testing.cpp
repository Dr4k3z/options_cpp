//
// Created by matte on 12/8/2024.
//

/*
	this file runs unit tests
*/
#include "unit_tests/test_framework.h"
#include "headers/rapidcsv.h"
#include <algorithm>
#include <iterator>
#include <exception>
#include "headers/options.h"
#include "headers/black_scholes.h"


float TestingFramework::string2float(std::string str){
	try{
		std::remove(str.begin(),str.end(),',');

		if (str == "-"){
			return 0;
		}

		return std::stof(str);
	}catch (std::exception& e){
		std::cerr << e.what() << "\n";
		std::string errmsg = "Error with the following string:  "+str+"\n";
		throw std::runtime_error(errmsg);
	}
}

std::vector<float> TestingFramework::implied_vols(std::string filename) {
	try {
		rapidcsv::Document doc(filename);
		std::vector<std::string> doc_vols = doc.GetColumn<std::string>("IV");

		auto func = [](std::string str){ return string2float(str); };
		std::vector<float> vols;
		std::transform(doc_vols.begin(),doc_vols.end(),std::back_inserter(vols),func);

		return vols;
	}catch (std::exception& e) {
		std::cerr << e.what() << "\n";
		throw std::runtime_error("Does the file exists?");
	}
}

int main() {
	Date value_date = Date::create(2024,7,31);
	Date expiry_date = Date::create(2024,8,1);
	Calendar calendar = Calendar::noHolidays();

	std::string filename = "../../assets/NIFTY_put_data_1_aug_2024.csv";
	OptionChain* chain = OptionChain::createFromCsv(value_date,expiry_date,calendar,European,Put,filename);

	float S = 24950.0; // NIFTY value at the 2024-7-31
	float rate = 0.1; // Risk-free rate

	auto vols = BlackScholes::calibrate(chain,S,rate);
	auto test_vols = TestingFramework::implied_vols("../../assets/NIFTY_put_data_1_aug_2024.csv");

	assert(vols.size() == test_vols.size() && "Error! the vols and test_vols vectors have different sizes");

	TestRunner runner;
	float tol = 1.2;
	for (size_t i=0;i<vols.size();i++) {
		std::string test_name = "Test Case "+std::to_string(i);
		runner.addTest(test_name,[vols,test_vols,i,tol]() {
			ASSERT_EQUAL(100*vols[i],test_vols[i],tol);
		});
	}

	runner.run();
}