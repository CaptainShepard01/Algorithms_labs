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
int dgt_cnt(T n) {
	int res = 1;	
	while ((n /= 10) > 0) res++;
	return res;
}

template<typename T>
struct is_bool_is_char
	: public std::disjunction<
	std::is_same<bool, typename std::decay<T>::type>,
	std::is_same<char, typename std::decay<T>::type>
	>
{};

template<
	typename T,
	typename = typename std::enable_if<std::is_arithmetic<T>::value && !is_bool_is_char<T>::value, T>::type
>
class Matrix {
private:
	std::vector<std::vector<T>> data;
	int row;
	int column;
	int old_row;
	int old_column;
public:
	Matrix(std::vector<std::vector<T>> data = {}, int row = 0, int column = 0) :row(row), data(data), old_row(row), column(column), old_column(column) {};

	bool is_row_empty(int index_row) {
		for (int i = 0; i < column; ++i) {
			if (data[index_row][i] != 0)
				return false;
		}
		return true;
	}

	bool is_column_empty(int index_column) {
		for (int i = 0; i < row; ++i) {
			if (data[i][index_column] != 0)
				return false;
		}
		return true;
	}

	void delete_row() {
		data.pop_back();
		row--;
	}

	void delete_column() {
		for (int i = 0; i < row; ++i) {
			data[i].pop_back();
		}
		column--;
	}

	void create_Matrix(int row, int column) {
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(0.0, 50.0);

		this->row = row;
		this->column = column;

		std::vector<std::vector<T>> res(row);
		for (int i = 0; i < row; ++i) {
			res[i].resize(column);
			for (int j = 0; j < column; ++j) {
				res[i][j] = int(dist(mt));
			}
		}

		data = res;
		old_row = row;
		old_column = column;
	}

	void print() {
		int width = 2;
		if (data.size() != 1) {
			width = std::max(dgt_cnt(data[0][0]), dgt_cnt(data[1][1]));
		}
		else
			width = dgt_cnt(data[0][0]);

		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < column; ++j) {
				std::cout << std::setw(width) << data[i][j] << ' ';
			}
			std::cout << '\n';
		}
		std::cout << "\n";
	}

	void setRow(int row) {
		this->row = row;
	}

	void setColumn(int column) {
		this->column = column;
	}

	void dim_Adjuster() {
		for (int i = row - 1; i >= 0; --i) {
			if (is_row_empty(i)) {
				delete_row();
			}
			else 
				break;
		}

		for (int i = column - 1; i >= 0; --i) {
			if (is_column_empty(i)) {
				delete_column();
			}
			else
				break;
		}
	}

	void additioning() {
		int temp = 1;

		while (true) {
			temp *= 2;
			if (temp > row) {
				if (temp > column)
					break;
			}
			else if (temp == row && temp == column) {
				return;
			}
		}

		int new_row = temp;
		//temp = temp - row;


		std::vector<std::vector<T>> res(temp);
		for (int i = 0; i < temp; ++i) {
			res[i].resize(temp);
			for (int j = 0; j < temp; ++j) {
				if (i < row && j < column) {
					res[i][j] = data[i][j];
				}
				else
					res[i][j] = 0;
			}
		}

		old_row = row;
		old_column = column;
		data = res;
		row = new_row;
		column = new_row;
	}

	Matrix Matrix_Multiplication(const Matrix& another_one) {
		if (another_one.row == 1) {
			std::vector<std::vector<T>> res(1);
			res[0].resize(1);
			res[0][0] = this->data[0][0] * another_one.data[0][0];
			return Matrix(res, another_one.row, another_one.column);
		}

		else {
			std::vector<std::vector<T>> res(another_one.row);
			for (int i = 0; i < another_one.row; ++i) {
				res[i].resize(another_one.row);
				for (int j = 0; j < another_one.row; ++j) {
					res[i][j] = 0;
					for (int k = 0; k < another_one.row; ++k) {
						res[i][j] += this->data[i][k] * another_one.data[k][j];
					}
				}
			}
			return Matrix(res, another_one.row, another_one.column);
		}
	}

	Matrix Matrix_Multiplication_Fast(const Matrix& another_one) {
		if (another_one.row == 1) {
			std::vector<std::vector<T>> res(1);
			res[0].resize(1);
			res[0][0] = this->data[0][0] * another_one.data[0][0];
			return Matrix(res, another_one.row, another_one.column);
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
			std::vector<std::vector<T>> res(1);
			res[0].resize(1);
			res[0][0] = this->data[0][0] * another_one.data[0][0];
			return Matrix(res, another_one.row, another_one.column);
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
		std::vector<std::vector<T>> res(end_row - begin_row + 1);
		for (int i = 0; i < end_row - begin_row + 1; ++i) {
			res[i].resize(end_col - begin_col + 1);
			for (int j = 0; j < end_col - begin_col + 1; ++j) {
				res[i][j] = orig.data[i + begin_row][j + begin_col];
			}
		}

		return Matrix(res, orig.row / 2, orig.column / 2);
	}

	Matrix make_Matrix(Matrix a_1_1, Matrix a_1_2, Matrix a_2_1, Matrix a_2_2) {
		std::vector<std::vector<T>> res(a_1_1.row * 2);

		for (int i = 0; i < a_1_1.row; ++i) {
			res[i].resize(a_1_1.row * 2);
			for (int j = 0; j < a_1_1.row; ++j) {
				res[i][j] = a_1_1.data[i][j];
			}
			for (int j = a_1_1.row; j < a_1_1.row * 2; ++j) {
				res[i][j] = a_1_2.data[i][j - a_1_1.row];
			}
		}
		for (int i = a_1_1.row; i < a_1_1.row * 2; ++i) {
			res[i].resize(a_1_1.row * 2);
			for (int j = 0; j < a_1_1.row; ++j) {
				res[i][j] = a_2_1.data[i - a_1_1.row][j];
			}
			for (int j = a_1_1.row; j < a_1_1.row * 2; ++j) {
				res[i][j] = a_2_2.data[i - a_1_1.row][j - a_1_1.row];
			}
		}

		return Matrix(res, a_1_1.row * 2, a_1_1.column * 2);
	}

	Matrix sum_Matrix(const Matrix& another_one) {
		int row = another_one.row;
		int column = another_one.column;
		std::vector<std::vector<T>> res(row);

		for (int i = 0; i < row; ++i) {
			res[i].resize(column);
			for (int j = 0; j < column; ++j) {
				res[i][j] = this->data[i][j] + another_one.data[i][j];
			}
		}

		return Matrix(res, row, column);
	}

	Matrix dif_Matrix(const Matrix& another_one) {
		int row = another_one.row;
		int column = another_one.column;
		std::vector<std::vector<T>> res(row);

		for (int i = 0; i < row; ++i) {
			res[i].resize(column);
			for (int j = 0; j < column; ++j) {
				res[i][j] = this->data[i][j] - another_one.data[i][j];
			}
		}

		return Matrix(res, row, column);
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

void Initializer(int n, int m) {
	Matrix<int> first;
	first.create_Matrix(n, m);
	Matrix<int> second;
	second.create_Matrix(m, n);
	first.additioning();
	second.additioning();

	auto t_start = std::chrono::high_resolution_clock::now();
	Matrix<int> res = first * second;
	auto t_end = std::chrono::high_resolution_clock::now();

	std::cout << "Time: " << std::chrono::duration<double, std::milli>(t_end - t_start).count() / 1000 << " sec\n";

	first.print();
	second.print();

	//system("pause");

	res.dim_Adjuster();
	res.print();
}

int main()
{
	static int row = 0;
	static int column = 0;
	while (true) {
		system("cls");

		std::cout << "Enter row number: ";
		std::cin >> row;

		if (row <= 0)
			break;

		std::cout << "Enter column number: ";
		std::cin >> column;

		if (column <= 0)
			break;

		system("cls");

		Initializer(row, column);

		system("pause");
	}

	return 0;
}