#! /bin/bash

set -o errexit # остановка после первой ошибки

# Директории
DATA="data"
RESULT="data/result"
MATR_DIR="data/matrices"
VECT_DIR="data/vectors"
SRC="src"
INCLUDE="include"
BIN_DIR="build/bin"

mkdir -p $MATR_DIR $VECT_DIR $RESULT

# Параметры программы
source ./config.cfg

# Программируем программу выдачи граничных значений и параметров
# cat <<Input >${INCLUDE}/params.h
# #pragma once
# extern const char *matrix_f;
# extern const char *vector_f;
# extern const char *result_f;
# extern const bool matrix_gen;
# extern const int rows_gen;
# extern const int cols_gen;
# extern const int debug;
# extern const double eps;
# Input

# cat <<Input >${SRC}/params.cpp
# #include "params.h"
# const char *matrix_f = "$matrix_f";
# const char *vector_f = "$vector_f";
# const char *result_f = "$result_f";
# const bool matrix_gen = $matrix_gen;
# const int rows_gen = $rows_gen;
# const int cols_gen = $cols_gen;
# const int debug = $debug;
# const double eps = 1e-7;
# Input

# Компиляция и запуск программы
make all 1>/dev/null
${BIN_DIR}/solve