#include<iostream>
using namespace std;

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

	double** delta = new double*[T];
	for (int i = 0; i < T; i++)
	{
		delta[i] = new double[A_r];
	}
	for (int i = 0; i < ini_r; i++)
	{
		for (int j = 0; j < ini_c; j++)
		{
			*(*(delta + i) + j) = 1;
		}
	}// initial state

	int** delta_idx = new int*[T];
	for (int i = 0; i < T; i++)
	{
		delta_idx[i] = new int[A_r];
	}
	for (int i = 0; i < ini_r; i++)
	{
		for (int j = 0; j < ini_c; j++)
		{
			*(*(delta_idx + i) + j) = 0;
		}
	}// initial state index

	for (int i = 0; i < A_c; i++)
	{
		*(*(delta + 0) + i) = *(*(B + i) + *(O + 0)) * *(*(Pi + 0) + i);
	}// initialize delta

	//iteration compute all delta
	for (int t = 1; t < T; t++)// t is current time step - 1
	{
		for (int i = 0; i < A_r; i++)// i refers to the current state
		{
			double max = 0;
			for (int j = 0; j < A_c; j++)// j refers to the previous state
			{
				if (max < *(*(A + j) + i) * *(*(delta + t - 1) + j) * *(*(B + i) + *(O + t)))
				{
					max = *(*(A + j) + i) * *(*(delta + t - 1) + j) * *(*(B + i) + *(O + t));
					*(*(delta_idx + t) + i) = j;
				}
			}
			*(*(delta + t) + i) = max;
		}
	}

	int* X = new int[T];
	for (int i = 0; i < T; i++)
	{
		*(X + i) = 0;
	}// hiden state matrix

	double max = 0;
	for (int i = 0; i < A_c; i++)
	{
		if (max < *(*(delta + T - 1) + i))
		{
			max = *(*(delta + T - 1) + i);
			*(X + T - 1) = i;
		}
	}// compute the last hidden state

	for (int i = T - 2; i >= 0; i--)
	{
				*(X + i) = *(*(delta_idx + i + 1) + *(X + i + 1));
	}

	for (int i = 0; i < T; i++)
	{
		cout << *(X + i) << " ";
	}

	return 0;
}