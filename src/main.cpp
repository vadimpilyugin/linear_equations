#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

#include "io.h"
#include "matrix.h"
#include "params.h"
#include "slau.h"
#include "solution.h"

int main()
{
	// Ввод данных
	Matrix<double> a, f, output;
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
	try{
		Slau sl(a, f);
		if(debug)
		{
			cout << "Исходная постановка задачи:" << endl;
			sl.print();
			cout << "==========================" << endl;
		}
		Solution result = sl.gauss();
		// Ввод различных вариантов свободных переменных
		if(debug)
		{
			cout << endl << "+++++++++++++++++++++++++" << endl;
			cout << "Результат вычислений: \n";
			result.print();
			cout << "+++++++++++++++++++++++++" << endl << endl;
			cout << "Корректность вычислений: \n";
			if(result.test_sol(a, f))
				printf("Все хорошо\n");
			else
				printf("Все не очень хорошо...\n");
			cout << endl;
		}
		

		// Загрузка ответа
		write_matrix(result_f, result.result());
	}
	catch(string s)
	{
		fprintf(stderr, "%s\n", s.c_str());
		exit(1);
	}catch(const char *s)
	{
		fprintf(stderr, "%s\n", s);
		exit(1);
	}

}
