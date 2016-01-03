#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class eig_matrix
{
public:
	eig_matrix();
	~eig_matrix();
	void dec(vector<vector<double>> &A);
	vector<double> getD();
	vector<vector<double>> getV();
	bool isComplex(double tol);
	void showEigVect();
	void showEigVal();

private:

	int n;
	bool symmetric;
	bool isSymmetric();
	void tred2();
	void tql2();
	void others();
	void hqr2();


	vector<double> ort;
	vector<vector<double>> H;
	vector<vector<double>> V;
	vector<double> d;
	vector<double> e;

};