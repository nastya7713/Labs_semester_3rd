#pragma once
#include "eig_matrix.h"
#include <fstream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#define precision 100000
#define eps 0.0000001

class Jordan
{
	vector<vector<double>> matrix;
	vector<vector<double>> jordanForm;
	vector<double> eigenValues;
	vector<vector<double>> eigenVectors;
	struct lmdvalue
	{
		double value;
		int multiplicity;//stores multiplicity of eigenvalue
		lmdvalue(int val, int mul) : value(val), multiplicity(mul)
		{}
	};

	int Size;

	int rank_power(vector<vector<double>>& A, int k);
	void mswap(vector<vector<double>>& matr, int row1, int row2, int col);
	int rankOfMatrix(vector<vector<double>> matr);
	void formMatrix(vector<vector<double>>& matr, int num);
	void createJf(vector<vector<double>> jcell, vector<lmdvalue> count);
	void mult_pow(vector<vector<double>>& matr, int k);
	void showJordan();

public:
	Jordan(string filename);
	Jordan(vector<vector<double>> &A);
	~Jordan();
	void ShowMatr();
	void JordanForm();
};

