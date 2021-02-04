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


int maxPrefix(std::string substr)
{
	int res = 0;

	for (int i = 0, j = substr.length() - 1; i < substr.length() - 1; i++, j--)
	{
		if (substr.substr(0, i + 1) == substr.substr(j, i + 1))
		{
			res = i + 1;
		}
	}

	return res;
}

std::vector<int> goodSuff(std::string substr)
{
	int length = substr.length();

	int max_shift = length - maxPrefix(substr);

	std::vector<int> shifts(substr.length(), max_shift);

	shifts[0] = 0;

	for (int i = 1; i < length; ++i) {
		std::string i_suf = substr.substr(length - i, i);

		std::string to_find_in = substr.substr(0, length - 1);

		int shift_from_beginning = (signed)to_find_in.rfind(i_suf);

		if (shift_from_beginning != -1) {
			shifts[i] = length - (shift_from_beginning + i_suf.length());
		}
	}

	return shifts;
}

std::vector<int> badChar(std::string substr) {
	std::vector<int> shifts(256, substr.length());

	for (int j = 0; j < substr.length() - 1; j++)
		shifts[(int)substr[j]] = substr.length() - j - 1;

	return shifts;
}

std::vector<int> BM_Search(std::string substr, std::string str)
{
	//std::vector<int> shifts(substr.size() + 1, 0);
	std::vector<int> res;
	std::vector<int> badC = badChar(substr);
	std::vector<int> shifts = goodSuff(substr);

	int pos = 0, pos_shift = 0;

	while (pos <= (str.size() - substr.size())) {
		int k = 0;
		int j = substr.size() - 1;

		while (j >= 0 && substr[j] == str[pos + j]) {
			j--;
			k++;
		}

		if (pos + j >= 0)
			pos_shift = std::max(badC[(int)str[pos + j]] - k, 1);
		else
			pos_shift = 1;

		if (k == 0) {
			pos += pos_shift;
		}
		else {
			if (j < 0) {
				res.push_back(pos);
				pos += pos_shift;
			}
			else {
				pos += std::max(pos_shift, shifts[k]);
			}
		}
	}

	return res;
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