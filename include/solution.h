#pragma once

#include "matrix.h"

using namespace std;

/*
 * Класс содержит методы для окончательного вычисления решения.
 * Матрица приведена к верхне-треугольному виду, на диагонали стоят 1,
 * в векторе f находится правая часть СЛАУ, стек содержит перестановки переменных
 * Если есть свободные переменные, то при создании объекта класса
 * пользователя просят ввести их значения.
 * 1) Метод recalculate() дает возможность ввести другие значения свободных переменных
 * 2) print() печатает решение системы
 * 3) test_sol(Matrix &a, Vector &f) вычисляет отклонение полученного решения от
 * 	правой части исходного уравнения и печатает отклонения по порядку.
 *  Параметры - исходная матрица a и вектор правых частей f.
 * 	Возвращаемое значение true, если максимальное отклонение меньше eps
 */
class Solution
{
	const Matrix<double> f;
	const Matrix<double> a; //для обратного хода
	Matrix<double> sol, free_var;
	const int free_var_num;
	const std::stack<vector<int>> st;
public:
	enum sol_type {NO_SOL, INF_MANY, ONLY_ONE};
private:
	const sol_type t;
public:
	Solution(sol_type _t, Matrix<double> _f, Matrix<double> _a, std::stack<vector<int>> _st, int free = 0);
	Solution(sol_type _t): t(_t), free_var_num(0) {}
	void print() const;
	double operator[] (int i) const { return sol(i, 0); }
	void recalculate();
	bool test_sol(const Matrix<double> &a1, const Matrix<double> &f1) const;
	Matrix<double> result() const {return sol;}
private:
	void sol_comp();
	void swap_back();
	void free_var_entry();
	int retrieve_var_num(int n);
};