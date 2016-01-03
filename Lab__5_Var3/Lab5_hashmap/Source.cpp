#include "HashMap.h"
using namespace std;

int main() 
{
	string first = "abcdefg hijk";
	string second = "abcdehp hijk";
	string third = "";
	string first1 = "abcdefg dddhijk";
	string second2 = "abcdehp hijssssk";
	string third3 = "hjiushjhgjiygo";
	
	HashMap myMap;

	myMap.push(first, 23);
	myMap.push(second, 45);
	myMap.push(first, 23);
	myMap.push(third, 77);
	myMap.push(second2, 45);
	myMap.push(first1, 23);
	myMap.push(third3, 77);

	cout << myMap.find(first) <<endl;
	myMap.showMember(first);
	cout << myMap.find(third) << endl;
	myMap.showMember(third);
	cout << myMap.find(second) << endl;
	myMap.showMember(second2);
	myMap.showMember(second);
	myMap.pop(second2);
	cout << myMap.find(second) << endl;

	system("pause");
	return 0;
}