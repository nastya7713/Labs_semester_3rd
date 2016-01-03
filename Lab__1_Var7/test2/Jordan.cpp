#include "Jordan.h"

struct myclass {
	bool operator() (double i, double j) { return (i<j); }
} compare;

Jordan::Jordan(string filename)
{
	fstream input(filename);
	input >> Size;
	matrix.assign(Size, vector<double>(Size, 0));
	
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			input >> matrix[i][j];
		}
	}
}

Jordan::Jordan(vector<vector<double>> &A)
{
	matrix.assign(Size, vector<double>(Size, 0));
	matrix = A;
}

void Jordan::ShowMatr()
{
	cout << "Given matrix: " << endl;
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			cout << setw(4) << matrix[i][j];
		}
		cout << endl;
	}
}

Jordan::~Jordan()
{
}

//performs involution and calculates rank
void Jordan::mult_pow(vector<vector<double>> &matr, int k)
{
	if (k == 0 || k < 0)
		return;

	vector<vector<double>> c;
	int n = matr.size();
	c.assign(n, vector<double>(n, 0));
	for (int i = 0; i<matr.size(); i++)
	{
		for (int j = 0; j<matr.size(); j++)
		{
			c[i][j] = 0;
			for (int k = 0; k< matr.size(); k++)
			{
				c[i][j] = c[i][j] + matr[i][k] * matr[k][j];
			}
		}
	}
	k--;
	matr = c;
	return mult_pow(matr, k);
}

int Jordan::rank_power(vector<vector<double>> &A, int k)//performs involution and calculates rank 
{
	int n = A.size();

	//case k = 0:
	//r^(k-1) - 2*r^(k) + r^(k+1) 
	// r^0 = n

	if (k == 0)
		return n;

	mult_pow(A, k-1);
	int rank = rankOfMatrix(A);
	return rank;
}

void Jordan::mswap(vector<vector<double>>& matr, int row1, int row2, int col)//swaps matrix rows
{
	for (int i = 0; i < col; i++)
	{
		double temp = matr[row1][i];
		matr[row1][i] = matr[row2][i];
		matr[row2][i] = temp;
	}
}
int Jordan::rankOfMatrix(vector<vector<double>> matr)//calculates rank of a matrix
{
	int rank = matr.size();
	//reduces to triangular
	for (int row = 0; row < rank; row++)
	{

		if (matr[row][row])
		{
			for (int col = 0; col < rank; col++)
			{
				if (col != row)
				{
					double mult = matr[col][row] /matr[row][row];
					for (int i = 0; i < rank; i++)
						matr[col][i] -= mult * matr[row][i];
				}
			}
		}

		else
		{
			bool reduce = true;

			for (int i = row + 1; i < rank; i++)
			{
				
				if (matr[i][row])//swaps zero rows
				{
					mswap(matr, row, i, rank);
					reduce = false;
					break;
				}
			}

			if (reduce)
			{
				rank--;

				for (int i = 0; i < rank; i++)
					matr[i][row] = matr[i][rank];
			}

			row--;
		}

	}
	return rank;
}
 
void Jordan::createJf(vector<vector<double>> jcell, vector<lmdvalue> count)
{
	vector<vector<double>> jf;
	int k(0);
	vector <lmdvalue> tmp = count;
	jf.assign(Size, vector<double>(Size, 0));
	//fulls matrix with eigenvalues
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			while (tmp[k].multiplicity <= 0)
				k++;
			if (i == j && tmp[k].multiplicity)
			{
				tmp[k].multiplicity--;
				jf[i][j] = tmp[k++].value;
			}

			if (k > 0)
			{
				if (tmp[k - 1].multiplicity)
				{
					k--;
					break;
				}
			}
		}
	}
	
	  k = 0;
	
	  //forms jordan cells
		for (int i = 0; i < Size; i++)
		{
			for (int j = 0; j < Size; j++)
			{
				if (j == i)
				{
						for (k = 0; k < Size; k++)
						{
							if (jf[i][j] == count[k].value)
								break;
						}
						
						for (int l = 1; l < Size; l++)
						if (jcell[k][l])
						{
								for (int t = k; t < l + 1; t++)
								for (int m = 0; m < l + k + 1; m++)
								if (t + 1 == m)
									jf[t][m] = 1;
								jcell[k][l] --;
						}
				}
			}
		}
		jordanForm = jf;
		showJordan();
}

void Jordan::showJordan()
{
	cout << "Jordan form: " << endl;
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			cout << setw(4) << jordanForm[i][j];
		}
		cout << endl;
	}
}

void Jordan::formMatrix(vector<vector<double>>& A, int num)//forms eigenvalue matrix
{
	A = matrix;

	for (int i = 0; i < A.size(); i++)
	for (int j = 0; j < A.size(); j++)
	if (i == j)A[i][j] -= num;
}

void Jordan:: JordanForm()
{
	eig_matrix eigen;
	eigen.dec(matrix);
	eigenValues = eigen.getD();
	
	cout << "EigenValues: ";
	for (int i = 0; i < Size; i++)
	{
		eigenValues[i] = round(eigenValues[i]);
		if (eigenValues[i] == 0)
			eigenValues[i] = 0;
		cout << eigenValues[i] << " ";
	}
	cout << endl;

	vector<double> tmp = eigenValues;

	vector<lmdvalue> count(Size, lmdvalue(0, 0));

	sort(tmp.begin(),tmp.end(), compare);
	int k(0);

	//calculates multiplicity of eigenvalues
	for (int i = 0; i < Size; i++)
	{
		k = i;
			count[i].value = tmp[i];
				count[i].multiplicity++;
			
		for (int j = i+1; j < Size; j++)
		if (tmp[i] == tmp[j] && tmp[j] && tmp[i])
		{
			count[i].multiplicity++;
			k++;
		}
		i = k;
	}

	vector<vector<double>> jcell;//stores number and size of jordan cells
	vector<vector<double>> A,D;
	jcell.assign(Size, vector<double>(Size, 0));

	/*calculates the number and size of jordan cells
	using formula r^(k-1) - 2*r^(k) + r^(k+1)
	where r^(k) = rank|A - lambda E|^(k)*/
	
	for (int i = 0; i < Size; i++)
	{
		if (count[i].multiplicity)
		{
			for (int j = 0; j < count[i].multiplicity; j++)
			{
				formMatrix(A, count[i].value);//form a matrix of eigenvalue
				D = A;
				double rank_k_1 = rank_power(D, count[i].multiplicity - j - 1);
				double rank_k = rank_power(D, count[i].multiplicity - j);
				double rank_kplus1 = rank_power(D, count[i].multiplicity - j + 1);
				int k = count[i].multiplicity - j - 1;
				//r^(k-1) - 2*r^(k) + r^(k+1)
				double rank_res = rank_k_1 - 2 * rank_k + rank_kplus1;
				jcell[i][k] = rank_res;
			}
		}
	}

	createJf(jcell, count);
}
