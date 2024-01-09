#pragma once

#include "Vector2.hpp"

/*template<int size, typename T>
class matrixRow {
public:
	matrixRow();

	T operator[](int index);

private:
	T _row[size];
};

template<int row, int column, typename T>
class matrix
{
public:

	matrix();

	matrixRow<row, T> operator[](int index);

	template<int m, int n, int p>
	matrix<m, p> operator*(matrix<m, n> mat1, matrix<n, p> mat2) {
		matrix<m, p> result;
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < p; j++) {
				result[i][j] = 0;
				for (int k = 0; k < n; k++) {
					result[i][j] += mat1[i][k] * mat2[k][j];
				}
			}
		}
		return result;
	}

private:
	matrixRow<row, T> _matrix[column];
};

template<int size, typename T>
inline matrixRow<size, T>::matrixRow()
{
	for (int i = 0; i < size; i++)
		_row[i] = 0;
}

template<int size, typename T>
inline T matrixRow<size, T>::operator[](int index)
{
	return _row[index];
}

template<int row, int column, typename T>
inline matrix<row, column, T>::matrix()
{
}

template<int row, int column, typename T>
inline matrixRow<row, T> matrix<row, column, T>::operator[](int index)
{
	return _matrix[index];
}
*/