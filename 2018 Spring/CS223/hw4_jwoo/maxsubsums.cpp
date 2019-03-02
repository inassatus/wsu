#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

double maxsubsum1(vector<double> &a);
double maxsubsum2(vector<double> &a);
double maxsubsum3(vector<double> &a);
double getmaxsum3(vector<double> &a, int, int);
double max3(double, double, double);
double maxsubsum4(vector<double> &a);
void testrandom(int);
void getfileandgo(string);
void now();

int main()
{
	srand(time(0));
	string input = "input_8_0.txt";
	//could take user's input in order of arraysize, number of file, and then convert the input into string,  and then add ".txt" to the string
	
	//getfileandgo(input);

	testrandom(8);
	testrandom(16);
	testrandom(32);
	testrandom(64);
	testrandom(128);
	testrandom(256);
	testrandom(512);
	testrandom(1024);
	testrandom(2048);
	//could get maxsubsum1 untill here
	//testrandom(4096); could not get maxsubsum 1, and 2


	system("pause");
	return 0;
}

void testrandom(int a) {
	if (a < 1) return;
	vector<double> b(a);
	for (int i = 0; i < a; i++) b[i] = (rand() % 19) - 9;
	cout << "Array size: " << b.size() << endl;
	now(); cout << "maxsubsum4: " << maxsubsum4(b) << " "; now();
	now(); cout << "maxsubsum3: " << maxsubsum3(b) << " "; now();
	now(); cout << "maxsubsum2: " << maxsubsum2(b) << " "; now();
	now(); cout << "maxsubsum1: " << maxsubsum1(b) << " "; now();
	return;
}//generate random array for testing. This function is for me to test the assignment.

void getfileandgo(string a) {
	vector<double> b;
	ifstream get;
	get.open(a);
	if (get.fail()) {
		cerr << "error" << endl;
		exit(1);
	}
	double temp;
	while (!get.eof()) {
		get >> temp;
		b.push_back(temp);
	}
	b.pop_back();//the file does not end at the last item.
	get.close();
	cout << "Array size: " << b.size() << endl;
	now(); cout << "maxsubsum4: " << maxsubsum4(b) << " "; now();
	now(); cout << "maxsubsum3: " << maxsubsum3(b) << " "; now();
	now(); cout << "maxsubsum2: " << maxsubsum2(b) << " "; now();
	now(); cout << "maxsubsum1: " << maxsubsum1(b) << " "; now();
	return;
}

void now() {
	static bool called = 0;
	static auto start = chrono::high_resolution_clock::now();
	if (called) {
		auto end = chrono::high_resolution_clock::now();
		cout << "elapsed time in microseconds: " << chrono::duration_cast<chrono::microseconds>(end-start).count() << endl;
		called = 0;
		return;
	}
	start = chrono::high_resolution_clock::now();
	called = 1;
	return;
}//count the elapsed time

double maxsubsum1(vector<double> &a) {
	double maxsum = 0;
	for (int i = 0; i < a.size(); i++) {
		for (int j = 0; j < a.size(); j++) {
			double csum = 0;
			for (int k = i; k <= j; k++) { csum += a[k]; }
			if (csum > maxsum) maxsum = csum;
		}
	}
	return maxsum;
}


double maxsubsum2(vector<double> &a) {
	double maxsum = 0;
	for (int i = 0; i < a.size(); i++) {
		double csum = 0;
		for (int j = i; j < a.size(); j++) {
			csum += a[j];
			if (csum > maxsum) maxsum = csum;
		}
	}
	return maxsum;
}


double maxsubsum3(vector<double> &a) {
	return getmaxsum3(a, 0, a.size() - 1);
}
//recursive
double max3(double a, double b, double c)
{
	if ((a > b)*(a > c)) return a;
	else if ((b > a)*(b > c)) return b;
	else return c;
}//return biggest
double getmaxsum3(vector<double> &a, int left, int right) {
	int center = (left + right) / 2;
	if (left == right) return a[left] > 0 ? a[left] : 0;
	double maxleftsum = getmaxsum3(a, left, center);
	double maxrightsum = getmaxsum3(a, center + 1, right);
	double tmaxleft = 0;
	double maxleft = 0;
	double tmaxright = 0;
	double maxright = 0;
	for (int i = center; i >= left; i--) {
		maxleft += a[i];
		if (maxleft > tmaxleft) tmaxleft = maxleft;
	}
	for (int j = center + 1; j <= right; j++) {
		maxright += a[j];
		if (maxright > tmaxright) tmaxright = maxright;
	}
	return max3(maxleftsum, maxrightsum, tmaxleft + tmaxright);
}//find biggest from three way of sum


double maxsubsum4(vector<double> &a) {
	double csum = 0;
	double maxsum = 0;
	for (int i = 0; i < a.size(); i++) {
		csum += a[i];
		if (csum > maxsum) maxsum = csum;
		else if (csum < 0) csum = 0;
	}
	return maxsum;
}//skip the first for-loop. But use condition that will reset the current sum.
