#include <chrono>
#include <random>
#include <fstream>

Matrix<double> generate_matrix(int rows, int cols)
{
	// construct a trivial random generator engine from a time-based seed:
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator (seed);
	std::uniform_int_distribution<int> distribution(-50, 50);
    // std::cout << distribution(generator) << " ";
    Matrix<double> result(rows, cols);
    for(int i = 0; i < rows; i++)
		for(int j = 0; j < cols; j++)
			result(i, j) = distribution(generator);
	return result;
}

Matrix<double> load_matrix(string filename)
{
	ifstream matr(filename);
	int rows, cols;
	matr >> rows >> cols;
	Matrix<double> result;
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < cols; j++)
			matr >> result(i, j);
	return result;
}

void write_matrix(string filename, const Matrix<double> &result)
{
	ofstream matr(filename);
	matr << result.n_rows << ' ' << result.n_cols << endl;
	for(int i = 0; i < result.n_rows; i++)
	{
		for(int j = 0; j < result.n_cols; j++)
			matr << result(i, j) << ' ';
		matr << endl;
	}
}
