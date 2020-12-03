#include <string>
#include <vector>
#include <iostream>
#include <cmath>

#define ALPHABET 256

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
	os << '[';
	for (auto i = v.begin(); i != v.end(); ++i)
		os << ' ' << *i;

	os << " ]";
	return os;
}

//Наївний алгоритм
std::vector<int> N_Search(const std::string& sample, const std::string& string) {
	std::vector<int> result;

	int m = string.length();
	int n = sample.length();
	for (int i = 0; i <= m - n; i++) {
		bool flag = true;
		for (int j = 0; j < n; j++) {
			if (string[i + j] != sample[j]) {
				flag = false;
				break;
			}
		}
		if (flag)
			result.push_back(i++);
	}
	return result;
}

//Рабіна-Карпа
std::vector<int> RK_Search(const std::string& sample, const std::string& string, int primeNumber = 31) {
	std::vector<int> result;

	int substringLenght = sample.length();
	int textLenght = string.length();
	int i = 0, j = 0;
	int substringHash = 0;
	int textHash = 0;
	int h = (int)pow(ALPHABET, substringLenght - 1) % primeNumber;

	for (i = 0; i < substringLenght; i++) {
		substringHash = (ALPHABET * substringHash + sample[i]) % primeNumber;
		textHash = (ALPHABET * textHash + string[i]) % primeNumber;
	}

	for (i = 0; i <= textLenght - substringLenght; i++) {
		if (substringHash == textHash) {
			for (j = 0; j < substringLenght; j++) {
				if (string[i + j] != sample[j])
					break;
			}

			if (j == substringLenght)
				//	std::cout << "sample found at index " << i << std::endl;
				result.push_back(i);
		}

		if (i < textLenght - substringLenght) {
			textHash = (ALPHABET * (textHash - string[i] * h) + string[i + substringLenght]) % primeNumber;

			if (textHash < 0)
				textHash = (textHash + primeNumber);
		}
	}
	return result;
}

std::vector<int> prefixFunction(std::string s) {
	int n = (int)s.length();
	std::vector<int> pi(n, 0);

	for (int i = 1; i < n; i++) {
		int j = pi[i - 1];

		while (j > 0 && s[i] != s[j])
			j = pi[j - 1];

		if (s[i] == s[j])
			j++;

		pi[i] = j;
	}
	return pi;
}

//Кнута-Морріса-Пратта
std::vector<int> KMP_Search(const std::string& sample, const std::string& string) {
	std::vector<int> result;

	int substringLength = sample.length();
	int textLength = string.length();

	auto prefixes = prefixFunction(sample);

	int i = 0;
	int j = 0;

	while (i < textLength) {
		if (sample[j] == string[i]) {
			j++;
			i++;
		}

		if (j == substringLength) {
			//	std::cout << "Found sample at index " << i - j << std::endl;
			result.push_back(i - j);

			j = prefixes[j - 1];
		}

		else if (i < textLength && sample[j] != string[i]) {
			if (j != 0)
				j = prefixes[j - 1];
			else
				i = i + 1;
		}
	}

	return result;
}

void goodSufffixHeuristic(std::vector<int>& shift, std::vector<int>& bpos, std::string sample) {
	int m = sample.length();
	int i = m, j = m + 1;

	bpos[i] = j;
	while (i > 0) {
		while (j <= m && sample[i - 1] != sample[j - 1]) {
			if (shift[j] == 0)
				shift[j] = j - i;
			j = bpos[j];
		}
		i--;
		j--;
		bpos[i] = j;
	}

	m = sample.length();

	i = 0, j = 0;

	j = bpos[0];

	for (i = 0; i <= m; i++) {
		if (shift[i] == 0)
			shift[i] = j;

		if (i == j)
			j = bpos[j];
	}
}

std::vector<int> badCharHeuristic(const std::string& str) {
	int i = 0;
	int size = str.length();

	std::vector<int> badchar(ALPHABET, -1);

	for (i = 0; i < size; i++)
		badchar[(int)str[i]] = i;

	return badchar;
}

//Бойєра-Мура
std::vector<int> BM_Search(const std::string& sample, const std::string& string) {
	int m = sample.size();
	int n = string.size();

	std::vector<int>bpos(m + 1), shift(m + 1, 0), result;

	std::vector<int> badchar = badCharHeuristic(sample);

	goodSufffixHeuristic(shift, bpos, sample);

	int s = 0;

	int d_1 = 0;

	while (s <= (n - m)) {
		int j = m - 1;

		while (j >= 0 && sample[j] == string[s + j])
			j--;

		if (j < 0) {
			//	std::cout << "sample occurs at shift = " << s << std::endl;
			result.push_back(s);
			d_1 = s + (s + m < n) ? m - badchar[string[s + m]] : 1;
			s += std::max(shift[0], d_1);
		}

		else
			s += std::max(1, j - badchar[string[s + j]]);
	}

	return result;
}

std::vector<int> shiftTable(const std::string& p) {
	std::vector<int> table(ALPHABET, p.length());

	for (int j = 0; j < p.length() - 1; j++) {
		table[(int)p[j]] = p.length() - 1 - j;
	}
	return table;
}

//Хорспула
std::vector<int> H_Search(const std::string& sample, const std::string& string) {
	std::vector<int> table = shiftTable(sample), result;

	int k = 0, m = sample.length(), n = string.length();

	int i = m - 1;
	while (i < n) {
		k = 0;
		while (k < m && sample[m - 1 - k] == string[i - k])
			k++;
		if (k == m) {
			result.push_back(i - m + 1);
			i++;
		}
		else
			i += table[string[i]];
	}
	return result;
}

int main() {
	std::cout << "Naive: " << N_Search("bcd", "hgjbcdhgbcdbdbcd") << std::endl;
	std::cout << "Rabin-Karp: " << RK_Search("bcd", "hgjbcdhgbcdbdbcd") << std::endl;
	std::cout << "Knuth-Morris-Pratt: " << KMP_Search("bcd", "hgjbcdhgbcdbdbcd") << std::endl;
	std::cout << "Boyer-Moore: " << BM_Search("bcd", "hgjbcdhgbcdbdbcd") << std::endl;
	std::cout << "Horspool: " << H_Search("bcd", "hgjbcdhgbcdbdbcd") << std::endl;
}