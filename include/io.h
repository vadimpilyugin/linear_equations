#pragma once

#include <string>
using namespace std;

#include "matrix.h"

Matrix<double> generate_matrix(int rows, int cols);
Matrix<double> load_matrix(string filename);
void write_matrix(string filename, const Matrix<double> &result);
