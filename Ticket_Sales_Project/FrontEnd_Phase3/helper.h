#pragma once
#include <string>

using namespace std;

class helper{
public:
	static string pad(string s, int length, string p, bool front);
	static string dtom(double amount);
};
