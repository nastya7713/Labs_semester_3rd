#include "Gauss.h"

int main()
{
	ifstream input("Text.txt");//the file which stores equations
	Gauss expression(input);
	expression.multiSolve();//solves equations
	system("pause");
	return 0;
}