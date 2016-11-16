#include "io.h"
#include "matrix.h"
#include "params.h"
#include "slau.h"
#include "solution.h"

int main()
{
	// Ввод данных
	Matrix<double> a;
	Matrix<double> f;
	Matrix<double> output;
	if(matrix_gen)
	{
		a = generate_matrix(rows_gen, cols_gen);
		f = generate_matrix(rows_gen, 1);
	}
	else
	{
		a = load_matrix(matrix_f);
		f = load_matrix(vector_f);
	}
	
	// Решение СЛАУ
	Slau sl(a, f);
	Solution result = sl.gauss();

	// Ввод различных вариантов свободных переменных
	
	// Загрузка ответа
	write_matrix(result_f, result.result());
}
