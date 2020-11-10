// Algorithms_lab_5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <stdio.h>
#include <random>
#include <conio.h>
#include <iomanip>
#include <time.h>
#include <chrono>

template<typename T>
bool is_line_empty(T* line, int length) {
	for (int i = 0; i < length; ++i) {
		if (line[i] != 0)
			return false;
	}
	return true;
}

template<typename T>
struct is_bool
	: public std::disjunction<
	std::is_same<bool, typename std::decay<T>::type>,
	std::is_same<char, typename std::decay<T>::type>
	>
{
};

template<typename T,
	std::enable_if_t<std::is_arithmetic<T>::value, bool> = true
>
class Matrix {
private:
	int row;
	T** data;
	int old_row;
public:
	Matrix(int row = 0, T** data = {}) :row(row), data(data), old_row(row) {};

	void create_Matrix(int row) {
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(0.0, 10.0);

		this->row = row;

		int** res = new int* [row];
		for (int i = 0; i < row; ++i) {
			res[i] = new int[row];
			for (int j = 0; j < row; ++j) {
				res[i][j] = int(dist(mt));
			}
		}

		data = res;
		this->row = row;
		old_row = row;
	}

	void print() {
		for (int i = 0; i < old_row; ++i) {
			for (int j = 0; j < old_row; ++j) {
				std::cout << std::setw(4) << data[i][j] << ' ';
			}
			std::cout << '\n';
		}
		std::cout << "\n";
	}

	void dim_Adjuster() {
		int new_dim = row;
		for (int i = 0; i < row; ++i) {
			if (is_line_empty(data[i], row)) {
				new_dim = i;
				break;
			}
		}


		T** res = new T * [new_dim];
		for (int i = 0; i < new_dim; ++i) {
			res[i] = new T[new_dim];
			for (int j = 0; j < new_dim; ++j) {
				res[i][j] = data[i][j];
			}
		}

		data = res;
		row = new_dim;
		old_row = new_dim;
	}

	void additioning() {
		int temp = 1;

		while (true) {
			temp *= 2;
			if (temp > row)
				break;
			else if (temp == row) {
				return;
			}
		}

		int new_row = temp;
		temp = temp - row;


		T** res = new T * [row + temp];
		for (int i = 0; i < row + temp; ++i) {
			res[i] = new T[row + temp];
			for (int j = 0; j < row + temp; ++j) {
				if (i < row && j < row) {
					res[i][j] = data[i][j];
				}
				else
					res[i][j] = 0;
			}
		}

		old_row = row;
		data = res;
		row = new_row;
	}

	Matrix Matrix_Multiplication(const Matrix& another_one) {
		if (another_one.row == 1) {
			T** res = new T * [1];
			res[0] = new T[1];
			res[0][0] = this->data[0][0] * another_one.data[0][0];
			return Matrix(another_one.row, res);
		}

		else {
			T** res = new T * [another_one.row];
			for (int i = 0; i < another_one.row; ++i) {
				res[i] = new T[another_one.row];
				for (int j = 0; j < another_one.row; ++j) {
					res[i][j] = 0;
					for (int k = 0; k < another_one.row; ++k) {
						res[i][j] += this->data[i][k] * another_one.data[k][j];
					}
				}
			}
			return Matrix(another_one.row, res);
		}
	}

	Matrix Matrix_Multiplication_Fast(const Matrix& another_one) {
		if (another_one.row == 1) {
			T** res = new T * [1];
			res[0] = new T[1];
			res[0][0] = this->data[0][0] * another_one.data[0][0];
			return Matrix(another_one.row, res);
		}

		else {
			Matrix a_1_1 = make_Submatrix(*this, 0, row / 2 - 1, 0, row / 2 - 1);
			Matrix a_1_2 = make_Submatrix(*this, 0, row / 2 - 1, row / 2, row - 1);
			Matrix a_2_1 = make_Submatrix(*this, row / 2, row - 1, 0, row / 2 - 1);
			Matrix a_2_2 = make_Submatrix(*this, row / 2, row - 1, row / 2, row - 1);

			Matrix b_1_1 = make_Submatrix(another_one, 0, row / 2 - 1, 0, row / 2 - 1);
			Matrix b_1_2 = make_Submatrix(another_one, 0, row / 2 - 1, row / 2, row - 1);
			Matrix b_2_1 = make_Submatrix(another_one, row / 2, row - 1, 0, row / 2 - 1);
			Matrix b_2_2 = make_Submatrix(another_one, row / 2, row - 1, row / 2, row - 1);

			Matrix c_1_1 = a_1_1 * b_1_1 + a_1_2 * b_2_1;
			Matrix c_1_2 = a_1_1 * b_1_2 + a_1_2 * b_2_2;
			Matrix c_2_1 = a_2_1 * b_1_1 + a_2_2 * b_2_1;
			Matrix c_2_2 = a_2_1 * b_1_2 + a_2_2 * b_2_2;

			Matrix c = make_Matrix(c_1_1, c_1_2, c_2_1, c_2_2);
			return c;
		}
	}

	Matrix Strassen_Matrix_Multiplication(const Matrix& another_one) {
		if (another_one.row == 1) {
			T** res = new T * [1];
			res[0] = new T[1];
			res[0][0] = this->data[0][0] * another_one.data[0][0];
			return Matrix(another_one.row, res);
		}

		else {
			Matrix a_1_1 = make_Submatrix(*this, 0, row / 2 - 1, 0, row / 2 - 1);
			Matrix a_1_2 = make_Submatrix(*this, 0, row / 2 - 1, row / 2, row - 1);
			Matrix a_2_1 = make_Submatrix(*this, row / 2, row - 1, 0, row / 2 - 1);
			Matrix a_2_2 = make_Submatrix(*this, row / 2, row - 1, row / 2, row - 1);

			Matrix b_1_1 = make_Submatrix(another_one, 0, row / 2 - 1, 0, row / 2 - 1);
			Matrix b_1_2 = make_Submatrix(another_one, 0, row / 2 - 1, row / 2, row - 1);
			Matrix b_2_1 = make_Submatrix(another_one, row / 2, row - 1, 0, row / 2 - 1);
			Matrix b_2_2 = make_Submatrix(another_one, row / 2, row - 1, row / 2, row - 1);

			Matrix m_1 = (a_1_1 + a_2_2) * (b_1_1 + b_2_2);
			Matrix m_2 = (a_2_1 + a_2_2) * (b_1_1);
			Matrix m_3 = a_1_1 * (b_1_2 - b_2_2);
			Matrix m_4 = a_2_2 * (b_2_1 - b_1_1);
			Matrix m_5 = (a_1_1 + a_1_2) * (b_2_2);
			Matrix m_6 = (a_2_1 - a_1_1) * (b_1_1 + b_1_2);
			Matrix m_7 = (a_1_2 - a_2_2) * (b_2_1 + b_2_2);


			Matrix c_1_1 = m_1 + m_4 - m_5 + m_7;
			Matrix c_1_2 = m_3 + m_5;
			Matrix c_2_1 = m_2 + m_4;
			Matrix c_2_2 = m_1 - m_2 + m_3 + m_6;

			Matrix c = make_Matrix(c_1_1, c_1_2, c_2_1, c_2_2);
			return c;
		}
	}

	Matrix make_Submatrix(Matrix orig, int begin_row, int end_row, int begin_col, int end_col) {
		T** res = new T * [end_row - begin_row + 1];
		for (int i = 0; i < end_row - begin_row + 1; ++i) {
			res[i] = new T[end_col - begin_col + 1];
			for (int j = 0; j < end_col - begin_col + 1; ++j) {
				res[i][j] = orig.data[i + begin_row][j + begin_col];
			}
		}

		return Matrix(orig.row / 2, res);
	}

	Matrix make_Matrix(Matrix a_1_1, Matrix a_1_2, Matrix a_2_1, Matrix a_2_2) {
		T** res = new T * [a_1_1.row * 2];

		for (int i = 0; i < a_1_1.row; ++i) {
			res[i] = new T[a_1_1.row * 2];
			for (int j = 0; j < a_1_1.row; ++j) {
				res[i][j] = a_1_1.data[i][j];
			}
			for (int j = a_1_1.row; j < a_1_1.row * 2; ++j) {
				res[i][j] = a_1_2.data[i][j - a_1_1.row];
			}
		}
		for (int i = a_1_1.row; i < a_1_1.row * 2; ++i) {
			res[i] = new T[a_1_1.row * 2];
			for (int j = 0; j < a_1_1.row; ++j) {
				res[i][j] = a_2_1.data[i - a_1_1.row][j];
			}
			for (int j = a_1_1.row; j < a_1_1.row * 2; ++j) {
				res[i][j] = a_2_2.data[i - a_1_1.row][j - a_1_1.row];
			}
		}

		return Matrix(a_1_1.row * 2, res);
	}

	Matrix sum_Matrix(const Matrix& another_one) {
		int row = another_one.row;
		T** res = new T * [row];

		for (int i = 0; i < row; ++i) {
			res[i] = new T[row];
			for (int j = 0; j < row; ++j) {
				res[i][j] = this->data[i][j] + another_one.data[i][j];
			}
		}

		return Matrix(row, res);
	}

	Matrix dif_Matrix(const Matrix& another_one) {
		int row = another_one.row;
		T** res = new T * [row];

		for (int i = 0; i < row; ++i) {
			res[i] = new T[row];
			for (int j = 0; j < row; ++j) {
				res[i][j] = this->data[i][j] - another_one.data[i][j];
			}
		}

		return Matrix(row, res);
	}

	Matrix operator+(const Matrix& another) {
		return sum_Matrix(another);
	}
	Matrix operator-(const Matrix& another_one) {
		return dif_Matrix(another_one);
	}
	Matrix operator*(const Matrix& another_one) {
		if (another_one.row >= 64) {
			return Matrix_Multiplication(another_one);
		}
		else
			return Matrix_Multiplication(another_one);

	}
};

//template<typename T>
//void make_Submatrix(T** orig, T**& res, int begin_row, int end_row, int begin_col, int end_col) {
//	res = new T * [end_row - begin_row + 1];
//	for (int i = 0; i < end_row - begin_row + 1; ++i) {
//		res[i] = new T[end_col - begin_col + 1];
//		for (int j = 0; j < end_col - begin_col + 1; ++j) {
//			res[i][j] = orig[i + begin_row][j + begin_col];
//		}
//	}
//}
//
//template<typename T>
//void make_Matrix(T** a_1_1, T** a_1_2, T** a_2_1, T** a_2_2, T**& res, int row) {
//	res = new T * [row * 2];
//	for (int i = 0; i < row; ++i) {
//		res[i] = new T[row * 2];
//		for (int j = 0; j < row; ++j) {
//			res[i][j] = a_1_1[i][j];
//		}
//		for (int j = row; j < row * 2; ++j) {
//			res[i][j] = a_1_2[i][j - row];
//		}
//	}
//	for (int i = row; i < row * 2; ++i) {
//		res[i] = new T[row * 2];
//		for (int j = 0; j < row; ++j) {
//			res[i][j] = a_2_1[i - row][j];
//		}
//		for (int j = row; j < row * 2; ++j) {
//			res[i][j] = a_2_2[i - row][j - row];
//		}
//	}
//}
//
//template<typename T>
//T** sum_Matrix(T** first, T** second, int row) {
//	T** res = new T * [row];
//	for (int i = 0; i < row; ++i) {
//		res[i] = new T[row];
//		for (int j = 0; j < row; ++j) {
//			res[i][j] = first[i][j] + second[i][j];
//		}
//	}
//
//	return res;
//}
//
//template<typename T>
//T** dif_Matrix(T** first, T** second, int row) {
//	T** res = new T * [row];
//
//	for (int i = 0; i < row; ++i) {
//		res[i] = new T[row];
//		for (int j = 0; j < row; ++j) {
//			res[i][j] = first[i][j] - second[i][j];
//		}
//	}
//
//	return res;
//}
//
//template<typename T>
//T** additioning(T** current, int row, int& new_row) {
//	int temp = 1;
//
//	while (true) {
//		temp *= 2;
//		if (temp > row)
//			break;
//		else if (temp == row) {
//			new_row = row;
//			return current;
//		}
//	}
//
//	temp = temp - row;
//
//	T** res = new T * [row + temp];
//	for (int i = 0; i < row + temp; ++i) {
//		res[i] = new T[row + temp];
//		for (int j = 0; j < row + temp; ++j) {
//			if (i < row && j < row) {
//				res[i][j] = current[i][j];
//			}
//			else
//				res[i][j] = 0;
//		}
//	}
//
//	new_row = temp + row;
//	return res;
//}
//
//int** create_Matrix(int row) {
//	std::random_device rd;
//	std::mt19937 mt(rd());
//	std::uniform_real_distribution<double> dist(0.0, 10.0);
//
//	int** res = new int* [row];
//	for (int i = 0; i < row; ++i) {
//		res[i] = new int[row];
//		for (int j = 0; j < row; ++j) {
//			res[i][j] = int(dist(mt));
//		}
//	}
//
//	return res;
//}
//
//template<typename T>
//void print(T** a, int row) {
//	for (int i = 0; i < row; ++i) {
//		for (int j = 0; j < row; ++j) {
//			std::cout << std::setw(4) << a[i][j] << ' ';
//		}
//		std::cout << '\n';
//	}
//	std::cout << "\n";
//}
//
//template<typename T>
//T** Matrix_Multiplication_Naive(T** first, T** second, int row) {
//	if (row == 1) {
//		T** res = new T * [1];
//		res[0] = new T[1];
//		res[0][0] = first[0][0] * second[0][0];
//		return res;
//	}
//
//	else {
//		T** res = new T * [row];
//		for (int i = 0; i < row; ++i) {
//			res[i] = new T[row];
//			for (int j = 0; j < row; ++j) {
//				res[i][j] = 0;
//				for (int k = 0; k < row; ++k) {
//					res[i][j] += first[i][k] * second[k][j];
//				}
//			}
//		}
//		return res;
//	}
//}
//
//template<typename T>
//T** Matrix_Multiplication(T** first, T** second, int row) {
//	if (row == 1) {
//		T** res = new T * [1];
//		res[0] = new T[1];
//		res[0][0] = first[0][0] * second[0][0];
//		return res;
//	}
//
//	else {
//		T** a_1_1 = nullptr;
//		make_Submatrix(first, a_1_1, 0, row / 2 - 1, 0, row / 2 - 1);
//		T** a_1_2 = nullptr;
//		make_Submatrix(first, a_1_2, 0, row / 2 - 1, row / 2, row - 1);
//		T** a_2_1 = nullptr;
//		make_Submatrix(first, a_2_1, row / 2, row - 1, 0, row / 2 - 1);
//		T** a_2_2 = nullptr;
//		make_Submatrix(first, a_2_2, row / 2, row - 1, row / 2, row - 1);
//
//		T** b_1_1 = nullptr;
//		make_Submatrix(second, b_1_1, 0, row / 2 - 1, 0, row / 2 - 1);
//		T** b_1_2 = nullptr;
//		make_Submatrix(second, b_1_2, 0, row / 2 - 1, row / 2, row - 1);
//		T** b_2_1 = nullptr;
//		make_Submatrix(second, b_2_1, row / 2, row - 1, 0, row / 2 - 1);
//		T** b_2_2 = nullptr;
//		make_Submatrix(second, b_2_2, row / 2, row - 1, row / 2, row - 1);
//
//		T** c_1_1 = sum_Matrix(Matrix_Multiplication(a_1_1, b_1_1, row / 2), Matrix_Multiplication(a_1_2, b_2_1, row / 2), row / 2);
//		T** c_1_2 = sum_Matrix(Matrix_Multiplication(a_1_1, b_1_2, row / 2), Matrix_Multiplication(a_1_2, b_2_2, row / 2), row / 2);
//		T** c_2_1 = sum_Matrix(Matrix_Multiplication(a_2_1, b_1_1, row / 2), Matrix_Multiplication(a_2_2, b_2_1, row / 2), row / 2);
//		T** c_2_2 = sum_Matrix(Matrix_Multiplication(a_2_1, b_1_2, row / 2), Matrix_Multiplication(a_2_2, b_2_2, row / 2), row / 2);
//
//		T** c = nullptr;
//		make_Matrix(c_1_1, c_1_2, c_2_1, c_2_2, c, row / 2);
//		return c;
//	}
//}
//
//template<typename T>
//T** Strassen_Matrix_Multiplication(T** first, T** second, int row) {
//	if (row == 1) {
//		T** res = new T * [1];
//		res[0] = new T[1];
//		res[0][0] = first[0][0] * second[0][0];
//		return res;
//	}
//
//	else {
//		T** a_1_1 = nullptr;
//		make_Submatrix(first, a_1_1, 0, row / 2 - 1, 0, row / 2 - 1);
//		T** a_1_2 = nullptr;
//		make_Submatrix(first, a_1_2, 0, row / 2 - 1, row / 2, row - 1);
//		T** a_2_1 = nullptr;
//		make_Submatrix(first, a_2_1, row / 2, row - 1, 0, row / 2 - 1);
//		T** a_2_2 = nullptr;
//		make_Submatrix(first, a_2_2, row / 2, row - 1, row / 2, row - 1);
//
//		T** b_1_1 = nullptr;
//		make_Submatrix(second, b_1_1, 0, row / 2 - 1, 0, row / 2 - 1);
//		T** b_1_2 = nullptr;
//		make_Submatrix(second, b_1_2, 0, row / 2 - 1, row / 2, row - 1);
//		T** b_2_1 = nullptr;
//		make_Submatrix(second, b_2_1, row / 2, row - 1, 0, row / 2 - 1);
//		T** b_2_2 = nullptr;
//		make_Submatrix(second, b_2_2, row / 2, row - 1, row / 2, row - 1);
//
//		T** m_1 = Strassen_Matrix_Multiplication(sum_Matrix(a_1_1, a_2_2, row / 2), sum_Matrix(b_1_1, b_2_2, row / 2), row / 2);
//		T** m_2 = Strassen_Matrix_Multiplication(sum_Matrix(a_2_1, a_2_2, row / 2), b_1_1, row / 2);
//		T** m_3 = Strassen_Matrix_Multiplication(a_1_1, dif_Matrix(b_1_2, b_2_2, row / 2), row / 2);
//		T** m_4 = Strassen_Matrix_Multiplication(a_2_2, dif_Matrix(b_2_1, b_1_1, row / 2), row / 2);
//		T** m_5 = Strassen_Matrix_Multiplication(sum_Matrix(a_1_1, a_1_2, row / 2), b_2_2, row / 2);
//		T** m_6 = Strassen_Matrix_Multiplication(dif_Matrix(a_2_1, a_1_1, row / 2), sum_Matrix(b_1_1, b_1_2, row / 2), row / 2);
//		T** m_7 = Strassen_Matrix_Multiplication(dif_Matrix(a_1_2, a_2_2, row / 2), sum_Matrix(b_2_1, b_2_2, row / 2), row / 2);
//
//		T** c_1_1 = sum_Matrix(m_1, dif_Matrix(sum_Matrix(m_4, m_7, row / 2), m_5, row / 2), row / 2);
//		T** c_1_2 = sum_Matrix(m_3, m_5, row / 2);
//		T** c_2_1 = sum_Matrix(m_2, m_4, row / 2);
//		T** c_2_2 = sum_Matrix(dif_Matrix(m_1, m_2, row / 2), sum_Matrix(m_3, m_6, row / 2), row / 2);
//
//		T** c = nullptr;
//		make_Matrix(c_1_1, c_1_2, c_2_1, c_2_2, c, row / 2);
//		return c;
//	}
//}

void Initializer(int n) {
	Matrix<int> first;
	first.create_Matrix(n);
	Matrix<int> second;
	second.create_Matrix(n);

	auto t_start = std::chrono::high_resolution_clock::now();
	Matrix<int> res = first * second;
	auto t_end = std::chrono::high_resolution_clock::now();

	std::cout << "Time: " << std::chrono::duration<double, std::milli>(t_end - t_start).count() / 1000 << " sec\n";

	//first.additioning();
	//second.additioning();
	//first.print();
	//second.print();
	//res.dim_Adjuster();
	//res.print();
}

int main()
{
	static int row = 0;
	while (true) {
		system("cls");

		std::cout << "Enter row number: ";
		std::cin >> row;

		if (row <= 0)
			break;

		system("cls");

		Initializer(row);

		system("pause");
	}

	return 0;
}

