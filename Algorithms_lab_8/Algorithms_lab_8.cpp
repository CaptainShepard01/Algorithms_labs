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

std::vector<int> naiveSearch(const std::string& pattern, const std::string& text) {
	std::vector<int> result;

	int m = text.length();
	int n = pattern.length();
	for (int i = 0; i <= m - n; i++) {
		bool flag = true;
		for (int j = 0; j < n; j++) {
			if (text[i + j] != pattern[j]) {
				flag = false;
				break;
			}
		}
		if (flag)
			result.push_back(i++);
	}
	return result;
}

std::vector<int> RKSearch(const std::string& pattern, const std::string& text, int primeNumber = 31) {
	std::vector<int> result;

	int patternLenght = pattern.length();
	int textLenght = text.length();
	int i = 0, j = 0;
	int patternHash = 0;
	int textHash = 0;
	int h = (int)pow(ALPHABET, patternLenght - 1) % primeNumber;

	for (i = 0; i < patternLenght; i++) {
		patternHash = (ALPHABET * patternHash + pattern[i]) % primeNumber;
		textHash = (ALPHABET * textHash + text[i]) % primeNumber;
	}

	for (i = 0; i <= textLenght - patternLenght; i++) {
		if (patternHash == textHash) {
			for (j = 0; j < patternLenght; j++) {
				if (text[i + j] != pattern[j])
					break;
			}

			if (j == patternLenght)
				//	std::cout << "Pattern found at index " << i << std::endl;
				result.push_back(i);
		}

		if (i < textLenght - patternLenght) {
			textHash = (ALPHABET * (textHash - text[i] * h) + text[i + patternLenght]) % primeNumber;

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

std::vector<int> KMPSearch(const std::string& pattern, const std::string& text) {
	std::vector<int> result;

	int patternLength = pattern.length();
	int textLength = text.length();

	auto prefixes = prefixFunction(pattern);

	int i = 0;
	int j = 0;

	while (i < textLength) {
		if (pattern[j] == text[i]) {
			j++;
			i++;
		}

		if (j == patternLength) {
			//	std::cout << "Found pattern at index " << i - j << std::endl;
			result.push_back(i - j);

			j = prefixes[j - 1];
		}

		else if (i < textLength && pattern[j] != text[i]) {
			if (j != 0)
				j = prefixes[j - 1];
			else
				i = i + 1;
		}
	}

	return result;
}

void preprocessStrongSuffix(std::vector<int>& shift, std::vector<int>& bpos, std::string pattern) {
	int m = pattern.length();
	int i = m, j = m + 1;

	bpos[i] = j;
	while (i > 0) {
		while (j <= m && pattern[i - 1] != pattern[j - 1]) {
			if (shift[j] == 0)
				shift[j] = j - i;
			j = bpos[j];
		}
		i--;
		j--;
		bpos[i] = j;
	}

	m = pattern.length();

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

std::vector<int> BMSearch(const std::string& pattern, const std::string& text) {
	int m = pattern.size();
	int n = text.size();

	std::vector<int>bpos(m + 1), shift(m + 1, 0), result;

	std::vector<int> badchar = badCharHeuristic(pattern);

	preprocessStrongSuffix(shift, bpos, pattern);

	int s = 0;

	int d_1 = 0;

	while (s <= (n - m)) {
		int j = m - 1;

		while (j >= 0 && pattern[j] == text[s + j])
			j--;

		if (j < 0) {
			//	std::cout << "Pattern occurs at shift = " << s << std::endl;
			result.push_back(s);
			d_1 = s + (s + m < n) ? m - badchar[text[s + m]] : 1;
			s += std::max(shift[0], d_1);
		}

		else
			s += std::max(1, j - badchar[text[s + j]]);
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

std::vector<int> HSearch(const std::string& pattern, const std::string& text) {
	std::vector<int> table = shiftTable(pattern), result;

	int k = 0, m = pattern.length(), n = text.length();

	int i = m - 1;
	while (i < n) {
		k = 0;
		while (k < m && pattern[m - 1 - k] == text[i - k])
			k++;
		if (k == m) {
			result.push_back(i - m + 1);
			i++;
		}
		else
			i += table[text[i]];
	}
	return result;
}

int main() {
	std::cout << naiveSearch("abcd", "abcdababcdabcdcdabcddddcaabcd") << std::endl;
	std::cout << RKSearch("abcd", "abcdababcdabcdcdabcddddcaabcd") << std::endl;
	std::cout << KMPSearch("abcd", "abcdababcdabcdcdabcddddcaabcd") << std::endl;
	std::cout << BMSearch("abcd", "abcdababcdabcdcdabcddddcaabcd") << std::endl;
	std::cout << HSearch("abcd", "abcdababcdabcdcdabcddddcaabcd") << std::endl;
}