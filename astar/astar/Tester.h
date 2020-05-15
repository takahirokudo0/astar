#pragma once

#include <functional>

class TestCase {
public:
	std::function<bool(void)> check;
	TestCase() {}
	TestCase(std::function<bool(void)> check) :check(check) {}
};

class Tester {
public:
	void Run();
private:
	bool Test(const TestCase& testcase);
	void DisplayResult(int number, int max_count, bool result, bool is_last);
};
