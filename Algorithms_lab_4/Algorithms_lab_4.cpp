#include <algorithm> 
#include <iostream> 
#include <vector> 

void display(int* array, int size) {
	for (int i = 1; i <= size; i++)
		std::cout << array[i] << " ";
	std::cout << std::endl;
}
int getMax(int array[], int size) {
	int max = array[1];
	for (int i = 2; i <= size; i++) {
		if (array[i] > max)
			max = array[i];
	}
	return max; //the max element from the array
}
void countSort(int* array, int size) {
	int* output = new int[size + 1];
	int max = getMax(array, size);
	int* count = new int[max + 1];     //create count array (max+1 number of elements)
	for (int i = 0; i <= max; i++)
		count[i] = 0;     //initialize count array to all zero
	for (int i = 1; i <= size; i++)
		count[array[i]]++;     //increase number count in count array.
	for (int i = 1; i <= max; i++)
		count[i] += count[i - 1];     //find cumulative frequency
	for (int i = size; i >= 1; i--) {
		output[count[array[i]]] = array[i];
		count[array[i]] -= 1; //decrease count for same numbers
	}
	for (int i = 1; i <= size; i++) {
		array[i] = output[i]; //store output array to main array
	}
}

//void Counting_Sort(int a[], int n) {
//	int* c = new int[2];
//	for (int i = 0; i < 2; ++i) {
//		c[i] = 0;
//	}
//
//	int* b = new int[n];
//
//	for (int j = 0; j < n; ++j) {
//		c[a[j]] = c[a[j]] + 1;
//	}
//
//	for (int i = 1; i < 2; ++i) {
//		c[i] += c[i - 1];
//	}
//
//	for (int j = n - 1; j >= 0; --j) {
//		b[c[a[j]]] = a[j];
//		c[a[j]] = c[a[j]] - 1;
//	}
//
//	for (int i = 0; i < n; ++i) {
//		a[i] = b[i];
//	}
//}


// Function to sort arr[] of size n using bucket sort 
template<typename T>
void bucketSort(T* arr, int n)
{
	// 1) Create n empty buckets 
	std::vector<T>* b = new std::vector<T>[2];

	// 2) Put array elements in different buckets 
	for (int i = 0; i < n; i++) {
		//int bi = arr[i]; // Index in bucket 
		b[arr[i]].push_back(arr[i]);
	}

	// 3) Sort individual buckets 
	for (int i = 0; i < n; i++)
		std::stable_sort(b[i].begin(), b[i].end());

	// 4) Concatenate all buckets into arr[] 
	int index = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < b[i].size(); j++)
			arr[index++] = b[i][j];

	delete[] b;
}


int main()
{
	//float arr[] = { 0.897, 0.565, 0.656, 0.1234, 0.665, 0.3434 };
	//int n = sizeof(arr) / sizeof(arr[0]);

	int a[] = { 0,1,1,0,0,1,0,1,0 };

	//Counting_Sort(a, sizeof(a)/sizeof(int));
	bucketSort(a, 9);
		std::cout << "Sorted array is \n";
	for (int i = 0; i < 9; i++)
		std::cout << a[i] << " ";
	return 0;
}