#include "VectorMyJosephus.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

int main() 
{
	clock_t start = clock();
	vectorj a(1024*2, 3);
	a.process();
	a.printall();
	clock_t end = clock();
	cout << "CPU elapsed time in seconds: "<<(double)(end - start)/CLOCKS_PER_SEC <<endl;
	system("pause");
	return 0;
}