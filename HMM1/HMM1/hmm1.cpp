#include<iostream>
using namespace std;

double** arrayMulti(double** firstArray, int fi_r, int fi_c, double** secondArray, int se_r, int se_c)
{
	double** result = new double*[fi_r];
	for (int i = 0; i < fi_r; i++)
	{
		result[i] = new double[se_c];
	}
	for (int i = 0; i < fi_r; i++)
	{
		for (int j = 0; j < se_c; j++)
		{
			*(*(result + i) + j) = 0;
		}
	} // ���ں������ȽϺ�һЩ restore result
	if (fi_c != se_r)
	{
		cout << "Error: can not be multiplied" << endl;
	}
	else
	{
		for (int i = 0; i < fi_r; i++)
		{
			for (int j = 0; j < se_c; j++)
			{
				for (int k = 0; k < fi_c; k++)
				{
					*(*(result + i) + j) += *(*(firstArray + i) + k) * *(*(secondArray + k) + j);
				}

			}
		}
	}
	return result;
}

int main()
{
	int A_r = 0, A_c = 0;
	int B_r = 0, B_c = 0;
	int ini_r = 0, ini_c = 0;   //demention of the matrix
	int T;

	//import data
	cin >> A_r >> A_c;
	double** A = new double*[A_r];
	for (int i = 0; i < A_r; i++)
	{
		A[i] = new double[A_c];
	}
	for (int i = 0; i < A_r; i++)
	{
		for (int j = 0; j < A_c; j++)
		{
			cin >> *(*(A + i) + j);
		}
	}// A is transition matrix

	cin >> B_r >> B_c;
	double** B = new double*[B_r];
	for (int i = 0; i < B_r; i++)
	{
		B[i] = new double[B_c];
	}
	for (int i = 0; i < B_r; i++)
	{
		for (int j = 0; j < B_c; j++)
		{
			cin >> *(*(B + i) + j);
		}
	}// B is observation matrix

	cin >> ini_r >> ini_c;
	double** Pi = new double*[ini_r];
	for (int i = 0; i < ini_r; i++)
	{
		Pi[i] = new double[ini_c];
	}
	for (int i = 0; i < ini_r; i++)
	{
		for (int j = 0; j < ini_c; j++)
		{
			cin >> *(*(Pi + i) + j);
		}
	}// initial state

	cin >> T;
	int* O = new int[T];
	for (int i = 0; i < T; i++)
	{
		cin >> *(O + i);
	}// observation matrix

	double** alfa = new double*[T];
	for (int i = 0; i < T; i++)
	{
		alfa[i] = new double[A_r];
	}
	for (int i = 0; i < ini_r; i++)
	{
		for (int j = 0; j < ini_c; j++)
		{
			*(*(alfa + i) + j) = 1;
		}
	}// initial state
	
	//initialize alfa
	*(alfa + 0) = *arrayMulti(Pi, ini_r, ini_c, B, B_r, B_c);

	//iteration comput all alfa
	for (int t = 1; t < T; t++)// t is time step - 1
	{
		for (int i = 0; i < A_r; i++)// i refers to the current state
		{
			double sum = 0;
			for (int j = 0; j < A_c; j++)// j refers to the previous state
			{
				sum += *(*(A + j) + i) * *(*(alfa + t - 1) + j);
			}
			*(*(alfa + t) + i) = *(*(B + i) + *(O + t)) * sum;
		}
	}

	for (int t = 1; t < T; t++)// t is time step - 1
	{
		double** term_2 = new double* [A_r];
		*term_2 = *arrayMulti((alfa + t - 1), 1, A_r, A, A_r, A_c);

		for (int i = 0; i < A_r; i++)
		{
			*(*(alfa + t) + i) = *(*(B + i) + *(O + t)) * *(*(term_2 +0) + i);
		}
	}

	double PobaGivenSequence = 0;
	for (int i = 0; i < A_r; i++)
	{
		PobaGivenSequence += *(*(alfa + T - 1) + i);
	}

	cout << PobaGivenSequence;

	return 0;
}