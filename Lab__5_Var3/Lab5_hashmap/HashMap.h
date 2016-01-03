#include <string>
#include <vector>
#include <iostream>
#include <list>
using namespace std;

//class which represents a member
class member {

	friend class HashMap;
	unsigned long int hashVal;
	string key;
	member* next;

public:
	member(int, string);
	~member();

	const int getHash();
	member* getNext();
	string getKey();

};

class HashMap {

	member** table;
	int ElementsNumber;
	int SIZE = 256;
	unsigned const int HashFunc(string _str);
	void resize(int SIZE);

public:
	HashMap();
	~HashMap();

	const int size();
	void push(string addStr, unsigned long int val);//adds member to map
	bool find(string findStr);//looking for a member if it exists , returns true
	bool pop(string delStr);//deletes a member of map
	void showMember(string str);

};