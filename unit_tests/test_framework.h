//
// Created by matte on 12/8/2024.
//

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <cmath>
#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <stdexcept>
#include <fstream>
#include "../headers/date.h"

#define ASSERT_EQUAL(actual, expected, tolerance) \
	if (std::abs((actual) - (expected)) > (tolerance)) { \
		throw std::runtime_error("Assertion failed: " + std::to_string(actual) + " != " + std::to_string(expected)); \
	}

struct TestCase {
	std::string name;
	std::function<void()> testFunction;
};

class TestRunner {
private:
	std::vector<TestCase> testCases;
	int testsPassed = 0;
	int testsFailed = 0;

public:
	void addTest(const std::string& testName, const std::function<void()>& testFunction) {
		testCases.push_back({testName, testFunction});
	}

	void run(std::string filename = "") {
		if (filename.empty()) {
			Date d = Date::today();
			filename = "../../unit_tests/test_"+d.to_string()+".csv";
		}

		std::cout << filename << "\n";

		std::ofstream log_file;
		log_file.open(filename);

		for (const auto& testCase : testCases) {
			try {
				testCase.testFunction();
				std::cout << "[PASS] " << testCase.name << "\n";
				log_file << "[PASS] " << testCase.name << "\n";
				++testsPassed;
			} catch (const std::exception& e) {
				std::string msg = "[FAIL] " + testCase.name + " - Exception: " + e.what() + "\n";
				std::cout << msg;
				log_file << msg;
				++testsFailed;
			} catch (...) {
				std::string msg = "[FAIL] " + testCase.name + " - Unknown exception\n";
				std::cout << msg;
				log_file << msg;
				++testsFailed;
			}
		}

		std::cout << "\nTest Summary:\n";
		std::cout << "Passed: " << testsPassed << "\n";
		std::cout << "Failed: " << testsFailed << "\n";

		log_file << "\nTest Summary:\n";
		log_file << "Passed: " << testsPassed << "\n";
		log_file << "Failed: " << testsFailed << "\n";
		log_file.close();
	}
};

namespace TestingFramework {
	float string2float(std::string str);
	std::vector<float> implied_vols(std::string filename);
}

#endif