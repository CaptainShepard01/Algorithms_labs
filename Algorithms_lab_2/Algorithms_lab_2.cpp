#include <iostream>
#include <algorithm>
#include <array>
#include <random>      
#include <chrono>       

int Partition(int* mas, int beg, int end, int checker) {
	int x = checker;
	for (int i = 0; i <= end; ++i) {
		if (mas[i] == checker) {
			int tmp = mas[i];
			mas[i] = mas[end];
			mas[end] = tmp;
		}
	}

	int i = beg-1;
	int tmp = 0;
	for (int j = beg; j < end; ++j) {
		if (mas[j] <= x) {
			i++;
			tmp = mas[i];
			mas[i] = mas[j];
			mas[j] = tmp;
		}
	}
	tmp = mas[i + 1];
	mas[i + 1] = mas[end];
	mas[end] = tmp;
	return i + 1;
}

void QuickSortDouble_alt(int* mas_1, int* mas_2, int beg, int end) {
	int tmp = mas_2[end];
	int tmp_1 = 0;
	if (beg < end) {
		for (int i = beg; i < end; ++i) {
			if (mas_1[i] == tmp) {
				tmp_1=mas_1[i];
				int t = mas_1[end];
				mas_1[end] = mas_1[i];
				mas_1[i] = t;
				break;
			}
		}

		int iter = Partition(mas_1, beg, end, tmp);
		Partition(mas_2, beg, end, tmp_1);
		QuickSortDouble_alt(mas_1, mas_2, beg, iter - 1);
		QuickSortDouble_alt(mas_1, mas_2, iter + 1, end);
	}
}

void QuickSortDouble(int* mas_1, int* mas_2, int beg, int end) {
	/*for (int i = beg; i <= end; ++i)
		std::cout << mas_1[i] << ' ';
	std::cout << '\n';

	for (int i = beg; i <= end; ++i)
		std::cout << mas_2[i] << ' ';
	std::cout << "\n\n";*/

	int i = beg, j = end, i1 = beg, j1 = end;
	int mid = (end + beg) / 2;

	int first = mas_1[mid];
	int tmp;

	for (int k = beg; k <= end; ++k) {
		if (mas_2[k] == first) {
			tmp = mas_2[k];
			mas_2[k] = mas_2[mid];
			mas_2[mid] = tmp;
			break;
		}
	}

	int second = mas_2[mid];

	do {
		while (mas_2[i] < first)
			i++;

		while (first < mas_2[j])
			j--;

		if (i <= j) {
			tmp = mas_2[i];
			mas_2[i] = mas_2[j];
			mas_2[j] = tmp;

			i++;
			j--;
		}

	} while (i < j);

	do {
		while (mas_1[i1] < second)
			i1++;

		while (second < mas_1[j1])
			j1--;

		if (i1 <= j1) {
			tmp = mas_1[i1];
			mas_1[i1] = mas_1[j1];
			mas_1[j1] = tmp;

			i1++;
			j1--;
		}
	} while (i1 < j1);


	if (beg < j)
		QuickSortDouble(mas_1, mas_2, beg, j);

	if (i < end)
		QuickSortDouble(mas_1, mas_2, i, end);
}

int main()
{
	int n = 0;
	std::cout << "Enter number of elements: ";
	std::cin >> n;

	int* mas_1 = new int[n];
	int* mas_2 = new int[n];

	std::vector<int> first, second, sort_1, sort_2;

	static std::random_device rd;
	static std::mt19937 g(rd());
	for (int i = 0; i < n; ++i) {
		mas_1[i] = i;
		mas_2[i] = i;
		first.push_back(i);
		second.push_back(i);
	}
	std::shuffle(mas_1, mas_1 + n, g);
	std::shuffle(mas_2, mas_2 + n, g);

	//for (int i = 0; i < n; ++i)
	//	std::cout << mas_1[i] << ' ';
	//std::cout << '\n';
	//
	//for (int i = 0; i < n; ++i)
	//	std::cout << mas_2[i] << ' ';
	//std::cout << "\n\n";

	int chosen = 0;
	
	std::cout << "\nWhich sort to choose:\n1 --> With partition.\n2 --> Without partition.\n";
	std::cin >> chosen;
	switch (chosen) {
	case 1:QuickSortDouble_alt(mas_1, mas_2, 0, n - 1);
		break;
	case 2:QuickSortDouble(mas_1, mas_2, 0, n - 1);
		break;
	}
	
	
	for (int i = 0; i < n; ++i) {
		sort_1.push_back(mas_1[i]);
		sort_2.push_back(mas_2[i]);
	}

	if (sort_1 == first && sort_2 == second) {
		std::cout << "Correct!" << "\n\n";
	}
	else 
		std::cout << "Incorrect!" << "\n\n";

	//for (int i = 0; i < n; ++i)
	//	std::cout << mas_1[i] << ' ';
	//std::cout << '\n';
	//
	//for (int i = 0; i < n; ++i)
	//	std::cout << mas_2[i] << ' ';
	//std::cout << '\n';

	system("pause");
	delete[] mas_1, mas_2;
	return 0;
}