#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stack>
#include <string.h>
using namespace std;

/*
 * Пилюгин Вадим, 323 группа
 * Программа решения СЛАУ методом Гаусса
 */

const int val_limit = 20; //для ГСЧ: от -val/2 до val/2
const double eps = 1e-10;
const bool debug = false;

int min(int a, int b) { return a < b ? a : b; }

/* Класс описывает матрицу размера mxn, где m и n > 0
 * Добавлять или удалять строки или столбцы нельзя
 * 1)Можно поменять местами столбцы и строки. Прим.: Индексы начинаются с 1.
 * 2)Обращение к элементу: A(1,1) - (номер_стр, номер_стлб).
 * 3)Найти max элемент в подматрице - find_max(номер_элемента_на_диагонали)
 *  Возвращаемое значение - пара индексов <i, j> макс. элемента.
 * 5)Напечатать матрицу - print()
 */
class Matrix
{
	//vector<vector<double> > a;
	double **a;
	int m, n;
public:
	Matrix(int rows_n, int columns_n);
	Matrix(): m(1), n(1) { a = new double * [1]; *a = new double [1]; **a = 0; }
	Matrix(Matrix &&x): m(x.m), n(x.n) { a = x.a; x.a = nullptr; }
	Matrix(ifstream &f);
	void operator= (Matrix &&x);
	void operator= (const Matrix &x);
	Matrix(const Matrix &other);
	~Matrix();
	void swap_rows(int first, int second);
	void swap_columns(int first, int second);
	vector<int> find_max(int diag_n) const;
	void print() const;//\***********
	double &operator() (int row, int col);
	double operator() (int row, int col) const;
	double operator() (vector<int> v) const;
	int get_m() const { return m; }
	int get_n() const { return n; }
};

//==========================<Реализация>================================
Matrix::Matrix(int rows_n, int columns_n): m(rows_n), n(columns_n)
{
	if(n <= 0 || m <= 0)
	{
		fprintf(stderr, "Некорректный размер матрицы: %dx%d\n", m, n);
		throw -1;
	}
	a = new double * [m];
	for(int i = 0; i < m; i++)
	{
		a[i] = new double [n];
		for(int j = 0; j < n; j++)
			a[i][j] = rand() % val_limit - val_limit/2; //заполнение случайными числами
	}
}
void Matrix::operator= (Matrix &&x)
{
	a = x.a;
	x.a = nullptr;
	m = {x.m};
	n = {x.n};
}
void Matrix::operator= (const Matrix &x)
{
	for(int i = 0; i < m; i++)
		for(int j = 0; j < n; j++)
			if(j < x.n && i < x.m && j < n && i < m)
				a[i][j] = x.a[i][j];
			else if(j >= n || i >= m)
				;
			else if(j >= x.n || i >= x.m)
				a[i][j] = 0;
}
Matrix::Matrix(const Matrix &other): m(other.m), n(other.n)
{
	a = new double * [m];
	for(int i = 0; i < m; i++)
		a[i] = new double [n];
	for(int i = 0; i < m; i++)
		for(int j = 0; j < n; j++)
			a[i][j] = other.a[i][j];
}
Matrix::~Matrix()
{
	if(a != nullptr)
	{
		for(int i = 0; i < m; i++)
			delete [] a[i];
		delete [] a;
	}
};
void Matrix::swap_columns(int first, int second)
{
	if(first > n || first < 1 || second > n || second < 1)
	{
		fprintf(stderr, "Некорректные номера столбцов: %d, %d\nКорректные номера: 1..%d", first, second, n);
		throw -3;
	}
	for(int i = 0; i < m; i++)
		swap(a[i][first-1], a[i][second-1]);
}
void Matrix::swap_rows(int first, int second)
{
	if(first > m || first < 1 || second > m || second < 1)
	{
		fprintf(stderr, "Некорректные номера строк: %d, %d\nКорректные номера: 1..%d", first, second, m);
		throw -2;
	}
	for(int j = 0; j < n; j++)
		swap(a[first-1][j], a[second-1][j]);
}
vector<int> Matrix::find_max(int diag_n) const
{
	double max = 0;
	int max_col = diag_n, max_row = diag_n;
	if(diag_n < 1 || diag_n > min(m, n))
	{
		fprintf(stderr, "Недопустимый для диагонали индекс: %d\nКорректные индексы: 1..%d", diag_n, min(m, n));
		throw -1;
	}
	for(int i = diag_n - 1; i < m; i++)
		for(int j = diag_n - 1; j < n; j++)
			if(fabs(a[i][j]) > max)
			{
				max = fabs(a[i][j]);
				max_col = j+1;
				max_row = i+1;
			}
	return {max_row, max_col};
}
void Matrix::print() const
{
	for(int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
			printf("%5.2f  ", a[i][j]);
		printf("\n");
	}
	printf("\n\n");
}
double & Matrix::operator() (int row, int col)
{
	if(row < 1 || row > m || col < 1 || col > n)
	{
		fprintf(stderr, "\nНекорректный доступ к элементу: (%d, %d).\nМаксимально возможный: (%d, %d)\n", row, col, m, n);
		throw -6;
	}
	return a[row-1][col-1];
}
double Matrix::operator() (int row, int col) const
{
	if(row < 1 || row > m || col < 1 || col > n)
	{
		fprintf(stderr, "\nНекорректный доступ к элементу: (%d, %d).\nМаксимально возможный: (%d, %d)\n", row, col, m, n);
		throw -6;
	}
	return a[row-1][col-1];
}
double Matrix::operator() (vector<int> v) const
{
	if(v[0] - 1 < 1 || v[0] - 1 > m || v[1] - 1 < 1 || v[1] - 1 > n)
	{
		fprintf(stderr, "\nНекорректный доступ к элементу: (%d, %d).\nМаксимально возможный: (%d, %d)\n", v[0] - 1, v[1] - 1, m, n);
		throw -6;
	}
	return a[v[0] - 1][v[1] - 1];
}
//==========================</Реализация>===============================

/*
 * Вектор - матрица nx1.
 * Индексы элементов с 1.
 * 1) Поменять местами элементы - swap(index1, index2)
 * 2) Вернуть размер - size().
 * 3) find_max - очевидно, вернуть максимальный элемент вектора
 */
class Vector
{
	Matrix v;
	int _size;
public:
	Vector(): v(), _size(0) {}
	Vector(int n): v(n, 1), _size(n) {}
	Vector(Vector &&f): v(move(f.v)), _size(f._size) {}
	Vector(const Vector &f): v(f.v), _size(f._size) {}
	~Vector() { }
	void operator =(Vector &&x) { v = move(x.v); _size = {x._size}; }
	void operator =(const Vector &x) { v = x.v; } //размер вектора не меняется
	double &operator [] (int row) { return v(row, 1); }
	double operator [] (int row) const { return v(row, 1); }
	int size() const { return _size; }
	void swap(int first, int second) { v.swap_rows(first, second); }
	void swap(const vector<int> x) { v.swap_rows(x[0], x[1]); }
	double find_max() const { return v(v.find_max(1)); }
	void print() const { v.print(); }
};

/*
 * Класс описывает стек, содержащий пары <int, int>.
 * retrieve_var_num(int var) - возвращает номер переменной до перестановок
 */

class Stack
{
	int *st_top, *st_bottom, size, cnt;
public:
	Stack();
	Stack(Stack &&st);
	Stack(const Stack &st);
	~Stack();
	void push(vector<int> v);
	void pop();
	vector<int> top() const;
	int retrieve_var_num(int var) const;
	bool empty() const { return cnt == 0; }
};

//==========================<Реализация>================================
Stack::Stack()
{
	size = 16;
	cnt = 0;
	st_top = new int[size];
	if(st_top == nullptr)
	{
		fprintf(stderr, "Не удалось создать стек\n");
		throw -4;
	}
	st_bottom = st_top;
}
Stack::Stack(Stack &&st)
{
	st_top = st.st_top;
	st_bottom = st.st_bottom;
	size = st.size;
	cnt = st.cnt;
	st.st_top = nullptr;
	st.st_bottom = nullptr;
}
Stack::Stack(const Stack &st)
{
	size = st.size;
	cnt = st.cnt;
	st_bottom = new int [size];
	int *tmp = st_bottom;
	memcpy(tmp, st.st_bottom, sizeof(int)*cnt*2);
	st_top = st_bottom + cnt * 2 - 1;
}
Stack::~Stack()
{
	if(st_bottom != nullptr)
		delete [] st_bottom;
}
void Stack::push(vector<int> v)
{
	if(size == cnt*2)
	{
		size *= 2;
		int *new_bottom = new int [size], *tmp = new_bottom;
		memcpy(tmp, st_bottom, sizeof(int)*cnt*2);
		delete [] st_bottom;
		st_bottom = new_bottom;
		st_top = st_bottom + cnt*2 - 1;
	}
	if(cnt == 0)
	{
        st_top[0] = v[0];
        st_top[1] = v[1];
        st_top++;
	}
	else
	{
        st_top[1] = v[0];
        st_top[2] = v[1];
        st_top += 2;
	}

	cnt++;
}
void Stack::pop()
{
	if(cnt > 0)
	{
		st_top -= 2;
		cnt--;
	}
}
vector<int> Stack::top() const
{
	if(cnt > 0)
		return {st_top[-1], st_top[0]};
	else
		throw "Стек пуст";
}
int Stack::retrieve_var_num(int var) const
{
	int *tmp = st_top;
	for(int i = 0; i < cnt; i++)
	{
		if(tmp[0] == var)
            var = tmp[-1];
        else if(tmp[-1] == var)
            var = tmp[0];
        tmp -= 2;
	}
	return var;
}
//==========================</Реализация>===============================

/*
 * Класс содержит методы для окончательного вычисления решения.
 * Матрица приведена к верхне-треугольному виду, на диагонали стоят 1,
 * в векторе v находится правая часть СЛАУ, стек содержит перестановки переменных
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
	const Vector v;
	const Matrix a; //для обратного хода
	Vector sol, free_var;
	const int free_var_num;
	Stack st;
public:
	enum sol_type {NO_SOL, INF_MANY, ONLY_ONE};
private:
	const sol_type t;
public:
	//Solution(Vector &f): t(ONLY_ONE) { v(move(f)); }
	Solution(Vector &&f, Matrix &&m, Stack &&s, sol_type _t, int free_num = 0);
	Solution(sol_type p): v(), a(), free_var_num(), t(p) {}
	Solution(const Solution &s): v(s.v), a(s.a), sol(s.sol), free_var(s.free_var), free_var_num(s.free_var_num), st(s.st), t(s.t) {}
	void print() const;
	double operator[] (int i) const { return sol[i]; }
	void recalculate();
	bool test_sol(const Matrix &a, const Vector &f) const;
private:
	void sol_comp();
	void swap_back();
	void free_var_entry();
};

//==========================<Реализация>================================
void Solution::swap_back()
{
	vector<int> var(2);
	Stack tmp = st;
	while(!tmp.empty())
	{
		var = tmp.top();
		sol.swap(var);
		tmp.pop();
	}
}
void Solution::free_var_entry()
{
	printf("Введите значения для %d свободных переменных:\n", free_var_num);
	for(int i = 0; i < free_var_num; i++)
	{
		printf("x%d: ", st.retrieve_var_num(a.get_n() - i));
		scanf("%lf", &free_var[a.get_n() - i]);
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
bool Solution::test_sol(const Matrix &matr, const Vector &f) const
{
	if(t == NO_SOL)
	{
		printf("Решения не существует\n");
		return true;
	}
	if(debug)
		printf("Проверка правильности решения:\n");
	double sum = 0, max = 0;
	int m = matr.get_m(), n = matr.get_n();
	Vector delta(m);
	for(int i = 1; i <= m; i++)
	{
		for(int j = 1; j <= n; j++)
		{
			if(j < n && debug)
				printf("%5.2f * %5.2f + ", matr(i, j), sol[j]);
			sum += matr(i, j) * sol[j];
		}
		if(debug)
			printf("%5.2f * %5.2f = %5.2f =? %5.2f - ", matr(i, n), sol[n], sum, f[i]);
		delta[i] = fabs(f[i] - sum);
		if(fabs(delta[i]) > eps && debug)
			printf("False\n");
		else if(debug)
			printf("True\n");
		if(delta[i] > max)
			max = delta[i];
		sum = 0;
	}
	/*
	if(debug)
	{
		printf("\n\nОтклонения от правой части:\n");
		delta.print();
	}*/
	printf("Максимальное отклонение = %lf\n", max);
	return max <= eps;
}

Solution::Solution(Vector &&f, Matrix &&m, Stack &&s, sol_type _t, int free_num): v(move(f)), a(move(m)), sol(m.get_n()),
																				free_var(m.get_n()), free_var_num(free_num), st(move(s)), t(_t)
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
		sol.print();
	}
}

void Solution::sol_comp()
{
	int i, k, d = min(a.get_m(), a.get_n()), n = a.get_n();
	if(t == ONLY_ONE)
		sol = v;
	else if(t == INF_MANY)
	{
		sol = v;
		for(i = 0; i < free_var_num; i++)
			sol[a.get_n() - i] = free_var[a.get_n() - i];
	}
	for(i = n; i > 0; i--)
		for(k = i-1; k > 0; k--)
			if(k <= d)
				sol[k] = sol[k] - sol[i] * a(k, i);
}
//==========================</Реализация>===============================

/*
 * Класс представляет систему линейных уравнений с матрицей a и вектором
 * правых частей f.
 * 1) Метод gauss() обеспечивает решение системы методом Гаусса.
 * Возвращаемое значение: объект класса Solution, который содержит
 * формулы для окончательного вычисления вектора решения.
 */
class Slau
{
	Matrix a, a2;
	Vector f, f2;
	Stack st;
public:
	Slau(int rows_n, int columns_n);
	Slau(ifstream &&file);
	Slau(const Matrix &m, const Vector &v): a(m), f(v) {}
	void print() const;
	Solution gauss();
	Matrix get_a() const { return a2; }
	Vector get_f() const { return f2; }
private:
	void swap_columns(int first, int second);
	void swap_rows(int first, int second);
	void copy();
};

//==========================<Реализация>================================
Slau::Slau(int rows_n, int columns_n): a(rows_n, columns_n), a2(rows_n, columns_n), f(rows_n), f2(rows_n)
{
	for(int i = 1; i <= rows_n; i++)
		f[i] = rand() % val_limit - val_limit/2;
	copy();
}
void Slau::copy()
{
	for(int i = 1; i <= a.get_m(); i++)
    	f2[i] = f[i];
    for(int i = 1; i <= a.get_m(); i++)
    	for(int j = 1; j <= a.get_n(); j++)
    		a2(i,j) = a(i,j);
}

Slau::Slau(ifstream &&file)
{
	int m, n, i, j;
	char delim;
	file >> m >> n;
	a = Matrix(m, n);
	f = Vector(m);
	a2 = Matrix(m, n);
	f2 = Vector(m);
	for(i = 1; i <= m; i++)
	{
		for(j = 1; j <= n; j++)
			file >> a(i, j);
		file >> delim;
		file >> f[i];
	}
	copy();
}

void Slau::print() const
{
	int n = a.get_n(), m = a.get_m();
	for(int i = 1; i <= m; i++)
	{
		for(int j = 1; j <= n; j++)
			printf("%5.2f  ", a(i, j));
		printf("|  %5.2f\n", f[i]);
	}
	printf("\n\n");
}
void Slau::swap_columns(int first, int second)
{
	if(first == second)
	{
		if(debug)
			printf("Максимальный элемент уже в %d столбце\nПерестановка не требуется\n\n", first);
		return;
	}
	a.swap_columns(first, second);
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
	a.swap_rows(first, second);
	swap(f[first], f[second]);
	if(debug)
	{
		printf("Переставляем %d и %d строки:\n\n", first, second);
		print();
	}
}
Solution Slau::gauss()
{
	int m = a.get_m(), n = a.get_n();
	int i, j, k, d = min(m, n);
	vector<int> pos_max(2);
	for(i = 1; i <= d; i++)
	{
		pos_max = a.find_max(i);
		if(fabs(a(pos_max[0],pos_max[1])) < eps)
			break; //это значит, что вся подматрица нулевая
		swap_columns(pos_max[1], i);
		swap_rows(pos_max[0], i);
		for(j = i + 1; j <= n; j++)
			a(i, j) = a(i, j) / a(i, i);
		f[i] = f[i] / a(i, i);
		a(i, i) = 1;
		for(k = i + 1; k <= m; k++)
		{
			for(j = i + 1; j <= n; j++)
				a(k, j) = a(k, j) - a(i, j) * a(k, i);
			f[k] = f[k] - f[i] * a(k, i);
			a(k, i) = 0;
		}
		if(debug)
		{
			printf("Обнуляем %d столбец:\n\n", i);
			print();
		}
	}
	;
	if(i == d+1)
	{
		if(m > d) //уравнений больше, чем переменных
		{
			for(i = m; i > d; i--)
			{
				if(fabs(f[i]) > eps)
					return Solution(Solution::NO_SOL);
			}
			return Solution(move(f), move(a), move(st), Solution::ONLY_ONE);
        }
		else if(n > d) //число переменных больше числа уравнений
			return Solution(move(f), move(a), move(st), Solution::INF_MANY, n - d);
		else
			return Solution (move(f), move(a), move(st), Solution::ONLY_ONE);
	}
	else  //сюда попадаем, если есть нулевая подматрица
	{
		k = i;
		for(i = k; i <= m; i++)
			if(fabs(f[i]) > eps)
				return Solution(Solution::NO_SOL);
		return Solution(move(f), move(a), move(st), Solution::INF_MANY, n - k + 1);
	}
	//a = a1;
	//f = f1;
}
//==========================</Реализация>===============================

int main()
{
	srand(time(nullptr)); //инициализация ГСЧ
	bool t = true;
	Slau s(ifstream("matrix1.txt"));  //ввод из файла
	//Slau s(100, 102); //заполняется случайными числами
	s.print();
	Solution sol(s.gauss());
	for(int i = 0; i < 3; i++)
	{
		sol.print();
		t = sol.test_sol(s.get_a(), s.get_f());
		sol.recalculate();
	}

	/*

	for(int i = 0; i < 20; i++)
	{
		Matrix *a = new Matrix(2, 2);
		Vector *f = new Vector(2);
		Slau *s = new Slau(*a, *f);
		printf("\n\n===============Система номер %d===================\n\n", i+1);
		s -> print();
		Solution *sol = new Solution(s -> gauss());
		sol -> print();
		if(!sol -> test_sol(*a, *f))
			t = false;
		delete a;
		delete f;
		delete s;
		delete sol;
	}*/
	printf("Все хорошо? ");
	if(t)
		printf("Да\n");
	else
		printf("Нет\n");
}
