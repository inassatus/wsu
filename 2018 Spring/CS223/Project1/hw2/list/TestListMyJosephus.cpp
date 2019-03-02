#include <iostream>
#include <string>
#include <ctime>
#include "ListMyJosephus.h"
#include "Person.h"

using namespace std;

int main() 
{
	clock_t start = clock();
	listj a(16,3);
	a.process();
	a.printall();
	clock_t end = clock();
	cout << "CPU elapsed time in seconds: " << (double)(end - start) / CLOCKS_PER_SEC << endl;
	system("pause");
	return 0;
}