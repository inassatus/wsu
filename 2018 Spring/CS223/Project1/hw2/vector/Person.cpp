#include "Person.h"
#include <iostream>
using namespace std;


Person::Person(int a)
{
	position = a;
};
void Person::print() {
	cout << position;
}
Person::~Person() {}

int Person::getp() {
	return position;
}
