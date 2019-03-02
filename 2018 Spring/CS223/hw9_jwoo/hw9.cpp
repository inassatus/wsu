#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

struct jobs {
	int id;
	string whatjob;
	int npro;
	int ntick;
};

struct jobtimer {
	jobs running;
	int tickremain;
};

struct compare {
	bool operator()(const jobs& l, const jobs& r) {
		return l.ntick > r.ntick;
	}

};

class sched {
public:
	sched() {
		p = 12;
		freep = p;
	}
	sched(int a) {
		p = a;
		freep = p;
	}
	void insertj(string a, int b, int c) {
		if ((b > p) + (b < 1) + (c < 1)) {
			cout << "not applicable job" << endl;
			return;
		}
		jobs temp;
		temp.id = njobs;
		temp.whatjob = a;
		temp.npro = b;
		temp.ntick = c;
		jlist.push(temp);
		njobs++;
		decrement();
		runjob();
	}
	bool isavailable() {
		if (jlist.empty()) return false;
		return (freep > jlist.top().npro)+(freep==jlist.top().npro);
	}
	void runjob() {
		if (isavailable()) {
			jobtimer temp;
			temp.running = jlist.top();
			temp.tickremain = jlist.top().ntick;
			occupiedprocessor.push_back(temp);
			nprocessoroccupied++;
			freep = freep - jlist.top().npro;
			cout << jlist.top().whatjob << " is now proceeding, ";
			cout << "ID: " << jlist.top().id << " needed processor: " << jlist.top().npro << " needed tick: " << jlist.top().ntick << endl;
			jlist.pop();
		}
		decrement();
		return;
	}
	void decrement() {
		static int ctick = 0;
		cout << "current tick: tick " << ctick << endl;
		for (int i = 0; i < nprocessoroccupied; i++) {
			occupiedprocessor[i].tickremain--;
			if (occupiedprocessor[i].tickremain<1)
			{
				freep += occupiedprocessor[i].running.npro;
				cout << occupiedprocessor[i].running.whatjob << " is now done, ";
				cout << "ID: " << occupiedprocessor[i].running.id << " needed processor: " << occupiedprocessor[i].running.npro << " needed tick: " << occupiedprocessor[i].running.ntick << endl;
				occupiedprocessor.erase(occupiedprocessor.begin() + i);
				nprocessoroccupied--;
				i--;
			}
		}
		ctick++;
		return;
	}
	void doalljobs() {
		while (!jlist.empty()+!occupiedprocessor.empty()) {
			runjob();
		}
	}

private:
	priority_queue<jobs, vector<jobs>, compare> jlist;
	int p;
	int freep;
	int njobs = 0;
	vector<jobtimer> occupiedprocessor;
	int nprocessoroccupied=0;
};

int main() {
	sched a;
	a.insertj("j1", 8, 10);
	a.insertj("j2", 2, 1);
	a.insertj("j3", 12, 12);
	a.insertj("j4", 10, 2);
	a.insertj("j5", 5, 8);
	a.insertj("j6", 4, 2);
	a.insertj("j7", 4, 6);
	a.insertj("j8", 2, 5);
	a.insertj("j9", 4, 3);
	a.insertj("j10", 6, 2);
	a.doalljobs();
	system("pause");
	return 0;
}