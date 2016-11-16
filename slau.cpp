#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

const int N=100;
const int numb_lim = 100;

void OutEquasion(const vector<vector<double> > &a, const vector<int> &x, const vector<double> &f)
{
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
			printf("%5.2f  ", a[i][j]);
		printf("x%d", x[i]);
		if(i == N/2)
			printf("  =  ");
		else
			printf("     ");
		printf("%12.9f\n", f[i]);
	}
	printf("\n\n");
}

int main()
{
	time_t t;
	srand((unsigned) time(&t));
	vector<vector <double> > a(N), a1(N);
	vector<double> f(N), f1(N), sol(N);
	vector<int> x(N);
	for(int i = 0; i < N; i++)
		a[i].resize(N);
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
			a[i][j] = rand() % numb_lim;
		f[i] = rand() % numb_lim;
		x[i] = i;
	}
	a1 = a;
	f1 = f;
	//OutEquasion(a, x, f);
	for(int i = 0; i < N; i++)
	{
		double max=a[i][0];
		int max_col = 0;
		for(int j = 1; j < N; j++)
			if(fabs(a[i][j]) > max)
			{
				max = fabs(a[i][j]);
				max_col = j;
			}
        max = a[i][max_col];
		if(fabs(max) < 1e-10)
			throw "Полностью нулевая строка!";
		for(int j = 0; j < N; j++)
			if(a[i][j] != 0)
				a[i][j] = a[i][j] / max;
		f[i] = f[i] / max;
                                                            //OutEquasion(a, x, f);
		a[i][max_col] = 1; //для точности
		for(int k = i+1; k < N; k++)
		{
			double max_elem = a[k][max_col];
			for(int m = 0; m < N; m++)
				if(m != max_col)
				{
					if(fabs(a[i][m]) > 1e-10)
						a[k][m] = a[k][m] - max_elem * a[i][m];
				}
				else
					a[k][max_col] = 0; //для точности
			f[k] = f[k] - f[i] * max_elem;
                                                            //OutEquasion(a, x, f);
		}
	}
	//OutEquasion(a, x, f);
	for(int i = N-1; i >= 0; i--)
		for(int j = 0; j < N; j++)
			if(fabs(a[i][j]) > 1e-10)
				for(int k = i-1; k >= 0; k--)
				{
					f[k] = f[k] - f[i] * a[k][j];
					a[k][j] = 0;
                                                            //OutEquasion(a, x, f);

				}
	//OutEquasion(a, x, f);
    for(int j = 0; j < N; j++)
        for(int i = 0; i < N; i++)
            if(fabs(a[i][j]-1) < 1e-10)
            {
                sol[j] = f[i];
                break;
            }
    //OutEquasion(a, x, sol);
    
	/*   Проверка ответа   */
	printf("Проверка ответа:\n");
	//OutEquasion(a1, x, sol);
	double res = 0;

	for(int i = 0; i < N; i++)
	{
		res = 0;
		for(int j = 0; j < N-1; j++)
		{
			//printf("%5.2f*%5.2f+", a1[i][j], sol[j]);
			res += a1[i][j] * sol[j];
		}
		res += a1[i][N-1] * sol[N-1];
		//printf("%5.2f*%5.2f = %5.2f\n", a1[i][N-1], sol[N-1], res);
		printf("Отклонение f%d: %12.9f\n", i, fabs(res - f1[i]));
	}
	
};
