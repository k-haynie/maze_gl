#ifndef randint_h
#define randint_h 

#include <random>

using namespace std;

// custom random int generator
int randint(int high, int low = 0) {
	random_device rd;
	mt19937 generator(rd());
	uniform_int_distribution<int> distr(low, high);
	return distr(generator);
};

#endif