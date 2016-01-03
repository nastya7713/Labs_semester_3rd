#include "eig_matrix.h"

eig_matrix::eig_matrix() : symmetric(true)
{

}

eig_matrix::~eig_matrix()
{

}


vector<double> eig_matrix::getD() 
{
	return d;
}

vector<vector<double>> eig_matrix::getV() 
{
	return V;
}

void eig_matrix::showEigVal()
{
	cout << "The eigenvalue D : " << endl;
	for (vector<double>::iterator itr = d.begin(); itr != d.end(); itr++)
		cout << *itr << " ";
	cout << endl;
}

void eig_matrix::showEigVect()
{
	cout << "The eigenvectors matrix : " << endl;
	for (vector<vector<double>>::iterator itr = V.begin(); itr != V.end(); itr++)
	{
		for (vector<double>::iterator it = (*itr).begin(); it != (*itr).end(); ++it)
			cout << *it << " ";
		cout << endl;
	}
}

bool eig_matrix::isComplex(double tol)
{
	if (symmetric)
		return false;

	double crt = 0;
	for (int i = 0; i<n; ++i)
		crt += abs(e[i]);

	if (crt > tol)
		return true;
	else
		return false;
}



void eig_matrix::dec(vector<vector<double>> &A)
{
	n = A.size();
	V.assign(n, vector<double>(n, 0));
	d.assign(n, 0);
	e.assign(n, 0);

	symmetric = true;
	for (int j = 0; (j<n) && symmetric; ++j)
	for (int i = 0; (i<n) && symmetric; ++i)
		symmetric = (A[i][j] == A[j][i]);

	if (symmetric)
	{
		for (int i = 0; i<n; ++i)
		for (int j = 0; j<n; ++j)
			V[i][j] = A[i][j];

		// tridiagonalize.
		tred2();

		// diagonalize.
		tql2();

	}
	else
	{
		H.assign(n, vector<double>(n, 0));
		ort.assign(n, 0);

		for (int j = 0; j < n; ++j)
		for (int i = 0; i < n; ++i)
			H[i][j] = A[i][j];


		// reduce to Hessenberg form
		others();

		// reduce Hessenberg to real Schur form
		hqr2();
	}

}




bool eig_matrix::isSymmetric()
{
	return symmetric;
}


void eig_matrix::tred2()
{
	for (int j = 0; j<n; ++j)
		d[j] = V[n - 1][j];

	// Householder reduction to tridiagonal form
	for (int i = n - 1; i>0; --i)
	{
		// scale to avoid under/overflow
		double scale = 0;
		double h = 0;

		for (int k = 0; k<i; ++k)
			scale += abs(d[k]);

		if (scale == 0)
		{
			e[i] = d[i - 1];
			for (int j = 0; j<i; ++j)
			{
				d[j] = V[i - 1][j];
				V[i][j] = 0;
				V[j][i] = 0;
			}
		}
		else
		{
			// generate Householder vector
			for (int k = 0; k<i; ++k)
			{
				d[k] /= scale;
				h += d[k] * d[k];
			}

			double f = d[i - 1];
			double g = sqrt(h);
			if (f > 0)
				g = -g;

			e[i] = scale * g;
			h = h - f * g;
			d[i - 1] = f - g;

			for (int j = 0; j<i; ++j)
				e[j] = 0;

			// Apply similarity transformation to remaining columns.
			for (int j = 0; j<i; ++j)
			{
				f = d[j];
				V[j][i] = f;
				g = e[j] + V[j][j] * f;

				for (int k = j + 1; k <= i - 1; ++k)
				{
					g += V[k][j] * d[k];
					e[k] += V[k][j] * f;
				}
				e[j] = g;
			}

			f = 0;
			for (int j = 0; j<i; ++j)
			{
				e[j] /= h;
				f += e[j] * d[j];
			}

			double hh = f / (h + h);
			for (int j = 0; j<i; ++j)
				e[j] -= hh * d[j];

			for (int j = 0; j<i; ++j)
			{
				f = d[j];
				g = e[j];
				for (int k = j; k <= i - 1; ++k)
					V[k][j] -= (f*e[k] + g*d[k]);

				d[j] = V[i - 1][j];
				V[i][j] = 0;
			}
		}
		d[i] = h;
	}

	// accumulate transformations
	for (int i = 0; i<n - 1; i++)
	{
		V[n - 1][i] = V[i][i];
		V[i][i] = 1;
		double h = d[i + 1];

		if (h != 0)
		{
			for (int k = 0; k <= i; ++k)
				d[k] = V[k][i + 1] / h;

			for (int j = 0; j <= i; ++j)
			{
				double g = 0;
				for (int k = 0; k <= i; ++k)
					g += V[k][i + 1] * V[k][j];

				for (int k = 0; k <= i; ++k)
					V[k][j] -= g * d[k];
			}
		}

		for (int k = 0; k <= i; ++k)
			V[k][i + 1] = 0;
	}

	for (int j = 0; j<n; ++j)
	{
		d[j] = V[n - 1][j];
		V[n - 1][j] = 0;
	}

	V[n - 1][n - 1] = 1;
	e[0] = 0;
}


/**
* Symmetric tridiagonal QL algorithm.
*/

void eig_matrix::tql2()
{
	for (int i = 1; i<n; ++i)
		e[i - 1] = e[i];
	e[n - 1] = 0;

	double f = 0;
	double tst1 = 0;
	double eps = pow(2.0, -52.0);

	for (int l = 0; l<n; ++l)
	{
		// find small subdiagonal element
		tst1 = max(tst1, abs(d[l]) + abs(e[l]));
		int m = l;

		// original while-loop from Java code
		while (m < n)
		{
			if (abs(e[m]) <= eps*tst1)
				break;
			m++;
		}

		// if m == l, d[l] is an eigenvalue, otherwise, iterate
		if (m > l)
		{
			int iter = 0;
			do
			{
				iter = iter + 1;

				// compute implicit shift
				double g = d[l];
				double p = (d[l + 1] - g) / (2.0 * e[l]);
				double r = hypot(p, 1.0);
				if (p < 0)
					r = -r;

				d[l] = e[l] / (p + r);
				d[l + 1] = e[l] * (p + r);
				double dl1 = d[l + 1];
				double h = g - d[l];

				for (int i = l + 2; i<n; ++i)
					d[i] -= h;
				f += h;

				// implicit QL transformation.
				p = d[m];
				double c = 1;
				double c2 = c;
				double c3 = c;
				double el1 = e[l + 1];
				double s = 0;
				double s2 = 0;

				for (int i = m - 1; i >= l; --i)
				{
					c3 = c2;
					c2 = c;
					s2 = s;
					g = c * e[i];
					h = c * p;
					r = hypot(p, e[i]);
					e[i + 1] = s * r;
					s = e[i] / r;
					c = p / r;
					p = c * d[i] - s * g;
					d[i + 1] = h + s * (c * g + s * d[i]);

					// accumulate transformation.
					for (int k = 0; k<n; ++k)
					{
						h = V[k][i + 1];
						V[k][i + 1] = s * V[k][i] + c * h;
						V[k][i] = c * V[k][i] - s * h;
					}
				}

				p = -s * s2 * c3 * el1 * e[l] / dl1;
				e[l] = s * p;
				d[l] = c * p;

			} while (abs(e[l]) > eps*tst1);
		}

		d[l] += f;
		e[l] = 0;
	}

	// Sort eigenvalues and corresponding vectors.
	for (int i = 0; i<n - 1; ++i)
	{
		int k = i;
		double p = d[i];

		for (int j = i + 1; j<n; ++j)
		if (d[j] < p)
		{
			k = j;
			p = d[j];
		}

		if (k != i)
		{
			d[k] = d[i];
			d[i] = p;
			for (int j = 0; j<n; ++j)
				swap(V[j][i], V[j][k]);
		}
	}
}

/**
* Nonsymmetric reduction to Hessenberg form.
*/
void eig_matrix::others()
{
	int low = 0;
	int high = n - 1;

	for (int m = low + 1; m <= high - 1; ++m)
	{
		// scale column.
		double scale = 0;
		for (int i = m; i <= high; ++i)
			scale += abs(H[i][m - 1]);

		if (scale != 0)
		{
			// compute Householder transformation.
			double h = 0;
			for (int i = high; i >= m; --i)
			{
				ort[i] = H[i][m - 1] / scale;
				h += ort[i] * ort[i];
			}

			double g = sqrt(h);
			if (ort[m] > 0)
				g = -g;

			h = h - ort[m] * g;
			ort[m] = ort[m] - g;

			// Apply Householder similarity transformation.
			for (int j = m; j<n; ++j)
			{
				double f = 0;
				for (int i = high; i >= m; --i)
					f += ort[i] * H[i][j];
				f = f / h;

				for (int i = m; i <= high; ++i)
					H[i][j] -= f*ort[i];
			}

			for (int i = 0; i <= high; ++i)
			{
				double f = 0;
				for (int j = high; j >= m; --j)
					f += ort[j] * H[i][j];
				f = f / h;

				for (int j = m; j <= high; ++j)
					H[i][j] -= f*ort[j];
			}

			ort[m] = scale * ort[m];
			H[m][m - 1] = scale * g;
		}
	}

	// accumulate transformations (Algol's ortran)
	for (int i = 0; i<n; ++i)
	for (int j = 0; j<n; ++j)
		V[i][j] = (i == j) ? 1 : 0;

	for (int m = high - 1; m >= low + 1; --m)
	if (H[m][m - 1] != 0)
	{
		for (int i = m + 1; i <= high; ++i)
			ort[i] = H[i][m - 1];

		for (int j = m; j <= high; ++j)
		{
			double g = 0;
			for (int i = m; i <= high; ++i)
				g += ort[i] * V[i][j];

			// double division avoids possible underflow
			g = (g / ort[m]) / H[m][m - 1];
			for (int i = m; i <= high; ++i)
				V[i][j] += g * ort[i];
		}
	}
}



/**
* Nonsymmetric reduction from Hessenberg to double Schur form.
*/

void eig_matrix::hqr2()
{
	// initialize
	int nn = n;
	int n = nn - 1;
	int low = 0;
	int high = nn - 1;
	double eps = pow(2.0, -52.0);
	double exshift = 0;
	double p = 0, q = 0, r = 0, s = 0, z = 0, t, w, x, y;

	// Store roots isolated by balanc and compute matrix norm.
	double norm = 0;
	for (int i = 0; i<nn; ++i)
	{
		if ((i < low) || (i > high))
		{
			d[i] = H[i][i];
			e[i] = 0;
		}

		for (int j = max(i - 1, 0); j<nn; ++j)
			norm += abs(H[i][j]);
	}

	// outer loop over eigenvalue index
	int iter = 0;
	while (n >= low)
	{
		// Look for single small sub-diagonal element.
		int l = n;
		while (l > low)
		{
			s = abs(H[l - 1][l - 1]) + abs(H[l][l]);
			if (s == 0)
				s = norm;

			if (abs(H[l][l - 1]) < eps*s)
				break;

			l--;
		}

		// one root found
		if (l == n)
		{
			H[n][n] = H[n][n] + exshift;
			d[n] = H[n][n];
			e[n] = 0;
			n--;
			iter = 0;
		}

		// two roots found
		else if (l == n - 1)
		{
			w = H[n][n - 1] * H[n - 1][n];
			p = (H[n - 1][n - 1] - H[n][n]) / 2.0;
			q = p * p + w;
			z = sqrt(abs(q));
			H[n][n] = H[n][n] + exshift;
			H[n - 1][n - 1] = H[n - 1][n - 1] + exshift;
			x = H[n][n];

			// double pair
			if (q >= 0)
			{
				if (p >= 0)
					z = p + z;
				else
					z = p - z;

				d[n - 1] = x + z;
				d[n] = d[n - 1];
				if (z != 0)
					d[n] = x - w / z;

				e[n - 1] = 0;
				e[n] = 0;
				x = H[n][n - 1];
				s = abs(x) + abs(z);
				p = x / s;
				q = z / s;
				r = sqrt(p * p + q * q);
				p = p / r;
				q = q / r;

				// row modification
				for (int j = n - 1; j<nn; ++j)
				{
					z = H[n - 1][j];
					H[n - 1][j] = q * z + p * H[n][j];
					H[n][j] = q * H[n][j] - p * z;
				}

				// column modification
				for (int i = 0; i <= n; ++i)
				{
					z = H[i][n - 1];
					H[i][n - 1] = q * z + p * H[i][n];
					H[i][n] = q * H[i][n] - p * z;
				}

				// accumulate transformations
				for (int i = low; i <= high; ++i)
				{
					z = V[i][n - 1];
					V[i][n - 1] = q * z + p * V[i][n];
					V[i][n] = q * V[i][n] - p * z;
				}
			}

			// complex pair
			else
			{
				d[n - 1] = x + p;
				d[n] = x + p;
				e[n - 1] = z;
				e[n] = -z;
			}

			n = n - 2;
			iter = 0;
		}
		else
		{

			// form shift
			x = H[n][n];
			y = 0;
			w = 0;

			if (l < n)
			{
				y = H[n - 1][n - 1];
				w = H[n][n - 1] * H[n - 1][n];
			}

			// Wilkinson's original ad hoc shift
			if (iter == 10)
			{
				exshift += x;
				for (int i = low; i <= n; ++i)
					H[i][i] -= x;

				s = abs(H[n][n - 1]) + abs(H[n - 1][n - 2]);
				x = y = 0.75 * s;
				w = -0.4375 * s * s;
			}

			// MATLAB's new ad hoc shift
			if (iter == 30)
			{
				s = (y - x) / 2.0;
				s = s * s + w;
				if (s > 0)
				{
					s = sqrt(s);
					if (y < x)
						s = -s;

					s = x - w / ((y - x) / 2.0 + s);
					for (int i = low; i <= n; ++i)
						H[i][i] -= s;

					exshift += s;
					x = y = w = 0.964;
				}
			}

			iter = iter + 1;

			// Look for two consecutive small sub-diagonal elements.
			int m = n - 2;
			while (m >= l)
			{
				z = H[m][m];
				r = x - z;
				s = y - z;
				p = (r * s - w) / H[m + 1][m] + H[m][m + 1];
				q = H[m + 1][m + 1] - z - r - s;
				r = H[m + 2][m + 1];
				s = abs(p) + abs(q) + abs(r);
				p = p / s;
				q = q / s;
				r = r / s;

				if (m == l)
					break;

				if (abs(H[m][m - 1]) * (abs(q) + abs(r)) <
					eps * (abs(p) * (abs(H[m - 1][m - 1]) + abs(z) +
					abs(H[m + 1][m + 1]))))
					break;

				m--;
			}

			for (int i = m + 2; i <= n; ++i)
			{
				H[i][i - 2] = 0;
				if (i > m + 2)
					H[i][i - 3] = 0;
			}

			// double QR step involving rows l:n and columns m:n
			for (int k = m; k <= n - 1; ++k)
			{
				int notlast = (k != n - 1);
				if (k != m)
				{
					p = H[k][k - 1];
					q = H[k + 1][k - 1];
					r = (notlast ? H[k + 2][k - 1] : 0);
					x = abs(p) + abs(q) + abs(r);

					if (x != 0)
					{
						p = p / x;
						q = q / x;
						r = r / x;
					}
				}

				if (x == 0)
					break;

				s = sqrt(p * p + q * q + r * r);
				if (p < 0)
					s = -s;

				if (s != 0)
				{
					if (k != m)
						H[k][k - 1] = -s * x;
					else if (l != m)
						H[k][k - 1] = -H[k][k - 1];

					p = p + s;
					x = p / s;
					y = q / s;
					z = r / s;
					q = q / p;
					r = r / p;

					// row modification
					for (int j = k; j<nn; ++j)
					{
						p = H[k][j] + q * H[k + 1][j];
						if (notlast)
						{
							p = p + r * H[k + 2][j];
							H[k + 2][j] = H[k + 2][j] - p * z;
						}

						H[k][j] = H[k][j] - p * x;
						H[k + 1][j] = H[k + 1][j] - p * y;
					}

					// column modification
					for (int i = 0; i <= min(n, k + 3); ++i)
					{
						p = x * H[i][k] + y * H[i][k + 1];
						if (notlast)
						{
							p = p + z * H[i][k + 2];
							H[i][k + 2] = H[i][k + 2] - p * r;
						}
						H[i][k] = H[i][k] - p;
						H[i][k + 1] = H[i][k + 1] - p * q;
					}

					// accumulate transformations
					for (int i = low; i <= high; ++i)
					{
						p = x * V[i][k] + y * V[i][k + 1];
						if (notlast)
						{
							p = p + z * V[i][k + 2];
							V[i][k + 2] = V[i][k + 2] - p * r;
						}
						V[i][k] = V[i][k] - p;
						V[i][k + 1] = V[i][k + 1] - p * q;
					}
				}  // (s != 0 )
			}  // k loop
		}  // check convergence
	}  // while ( n >= low )

	// Backsubstitute to find vectors of upper triangular form.
	if (norm == 0)
		return;

	for (n = nn - 1; n >= 0; --n)
	{
		p = d[n];
		q = e[n];

		// double vector
		if (q == 0)
		{
			int l = n;
			H[n][n] = 1;
			for (int i = n - 1; i >= 0; --i)
			{
				w = H[i][i] - p;
				r = 0;
				for (int j = l; j <= n; ++j)
					r = r + H[i][j] * H[j][n];

				if (e[i] < 0)
				{
					z = w;
					s = r;
				}
				else
				{
					l = i;
					if (e[i] == 0)
					{
						if (w != 0)
							H[i][n] = -r / w;
						else
							H[i][n] = -r / (eps * norm);
					}

					// solve double equations
					else
					{
						x = H[i][i + 1];
						y = H[i + 1][i];
						q = (d[i] - p) * (d[i] - p) + e[i] * e[i];
						t = (x * s - z * r) / q;
						H[i][n] = t;

						if (abs(x) > abs(z))
							H[i + 1][n] = (-r - w * t) / x;
						else
							H[i + 1][n] = (-s - y * t) / z;
					}

					// overflow control
					t = abs(H[i][n]);
					if ((eps*t)*t > 1)
					for (int j = i; j <= n; ++j)
						H[j][n] = H[j][n] / t;
				}
			}
		}

		// complex vector
		else if (q < 0)
		{
			//cout << "Complex" << endl;
			return;
		}
	}

	// vectors of isolated roots
	for (int i = 0; i<nn; ++i)
	if ((i < low) || (i > high))
	for (int j = i; j<nn; ++j)
		V[i][j] = H[i][j];

	// Back transformation to get eigenvectors of original matrix.
	for (int j = nn - 1; j >= low; --j)
	for (int i = low; i <= high; ++i)
	{
		z = 0;
		for (int k = low; k <= min(j, high); ++k)
			z += V[i][k] * H[k][j];

		V[i][j] = z;
	}
}


