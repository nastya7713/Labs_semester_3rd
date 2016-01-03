#include "Gauss.h"

Gauss::Gauss(ifstream &input)
{
	input >> n;
	for (int i = 0; i < n; i++) b.push_back(0);
	for (int i = 0; i < n; i++) a.push_back(b);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			input >> a[i][j];
		}
		input >> b[i];
	}

	for (int j = 0; j < n; j++)
	{
		
		for (int i = 0; i < n; i++)
		{
			cout << a[j][i] << " ";
		}
		cout << b[j];
		cout << endl;
	}

	cout << "Enter the number of threads: " << endl;
	cin >> thread_num;
}


Gauss::~Gauss()
{
	a.~vector();
	b.clear();
}

void Gauss::part(int k, int p)
{
	int n = b.size();

	double coeff;
	if (a[p][p])
	{
		coeff = a[k][p] / a[p][p];//counts needed coefficient of equation
		b[k] = b[k] - coeff * b[p];//subtracts the current row free equation multiplied counted coefficient
	}

	for (int i = p; i < (int)b.size(); i++)
		a[k][i] = a[k][i] - coeff * a[p][i];//reduces matrix to an upper triangular form subtracting the current row multiplied a coefficient
}
void threadFunc(vector<vector<double>> &a, vector<double> &b, int k, int p)//the function to perform multithreading
{
	int n = b.size();

	double coeff;
	if (a[p][p])
	{
		coeff = a[k][p] / a[p][p];//counts needed coefficient of equation
		b[k] = b[k] - coeff * b[p];//subtracts the current row free equation multiplied counted coefficient
	}

	for (int i = p; i < (int)b.size(); i++)
		a[k][i] = a[k][i] - coeff * a[p][i];//reduces matrix to an upper triangular form subtracting the current row multiplied a coefficient
}

void Gauss::Multithread()//uses multithreading to implement the front movement of Gauss elimination
{
	if (thread_num == 1)
			front();	
	else
	{
			int row_need = n < thread_num ? n : thread_num;//counts the number of rows required
			row_need--;
			thread* array = new thread[row_need*n];//the array of threads

			int k = 0, j = 0, i = 0;

			for (i = 0; i < row_need; ++i)
			{
				for (j = i + 1; j < n; ++j)
				{
					array[k++] = thread(threadFunc, ref(a), ref(b), j, i);//creates threads to perform the front movement of Gauss elimination
				}
			}

			for (int i = 0; i < k; i++)
			if (array[i].joinable())
			{
				array[i].join();
			}

			while (j < n)//counts the number required to finish the solvement
			{
				part(j, i);
				j++;
			}

			for (int l = i; l < n; ++l)//performs solvement
			{
				for (j = l + 1; j < n; ++j)
				{
					part(j, l);
				}
			}
	}
}

void Gauss::front()//the front movement of Gauss elimination
{
	vector<vector<double>> left;
	left.assign(n, vector<double>(n, 0));

	for (int k = 0; k < n - 1; k++)
	{
		for (int i = k + 1; i < n; i++)
		{
			if (a[k][k] != 0)
			{
				left[i][k] = a[i][k] / a[k][k];//counts coefficients of equation
				b[i] = b[i] - left[i][k] * b[k];//subtracts the current row free equation multiplied counted coefficient
			}
		}
		for (int j = k + 1; j < n; j++)
		{
			for (int i = 0; i < n; i++)
				a[j][i] = a[j][i] - left[j][k] * a[k][i];//reduces matrix to an upper triangular form subtracting the current row multiplied a coefficient
		}
	}
	left.clear();
}
void Gauss:: Show()
{
	for (int j = 0; j < n; j++)
	{

		for (int i = 0; i < n; i++)
		{
			cout << a[j][i] << " ";
		}
		cout << b[j];
		cout << endl;
	}
}
void Gauss::multiSolve()
{
	Multithread();//uses multithreading to implement the front movement of Gauss elimination
	inverse();//the inverse movement of Gauss elimination
}
void Gauss:: inverse()//the inverse movement of Gauss elimination
{
	vector<double> x(n, 0);
	int n = a.size(), k = n- 1;

	Show();

	x[k] = b[k] / a[k][k];//the last variable of the system
	for (int i = n - 2; i >= 0; i--)//performs the expression of other variables
	{
		x[i] = b[i];//makes x[i] equal to the free coefficient
			for (int j = i + 1; j < n; j++)
			{
				x[i] = x[i] - a[i][j] * x[j];//using the variables counted before to find x[i] * coefficient
			}
		x[i] = x[i] / a[i][i];//divides x[i] coefficient
	}

	for (int i = 0; i < n; i++)
		cout << x[i] << " " << endl;

	x.clear();
}