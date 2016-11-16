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

mkdir $MATR_DIR $VECT_DIR $RESULT

# Параметры программы
source ./config.cfg

# Программируем программу выдачи граничных значений и параметров
cat <<Input >${INCLUDE}/params.h
#pragma once
const char *matrix_f = $matrix_f;
const char *vector_f = $vector_f;
const char *result_f = $result_f;
bool matrix_gen = $matrix_gen;
int rows_gen = $rows_gen;
int cols_gen = $cols_gen;
Input

# Компиляция и запуск программы
make all 1>/dev/null
${BIN_DIR}/solve