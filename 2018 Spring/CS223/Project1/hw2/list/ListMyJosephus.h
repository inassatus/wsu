//this process the game with lists

#ifndef __LISTMyJosephus_H__
#define __LISTMyJosephus_H__

#include <iostream>
#include <list>
#include "Person.h"
#include <string>

using namespace std;

class listj {

public:

	listj();
	listj(int a, int b);
	~listj();
	void init();
	void process();
	void printall();

private:
	list<Person> p;
	int n;		
	int m;
	string before ="";
	double time;
};



#endif