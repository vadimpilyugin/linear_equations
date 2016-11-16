#pragma once

#include <stack>
#include <vector>

#include "matrix.h"
#include "solution.h"

using namespace std;

class Slau
{
	Matrix<double> a, f;
	std::stack<vector<int>> st; // стек перестановок столбцов
public:
	Slau(const Matrix<double> _a, const Matrix<double> _f): a(_a), f(_f) {}
	void print() const;
	Solution gauss();
private:
	void swap_columns(int first, int second);
	void swap_rows(int first, int second);
	vector<int> find_max(int diag);
};
