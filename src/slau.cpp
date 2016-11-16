#include <stdio.h>
#include <stack>
#include <vector>
#include <cmath>
using namespace std;

#include "slau.h"
#include "params.h"

void Slau::swap_columns(int first, int second)
{
	if(first == second)
	{
		if(debug)
			printf("Максимальный элемент уже в %d столбце\nПерестановка не требуется\n\n", first);
		return;
	}
	if(first >= a.n_cols || second >= a.n_cols)
	{
		fprintf(stderr, "Максимальный номер столбца %d! Указаны %d и %d!\n", a.n_cols, first, second);
		throw -1;
	}
	for(int i = 0; i < a.n_rows; i++)
		swap(a(i, first), a(i, second));
	//a.swap_columns(first, second);
	st.push({first, second}); //так мы запоминаем перестановки
	if(debug)
	{
		printf("Переставляем %d и %d столбцы:\n\n", first, second);
		print();
	}
}
void Slau::swap_rows(int first, int second)
{
	if(first == second)
	{
		if(debug)
			printf("Максимальный элемент уже в %d строке\nПерестановка не требуется\n\n", first);
		return;
	}
	if(first >= a.n_rows || second >= a.n_rows)
	{
		fprintf(stderr, "Максимальный номер столбца %d! Указаны %d и %d!\n", a.n_rows, first, second);
		throw -1;
	}
	for(int i = 0; i < a.n_cols; i++)
		swap(a(first, i), a(second, i));
	// a.swap_rows(first, second);
	swap(f(first, 0), f(second, 0));
	if(debug)
	{
		printf("Переставляем %d и %d строки:\n\n", first, second);
		print();
	}
}

vector<int> Slau::find_max(int d)
{
	int i, j;
	double max = 0;
	vector<int> result = {0, 0};
	for(i = d; i < a.n_rows; i++)
		for(j = d; j < a.n_cols; j++)
			if(fabs(a(i, j)) > max)
			{
				max = a(i, j);
				result[0] = i;
				result[1] = j;
			}		
	return result;	
}

Solution Slau::gauss()
{
	int rows = a.n_rows, cols = a.n_cols;
	int i, j, k, d = min(rows, cols);
	vector<int> pos(2);
	for(i = 0; i < d; i++)
	{
		pos = find_max(i);
		if(fabs(a(pos[0], pos[1])) < eps)
			break;
		swap_columns(pos[1], i);
		swap_columns(pos[0], i);
		for(j = i + 1; j < cols; j++)
			a(i, j) = a(i, j) / a(i, i);
		f(i, 0) = f(i, 0) / a(i, i);
		for(k = i+1; k < rows; k++)
		{
			for(j = i+1; j < cols; j++)
				a(k, j) = a(k, j) - a(i, j) * a(k, i);
			f(k, 0) = f(k, 0) - f(i, 0) * a(k, i);
			a(k, i) = 0;
		}
		if(debug)
		{
			printf("Обнуляем %d столбец:\n\n", i);
			print();
		}
	}
	;
	if(i == d)
	{
		if(rows > d)
		{
			for(i = d; i < rows; i++)
			{
				if(fabs(f(i, 0)) > eps)
					return Solution(Solution::NO_SOL);
			}
			return Solution(Solution::ONLY_ONE, f, a, st);
		}
		else if(cols > d)
			return Solution(Solution::INF_MANY, f, a, st, cols - d);
		else
			return Solution (Solution::ONLY_ONE, f, a, st);
	}
	else  //сюда попадаем, если есть нулевая подматрица
	{
		k = i;
		for(i = k; i < rows; i++)
			if(fabs(f(i, 0)) > eps)
				return Solution(Solution::NO_SOL);
		return Solution(Solution::INF_MANY, f, a, st, cols - k);
	}
}

void Slau::print() const
{
	int rows = a.n_rows, cols = a.n_cols;
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
			printf("%5.2f  ", a(i, j));
		printf("|  %5.2f\n", f(i, 0));
	}
	printf("\n\n");
}