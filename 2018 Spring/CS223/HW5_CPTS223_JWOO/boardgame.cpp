#include <iostream>
#include <vector>
#include <string>
using namespace std;

class player {
public:
	player() {
	}
	~player() {}
	void setid(int a) {
		id = a;
	}
	void setplace(int a, int b) {
		r = a;
		c = b;
	}
	int getrow() {
		return r;
	}
	int getcol() {
		return c;
	}
	int getid() {
		return id;
	}
	bool isonb;
private:
	int r;
	int c;
	int id;
};

class board {
public:
	board() {
		m = 5;
		n = 0;
		init();
	}
	board(int a) {
		n = 0;
		m = a;
		init();
	}
	~board() {}
	void init() {
		for (int i = 0; i < m*m; i++) 
		{
			ontheboard.push_back(&non);
		}
	}//creates a board

	int access(int r, int c) {
		return (m*(r - 1)) + (c - 1);
	}//since the board is linear, we need this function to access a location as coordinate(x,y)

	void put(int r, int c){
		if (n > m) {
			cout << "there are already maximum players" << endl;
			return;
		}
		if(*ontheboard[access(r,c)]){
			cout << "the place is already occupied" << endl;
			return;
		}
		if ((r < 1) + (r > m) + (c > m) + (c < 1)) {
			cout << "you can not put your piece out of the board" << endl;
			return;
		}
		player newp;
		pofgame.push_back(newp);//new player added
		pofgame[n].setid(n);//player's id is the same as the number of current players
		pofgame[n].setplace(r, c);//record the place
		ontheboard[access(r, c)] = &pofgame[n].isonb;//the player is on the board now
		*ontheboard[access(r, c)] = 1;
		n++;
		return;
	}

	void move(int id, int x, int y) {//this function move the piece
		if (id < 0) {
			cout << "invliad id" << endl;
			return;
		}
		else if (id > n) {
			cout << "invalid id" << endl;
			return;
		}
		if (!(pofgame[id].isonb)) {
			cout << "your piece was eliminated from the board" << endl;
			return;
		}
		if ((x > 1) + (x < -1) + (y > 1) + (y < -1)) {
			cout << "you can only move one" << endl;
		}
		bool isok= 1;
		if ((pofgame[id].getcol() < 2)*(x < 0)) {
			cout << "your piece is at the left boundary so can not move to the left" << endl;
			isok = 0;
		}
		if ((pofgame[id].getcol() > m-1)*(x > 0)) {
			cout << "your piece is at the right boundary so can not move to the right" << endl;
			isok = 0;
		}
		if ((pofgame[id].getrow() < 2)*(y > 0)) {
			cout << "your piece is at the top boundary so can not move to upside" << endl;
			isok = 0;
		}
		if ((pofgame[id].getrow() > m - 1)*(y < 0)) {
			cout << "your piece is at the bottom boundary so can not move to downside" << endl;
			isok = 0;
		}
		if (!isok) { return; }//in the case that a piece is at two boundaries
		
		*ontheboard[access(pofgame[id].getrow(), pofgame[id].getcol())] = 0;
		ontheboard[access(pofgame[id].getrow(), pofgame[id].getcol())] = &non;
		if (*ontheboard[access(pofgame[id].getrow() - y, pofgame[id].getcol() + x)]) {//if there is another piece on the destination
			*ontheboard[access(pofgame[id].getrow() - y, pofgame[id].getcol() + x)] = 0;
			cout << "enemy is eliminated" << endl;
		}//this is removal process. it does not have to be separate function, so I put it in move function
		ontheboard[access(pofgame[id].getrow() - y, pofgame[id].getcol() + x)] = &pofgame[id].isonb;
		pofgame[id].setplace(pofgame[id].getrow() - y, pofgame[id].getcol() + x);
		pofgame[id].isonb = 1; //the piece is moved
		return;
	}
	void printall() {
		for (int i = 0; i < n; i++) {
			cout << "ID: " << pofgame[i].getid() << endl;
			cout << "r: " << pofgame[i].getrow() << endl;
			cout << "c: " << pofgame[i].getcol() << endl;
			cout << endl;
		}
		return;
	}

private:
	int n;
	int m;
	vector<bool*> ontheboard;
	vector<player> pofgame;
	bool non = 0;//this is pointed to false for always
};



int main()//my programming structure is not right for the given testboard, so I changed the code. 
{
	board a(1000);
	a.put(3, 2);//ID of each player is automatically generated
	a.put(4, 3);
	a.put(4, 6);
	a.put(5, 7);
	a.put(6, 3);
	a.put(8, 2);
	a.put(9, 3);
	a.put(9, 8);
	
	a.put(4, 6);
	a.put(9, 8);
	a.move(4, 1, -1);//can move only one space in every side
	a.put(7, 4);
	a.printall();

	system("pause");
	return 0;
}