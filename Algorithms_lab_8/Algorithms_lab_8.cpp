#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>


int naive_Search(const std::string& str, const std::string& substr) { // наївний алгоритм
	int m = str.length();
	int n = substr.length();
	for (int i = 0; i <= m - n; i++) {
		bool flag = true;
		for (int j = 0; j < n; j++) {
			if (str[i + j] != substr[j]) {
				flag = false;
				break;
			}
		}
		if (flag) return i;
	}
	return -1;
}

int RK_search(const std::string& str, const std::string& substr) { 	// Алгоритм Рабіна-Карпа
	const int p = 31;
	std::vector<long long> p_pow(std::max(str.length(), substr.length()));
	p_pow[0] = 1;
	for (size_t i = 1; i < p_pow.size(); ++i)
		p_pow[i] = p_pow[i - 1] * p;

	std::vector<long long> h(str.length());
	for (size_t i = 0; i < str.length(); ++i)
	{
		h[i] = (str[i] - 'a' + 1) * p_pow[i];
		if (i)  h[i] += h[i - 1];
	}

	long long h_s = 0;
	for (size_t i = 0; i < substr.length(); ++i)
		h_s += (substr[i] - 'a' + 1) * p_pow[i];

	for (size_t i = 0; i + substr.length() - 1 < str.length(); ++i)
	{
		long long cur_h = h[i + substr.length() - 1];
		if (i)  cur_h -= h[i - 1];
		if (cur_h == h_s * p_pow[i])
			return i;
	}

	return -1;
}

int Horsool_search(const std::string& str, const std::string& substr) { // Алгоритм Хорспула
	unsigned int skip_table[ALPHABET];
	int i;

	for (i = 0; i < ALPHABET; i++)
		skip_table[i] = substr.size();

	for (i = 0; i < substr.size() - 1; i++)
		skip_table[(int)what[i]] = substr.size() - i - 1;

	i = 0;
	while (i <= str.size() - substr.size()) {
		if (str[i + substr.size() - 1] == substr[substr.size() - 1])
			if (compare(str.begin() + i, substr.begin(), substr.size() - 2) == true)
				return i;

		i += skip_table[(int)str[i + substr.size() - 1]];
	}

	return -1;
}

int BM_search(const std::string& str, const std::string& substr) { 	// Алгоритм Боєра-Мура
	std::vector<int> good_shift(substr.size() + 1, substr.size());
	std::vector<int> bad_char(ALPHABET, substr.size());

	std::vector<int> z(substr.size(), 0);
	for (int j = 1, maxZidx = 0, maxZ = 0; j < substr.size(); ++j) {
		if (j <= maxZ) z[j] = std::min(maxZ - j + 1, z[j - maxZidx]);
		while (j + z[j] < substr.size() && str[substr.size() - 1 - z[j]] == str[substr.size() - 1 - (j + z[j])]) z[j]++;
		if (j + z[j] - 1 > maxZ) {
			maxZidx = j;
			maxZ = j + z[j] - 1;
		}
	}
	for (int j = substr.size() - 1; j > 0; j--) good_shift[substr.size() - z[j]] = j; //цикл №1
	for (int j = 1, r = 0; j <= substr.size() - 1; j++) //цикл №2
		if (j + z[j] == substr.size())
			for (; r <= j; r++)
				if (good_shift[r] == substr.size()) good_shift[r] = j;

	for (int j = 0; j < substr.size() - 1; j++) bad_char[(int)substr[j]] = substr.size() - j - 1;

	int i = 0;
	while (i <= substr.size() - str.size()) {
		int j = 0;
		for (j = substr.size() - 1; j >= 0 && substr[j] == str[i + j]; --j);
		if (j < 0) {
			return i;
		}
		else i += std::max((good_shift[j + 1]), (int)(bad_char[(int)str[i + j]] - substr.size() + j + 1));
	}
	return -1;
}

int KMP_search(const std::string& str, const std::string& substr) {	// Алгоритм Кнута-Моріса-Прата
	std::vector<int> z;
	z.resize(substr.size());
	z[0] = 0;
	for (int i = 1; i < z.size(); ++i) {
		int pos = z[i - 1];
		while (pos > 0 && substr[pos] != substr[i])
			pos = z[pos - 1];
		z[i] = pos + (substr[pos] == substr[i] ? 1 : 0);
	}

	int pos = 0;
	for (int i = 0; i < str.size(); ++i) {
		while (pos > 0 && (pos >= substr.size() || substr[pos] != str[i]))
			pos = z[pos - 1];
		if (str[i] == substr[pos]) pos++;
		if (pos == substr.size())
			return (i - pos + 1);
	}
	return -1;
}


int main()
{
    std::cout << "Hello World!\n";
}
