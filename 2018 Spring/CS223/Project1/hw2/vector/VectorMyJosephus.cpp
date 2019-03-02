#include <iostream>
#include <vector>
#include "Person.h"
#include "VectorMyJosephus.h"
#include <ctime>
using namespace std;

vectorj::vectorj() {
	n = 5;//default n=5, m=1
	m = 1;
	init();
}
vectorj::vectorj(int a, int b) {
	n = a;
	m = b;
	init();
}
vectorj::~vectorj() {}

void vectorj::init() {
	for (int i = 0; i < n; i++) {
		Person a(i);
		p.push_back(a);
	}
}

void vectorj::process() {
	int eliminated = 0;
	int t = 0;
	double T = 0.00000000;
	int i = 0;
	int j = n-1;
	if (n < 2) { return; }
	while (1) {
		clock_t start = clock();
		if (n - eliminated == 1) {
			time = T / t;
			return; }
		for (int k = 0; k < m; k++) {
			if (i > j) { i = 0; }
			i++;
			if (i > j) { i = 0; }
		}
		before += to_string(p[i].getp());
		before += " ";
		cout << p[i].getp() << " is eliminated" << endl;
		p.erase(p.begin() + i);
		eliminated++;
		j--;
		clock_t end = clock();
		T = T + (double)(end - start) / CLOCKS_PER_SEC;
		t++;
	}
}

void vectorj::printall() {
	cout << "Eliminated: " << before << endl;
	cout << "Winner: " << p[0].getp() << endl;
	cout << "Average time between elimination: " << time << endl;
}