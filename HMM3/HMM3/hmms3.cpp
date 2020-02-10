#include<iostream>
#include <math.h>
#include<float.h>
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
	// end of input 
	
	double** alfa = new double*[T];
	for (int i = 0; i < T; i++)
	{
		alfa[i] = new double[A_r];
	}
	for (int i = 0; i < T; i++)
	{
		for (int j = 0; j < A_r; j++)
		{
			*(*(alfa + i) + j) = 1;
		}
	}// initialize alfa

	double** beta = new double*[T];
	for (int i = 0; i < T; i++)
	{
		beta[i] = new double[A_r];
	}
	for (int i = 0; i < T; i++)
	{
		for (int j = 0; j < A_r; j++)
		{
			*(*(beta + i) + j) = 0;
		}
	}// initialize beta

	double*** gamma = new double**[T];
	for (int i = 0; i < T; i++)
	{
		*(gamma + i) = new double*[A_r];
		for (int j = 0; j < A_r; j++)
		{
			*(*(gamma + i) + j) = new double[A_r];
		}
	}
	for (int i = 0; i < T; i++)
	{
		for (int j = 0; j < A_r; j++)
		{
			for (int k = 0; k < A_r; k++)
			{
				*(*(*(gamma + i) + j) + k) = 1;
			}	
		}
	}// initialize gamma


	double** gamma_ = new double*[T];
	for (int i = 0; i < T; i++)
	{
		gamma_[i] = new double[A_r];
	}
	for (int i = 0; i < T; i++)
	{
		for (int j = 0; j < A_r; j++)
		{
			*(*(gamma_ + i) + j) = 0;
		}
	}// initialize gamma_

	double* C = new double[T];
	for (int i = 0; i < T; i++)
	{
		*(C + i) = 0;
	}// observation matrix

	int maxIters = 500;
	int iters = 0;
	double logProb = -DBL_MAX;
	double oldLogProb = 0;
	do
	{
		oldLogProb = logProb;

		//initialize alfa
		*(C + 0) = 0;
		for (int i = 0; i < A_r; i++)
		{
			*(*(alfa + 0) + i) = *(*(Pi + 0) + i) * *(*(B + i) + *(O + 0));
			*(C + 0) += *(*(alfa + 0) + i);
		}

		*(C + 0) = 1 / *(C + 0);
		for (int i = 0; i < A_r; i++)
		{
			*(*(alfa + 0) + i) = *(C + 0) * *(*(alfa + 0) + i);
		}

		//iteration compute all alfa
		for (int t = 1; t < T; t++)// t is time step - 1
		{
			*(C + t) = 0;
			for (int i = 0; i < A_r; i++)// i refers to the current state
			{
				*(*(alfa + t) + i) = 0;
				for (int j = 0; j < A_c; j++)// j refers to the previous state
				{
					*(*(alfa + t) + i) += *(*(alfa + t - 1) + j) * *(*(A + j) + i);
				}
				*(*(alfa + t) + i) = *(*(B + i) + *(O + t)) * *(*(alfa + t) + i);
				*(C + t) += *(*(alfa + t) + i);
			}

			*(C + t) = 1 / *(C + t);
			for (int i = 0; i < A_r; i++)
			{
				*(*(alfa + t) + i) = *(C + t) * *(*(alfa + t) + i);
			}
		}

		//initialize beta
		for (int i = 0; i < A_r; i++)
		{
			*(*(beta + T -1) + i) = *(C + T - 1);
		}

		//iteration compute all beta
		for (int t = T - 2; t >= 0; t--)// t is time step - 1
		{
			for (int i = 0; i < A_r; i++)// i refers to the current state
			{
				*(*(beta + t) + i) = 0;
				for (int j = 0; j < A_c; j++)// j refers to the next state
				{
					*(*(beta + t) + i) += *(*(beta + t + 1) + j) * *(*(B + j) + *(O + t + 1)) * *(*(A + i) + j);
				}
				*(*(beta + t) + i) = *(C + t) * *(*(beta + t) + i);
			}
		}

		//iteration compute all gamma
		double denom = 0;
		for (int i = 0; i < A_r; i++)
		{
			denom += *(*(alfa + T - 1) + i);
		}
		for (int t = 0; t < T - 1; t++)
		{
			//double denom = 0;
			//for (int i = 0; i < A_r; i++)// i refers to the current state
			//{
				//for (int j = 0; j < A_c; j++)// j refers to the next state
				//{
					//denom += *(*(alfa + t) + i) * *(*(A + i) + j) * *(*(B + j) + *(O + t + 1)) + *(*(beta + t + 1) + j);
				//}
			//}
			for (int i = 0; i < A_r; i++)// i refers to the current state
			{
				*(*(gamma_ + t) + i) = 0;
				for (int j = 0; j < A_c; j++)// j refers to the next state
				{
					*(*(*(gamma + t) + i) +j ) = (*(*(alfa + t) + i) * *(*(A + i) + j) * *(*(B + j) + *(O + t + 1)) * *(*(beta + t + 1) + j)) / denom;
					*(*(gamma_ + t) + i) += *(*(*(gamma + t) + i) + j);
				}
			}
		}

		//compute gamma_ T - 1
		denom = 0;
		for (int i = 0; i < A_c; i++)
		{
			denom += *(*(alfa + T - 1) + i);
		}
		for (int i = 0; i < A_c; i++)
		{
			*(*(gamma_ + T - 1) + i) = *(*(alfa + T - 1) + i) / denom;
		}
		
		//iteration upsate all A
		for (int i = 0; i < A_r; i++)
		{
			for (int j = 0; j < A_r; j++)
			{
				double up_A_numerator = 0;
				double up_A_denominator = 0;
				for (int t = 0; t < T - 1; t++)
				{
					up_A_numerator += *(*(*(gamma + t) + i) + j);
					up_A_denominator += *(*(gamma_ + t) + i);
				}
				*(*(A + i) + j) = up_A_numerator / up_A_denominator;
			}
		}

		//iteration upsate all B
		for (int j = 0; j < A_r; j++)
		{
			for (int k = 0; k < B_c; k++)
			{
				double up_B_numerator = 0;
				double up_B_denominator = 0;
				for (int t = 0; t < T - 1; t++)
				{
					if (*(O + t) == k)
					{
						up_B_numerator += *(*(gamma_ + t) + j) ;
					}
					up_B_denominator += *(*(gamma_ + t) + j);
				}
				*(*(B + j) + k) = up_B_numerator / up_B_denominator;
			}
		}

		for (int i = 0; i < A_c; i++)
		{
			*(*(Pi + 0) + i) = *(*(gamma_ + 0) + i);
		}

		// compute log[P(O|lambda)]
		logProb = 0;
		for (int i = 0; i < T; i++)
		{
			logProb += log(*(C + i));
		}
		logProb = -logProb;

		iters++;
	}while (iters < maxIters && logProb > oldLogProb);

	cout << "logProb:" << logProb << endl;
	cout << "iters:" << iters << endl;//C

	cout << A_r << " " << A_c << " ";
	for (int i = 0; i < A_r; i++)
	{
		for (int j = 0; j < A_c; j++)
		{
			cout << *(*(A + i) + j) << " ";
		}
	}

	cout << endl;
	cout << B_r << " " << B_c << " ";
	for (int i = 0; i < B_r; i++)
	{
		for (int j = 0; j < B_c; j++)
		{
			cout << *(*(B + i) + j) << " ";
		}
	}
	
	cout << endl;
	for (int i = 0; i < A_c; i++)
	{
		cout << *(*(Pi + 0) + i) << " ";
	}//C

	return 0;
}