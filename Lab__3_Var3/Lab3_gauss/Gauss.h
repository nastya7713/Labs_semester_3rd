#pragma once
#include <vector>
#include <thread>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

class Gauss
{
	vector<vector<double>> a;//stores equation
	vector<double> b;//stores free coefficients of equation
	int n;
	int thread_num;

	void Multithread();//uses multithreading to implement the front movement of Gauss elimination
	void part(int k, int p);//the function which is responsible for partial realization of the front movement of Gauss elimination 
	void inverse();//the inverse movement of Gauss elimination
	void front();//the front movement of Gauss elimination

public:
	Gauss(ifstream& input);
	void multiSolve();//multithreading function to implement Gauss elimination
	void Show();
	~Gauss();
};

