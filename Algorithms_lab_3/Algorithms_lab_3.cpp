#include <iostream>
#include <vector>
#include <exception>

class D_ary_Heap {
private:
	int d;
	std::vector<int> mas;

	int get_Parent_Index(int i) const {
		if (i >= 0 && i < mas.size())
			return (i - 1) / d;
	}

	int get_Nth_Child_Index(int i, int n) const {
		if (i >= 0 && i < mas.size() && n >= 1 && n <= d)
			return i * d + n;
	}

	void build_Max_Heap() {
		for (int i = mas.size() / d; i >= 0; i--)
			max_Heapify(i, mas.size());
	}

	int heap_Max() const {
		if (mas.size() > 0)
			return mas[0];
	}

public:
	D_ary_Heap(int d)
		:d(d) {
		if (d <= 1)
			throw std::invalid_argument("Can`t create a heap with that value of d.");
	}

	template <typename Iterator>
	D_ary_Heap(Iterator begin, Iterator end, int d)
		: d(d),
		mas(begin, end) {
		if (d <= 1)
			throw std::invalid_argument("Can`t create a heap with that value of d.");
		build_Max_Heap();
	}

	void max_Heapify(int index, int currentSize) {
		if (index < 0 || index > currentSize)
			return;

		int maxIndex = index;
		int currentIndex = get_Nth_Child_Index(index, 1);
		int currentMax = mas[index];

		for (int n = currentSize, i = 1; currentIndex < n && i <= d; currentIndex++, i++) {
			int temp = mas[get_Nth_Child_Index(index, i)];
			currentMax = std::max(temp, currentMax);

			if (temp == currentMax)
				maxIndex = currentIndex;
		}

		if (index != maxIndex) {
			std::swap(mas[index], mas[maxIndex]);
			max_Heapify(maxIndex, currentSize);
		}
	}

	void heapsort() {
		build_Max_Heap();

		for (int i = mas.size() - 1; i >= 1; i--) {
			std::swap(mas[0], mas[i]);
			max_Heapify(0, i);
		}
	}

	int heap_Extract_Max() {
		if (mas.size() == 0)
			throw std::invalid_argument("Heap is empty!");

		int max = heap_Max();

		mas[0] = mas[mas.size() - 1];
		mas.pop_back();
		max_Heapify(0, mas.size());

		return max;
	}

	void heap_Increase_Key(int i, int key) {
		if (mas[i] >= key)
			return;

		mas[i] = key;

		while (i > 0 && mas[get_Parent_Index(i)] < mas[i]) {
			std::swap(mas[get_Parent_Index(i)], mas[i]);
			i = get_Parent_Index(i);
		}
	}

	void max_Heap_Insert(int key) {
		mas.push_back(INT_MIN);
		heap_Increase_Key(mas.size() - 1, key);
	}

	auto begin() const {
		return mas.begin();
	}

	auto end() const {
		return mas.end();
	}
};

std::ostream& operator<<(std::ostream& out, const D_ary_Heap& heap) {
	for (const auto& num : heap)
		out << num << ' ';

	return out;
}

int main() {
	std::vector<int> v{ 2, 3, 6, 7, 8, 9, 10, -5, -10 };

	D_ary_Heap heap(v.begin(), v.end(), 5);

	std::cout << heap << std::endl;

	heap.heap_Increase_Key(3, 25);
	std::cout << heap << std::endl;

	heap.max_Heap_Insert(100);
	std::cout << heap << std::endl;

	heap.heapsort();
	std::cout << heap << std::endl;
}