#include <iostream>
#include <vector>
#include <string>
#include "functional.h"
using namespace std;

bool odd(int n) { return 1 == n % 2; }

int square(int n) { return n*n; }

int toInt(string str) { return strtol(str.c_str(), nullptr, 10); }

template<class T> void print(T e) { cout << e << " "; }

int main() {
	vector<string> vec{ "1", "2", "3", "4", "5", "6", "7" };
	fp::chain(vec)
		.map(toInt)
		.map(square)
		.filter(odd)
		.foreach(print<int>);
	return 0;
}
