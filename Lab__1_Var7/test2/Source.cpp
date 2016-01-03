#include "Jordan.h"

int main()
{

	string filename = "matrix.txt";
	Jordan jf_matrix(filename);
	jf_matrix.ShowMatr();
	jf_matrix.JordanForm();
	system("pause");
	return 0;
}