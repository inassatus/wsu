//it runs the game with vector

#ifndef __LISTMyJosephus_H__
#define __LISTMyJosephus_H__

#include <iostream>
#include <vector>
#include <string>
#include "Person.h"
#include <ctime>

using namespace std;

class vectorj {

public:
	vectorj();
	vectorj(int n, int m);
	~vectorj();
	void init();
	void process();
	void printall();
private:
	vector<Person> p;
	int n;		
	int m;
	string before = "";
	double time;
};



#endif