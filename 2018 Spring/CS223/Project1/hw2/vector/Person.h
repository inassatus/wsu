
#ifndef __PERSON_H__
#define __PERSON_H__


class Person {

public:
	Person(int a);
	~Person();

	void print();
	int getp();

private:
	int position;
};

#endif