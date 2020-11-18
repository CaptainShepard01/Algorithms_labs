﻿#include <vector>
#include <random>
#include <iostream>
#include <exception>

template<typename T>
class Container {
	int key;
	T data;

public:
	Container(int key = 0, T data = {})
		:key(key),
		data(data) {
		if (key != 0 && key != 1)
			throw std::invalid_argument("You may use ONLY 0 OR 1 for a key.");
	}

	bool operator< (const Container<T>& rhs) {
		return key < rhs.key;
	}

	int getKey() const {
		return key;
	}

	template<typename T>
	friend std::ostream& operator<<(std::ostream& out, const Container<T>& c);
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Container<T>& c) {
	out << c.key << " -> " << c.data;
	return out;
}

auto getRandomVector(int n) {
	std::vector<Container<double>> data;

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0, 2);

	for (int i = 0; i < n; i++) {
		data.push_back(Container<double>({ (int)dist(mt), (double)i }));
	}

	return data;
}

//	linear | stable
template<typename T>
void countingSort(typename std::vector<T>::iterator begin, typename std::vector<T>::iterator end, int min = 0, int max = 1) {
	int amount = std::distance(begin, end);
	if (amount == 0 || min > max)
		return;

	std::vector<int> counts(max - min + 1);
	std::vector<T> result(amount);

	for (auto it = begin; it < end; it = std::next(it)) {
		++counts[it->getKey() - min];
	}

	counts[0]--;
	for (int i = 1, k = max - min + 1; i < k; i++) {
		counts[i] += counts[i - 1];
	}

	for (int i = amount - 1; i >= 0; i--) {
		int index = std::next(begin, i)->getKey();
		result[counts[index]] = *std::next(begin, i);
		counts[std::next(begin, i)->getKey()]--;
	}

	std::copy(result.begin(), result.end(), begin);
}

//	const space | stable
template<typename Iterator>
void insertionSort(Iterator begin, Iterator end) {
	for (auto it = std::next(begin); it < end; it++) {
		for (auto j = it; j > begin; j--) {
			if (*j < *std::prev(j))
				std::iter_swap(j, std::prev(j));
		}
	}
}

//	const space | linear
template<typename Iterator>
void edgeSort(Iterator begin, Iterator end) {
	if (begin >= end)
		return;

	end = std::prev(end);
	while (begin < end) {
		while (begin->getKey() != 1 && begin < end) {
			begin = std::next(begin);
		}

		while (end->getKey() != 0 && begin < end) {
			end = std::prev(end);
		}

		if (begin > end)
			break;

		std::iter_swap(begin, end);
	}
}

template<typename T>
void printVector(const std::vector<T>& v) {
	for (const auto& item : v) {
		std::cout << item << " | ";
	}
	std::cout << std::endl;

}

int main() {
	std::vector<Container<double>> data = getRandomVector(10);

	std::cout << "Generated :\n";
	printVector(data);

	//countingSort<Container<double>>(data.begin(), data.end());
	//insertionSort(data.begin(), data.end());
	edgeSort(data.begin(), data.end());

	std::cout << "Sorted :\n";
	printVector(data);
}