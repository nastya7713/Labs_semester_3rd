#include "HashMap.h"

member::member(int data, string str_key) : hashVal(data), key(str_key), next(nullptr){};
member::~member() {
}

const int member::getHash() {
	return hashVal;
}

member* member::getNext() {
	return next;
}

string member::getKey() {
	return key;
}

HashMap::HashMap() {
	table = new member*[SIZE];
	for (int i = 0; i < SIZE; i++)
		table[i] = NULL;
}
HashMap::~HashMap() {
	for (int i = 0; i < SIZE; i++)
	if (table[i] != NULL)
		delete table[i];
	delete[] table;
}

//the hashfunc which generates an index using string
unsigned const int HashMap::HashFunc(string _str) {
	unsigned int value = 1;

	for (unsigned int i = 0; i < _str.size(); i++) {
		value += (_str[i] + 1) * (i + 1);
		value %= 257;
	}
	return value;
}


const int HashMap::size() {
	return ElementsNumber;
}

bool HashMap::find(string findStr)//if the element is in a map, returns true. Otherwise false.
{
	auto str_hash = HashFunc(findStr);
	if ((int)str_hash > SIZE) return false;

	member* tmp = table[str_hash];
	while (tmp != NULL)
	{
		if (tmp->key == findStr)
			return true;

		tmp = tmp->next;
	}
	return false;
}

void HashMap::showMember(string str)
{	
	if (!find(str)) return;
	
	auto str_hash = HashFunc(str);
	
	member* tmp = table[str_hash];
	while (tmp != NULL)
	{
		if (tmp->key == str)
			break;

		tmp = tmp->next;
	}
	
	cout << " string: " << tmp->key << " str_hash: " << str_hash  << " value: " << tmp->hashVal << endl;
}

//adds member to the map
void HashMap::push(string addStr, unsigned long int val) {
	
	auto str_hash = HashFunc(addStr);

	if (!table[str_hash])//if no collisions had occured before, creates a new member
		table[str_hash] = new member(val, addStr);
	
	else {
		//adds it to end of the list 
		member *entry = table[str_hash];
		while (entry->next != NULL)
		{
			if (entry->key == addStr)
			{
				entry->hashVal = val;
				return;
			}
			entry = entry->next;
	
		}
		entry->next = new member(val, addStr);
	}
	ElementsNumber++;//increments the number of elements
}

bool HashMap::pop(string delStr)//deletes a member
{	
	auto str_hash = HashFunc(delStr);
	if (!find(delStr)) return false;//if it does not exist, returns false

	ElementsNumber--;//decrements the number of elements
	if (!table[str_hash]->next)//if there is an only one member, just delete. 
	{
		member*tmp = table[str_hash];
		table[str_hash] = NULL;
		delete tmp;
		return true;
	}
	member*curr = table[str_hash];
	member* prev = NULL;

	//implements deleting of element from a list
	while (curr)
	{
		if (curr->key == delStr)
			break;
		prev = curr;
		curr = curr->next;
	}
	if (prev->next == NULL)
		return false;

	member*tmp = curr;
	prev->next = curr->next;
	delete curr;
	return true;
}
	
