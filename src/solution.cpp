#include <cmath>
#include <vector>
#include <stack>

#include "solution.h"
#include "params.h"

void Solution::swap_back()
{
	vector<int> var(2);
	auto tmp = st;
	while(!tmp.empty())
	{
		var = tmp.top();
		// sol.swap(var);
		swap(sol(var[0], 0), sol(var[1], 0));
		tmp.pop();
	}
}

int Solution::retrieve_var_num(int n)
{
	stack<std::vector<int>> tmp = st;
	std::vector<int> v(2);
	while(!tmp.empty())
	{
		v = tmp.top();
		if(v[0] == n)
			n = v[1];
		else if(v[1] == n)
			n = v[0];
		tmp.pop();
	}
	return n;
}

void Solution::free_var_entry()
{
	printf("Введите значения для %d свободных переменных:\n", free_var_num);
	int pos = a.n_cols - free_var_num;
	for(int i = 0; i < free_var_num; i++)
	{
		printf("x%d: ", retrieve_var_num(pos + i));
		scanf("%lf", &free_var[i]);
		printf("\n");
	}
}
void Solution::recalculate()
{
	if(t == INF_MANY)
	{
		free_var_entry();
		sol_comp();
		swap_back();
	}
}
bool Solution::test_sol(const Matrix<double> &a1, const Matrix<double> &f1) const
{
	if(t == NO_SOL)
	{
		printf("Решения не существует\n");
		return true;
	}
	double tmp;
	double sum = 0, max = 0;
	for(int i = 0; i < a1.n_rows; i++)
	{
		for(int j = 0; j < a1.n_cols; j++)
			sum += a1(i, j) * sol[j];
		if((tmp = fabs(f1[i] - sum)) > max)
			max = tmp;
		sum = 0;
	}
	printf("Максимальное отклонение = %lf\n", max);
	return max < eps;
}

Solution::Solution(sol_type _t, Matrix<double> _f, Matrix<double> _a, std::stack<vector<int>> _st, int free_num):
									f(_f), a(_a), sol(_a.n_cols), free_var(free_num), free_var_num(free_num), st(_st), t(_t)
{
	if(t == ONLY_ONE)
	{
		sol_comp();
		swap_back();
	}
	else if(t == INF_MANY)
	{
		free_var_entry();
		sol_comp();
		swap_back();
	}
}
void Solution::print() const
{
	if(t == NO_SOL)
		printf("Нет решений\n");
	else
	{
		printf("Решение системы:\n");
		for(int i = 0; i < sol.n_rows; i++)
			printf("%lf\n", sol[i]);
	}
}
void Solution::sol_comp()
{
	if(t == NO_SOL)
		return;
	int i, j, k, cols = a.n_cols, pos = cols - free_var_num;
	sol = f;
	for(i = 0; i < free_var_num; i++)
		sol(pos+i, 0) = free_var[i];
	for(k = pos - 1; k >= 0; k--)
		for(j = k+1; j < cols; j++)
			sol[k] -= sol[j] * a(k, j);
}