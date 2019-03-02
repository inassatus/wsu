#include "ListMyJosephus.h"
#include <list>
#include <iostream>
#include <ctime>

using namespace std;

listj::listj()
{
	n = 5;//default n=5, m=1
	m = 1;
	init();
}

listj::listj(int a, int b) {
	n = a;
	m = b;
	init();
}

listj::~listj() {}

void listj::init() {
	for (int i = 0; i < n; i++) {
		Person a(i);
		p.push_back(a);
	}
}

void listj::process() {
	if (n < 2) { return; }
	list<Person>::iterator place = p.begin();
	int eliminated = 0;
	int t = 0;
	double T = 0.00000000;
	int i = 0;
	int j = n - 1;
	while (1) {
		clock_t start = clock();
		if (n - eliminated == 1) { 
			time = T / t;
			return; 
		}
		for (int k = 0; k < m; k++) {
			if (i > j) { i = 0; place = p.begin(); }
			i++;
			place++;
			if (i > j) { i = 0; place = p.begin(); }
		}
		before += to_string(place->getp());
		before += " ";
		cout << place->getp() << " is eliminated" << endl;
		p.erase(place);
		place = p.begin();
		for (int z = 0; z < i; z++) { place++; }
		eliminated++;
		j--;
		clock_t end = clock();
		T = T + (double)(end - start) / CLOCKS_PER_SEC;
		t++;
	}	
}

void listj::printall() {
	
	cout << "Eliminated: " << before << endl;
	cout << "Winner: ";
	cout<< p.begin()->getp() << endl;
	cout << "Average time between elimination: " << time << endl;
}
