#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <conio.h>
#include <vector>
#include <ctime>
#include <algorithm>
#include <random>
#include <string>
#include <windows.h> 
#include <direct.h>
#include <filesystem>
#include "Header.h"
using namespace std;

void printBinaryFile(const char* filename);
void initialize(int& numb, int& maxRAM, int& fileCount);
void randomizer(const char* filename, int count);
std::pair<std::vector<int>, int> chunkNumberDeterminer(const int& fileCount, const int& elementsCount, const int& maxRAM, int& total_chunks);
std::vector<char*> chunkSeparator(const char* filename, const int& chunkSize, const std::vector<int>& chunks);
void removeIntFromTheTop(const char* filename, const int N);
bool isEmptyFile(const char* filename);
int fileMerge(std::vector<char*> filenames, const int& chunkSize, const int& total_chunks);
void printAllBinaries(std::vector<char*> filenames, int outputIndex);
std::vector<int> readBinaryFile(const char* filename) {
	FILE* file;

	std::vector<int> result;

	fopen_s(&file, filename, "rb");

	if (file == nullptr)
		throw std::runtime_error("");

	int tempNumber = 0;

	while (fread_s(&tempNumber, sizeof(int), sizeof(int), 1, file)) {
		result.push_back(tempNumber);
	}

	if (file)
		fclose(file);

	return result;
}

int main() {
	HANDLE  hConsole;
	int k;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15);

	//SetConsoleTextAttribute(hConsole, 0);

	if (_chdir(".\\Files") == -1) {
		if (_mkdir(".\\Files") == 0)
			_chdir(".\\Files");
	}

	int total_chunks = 0;
	int elementsCount = 11, maxRAM = 5, fileCount = 3;
	const char* filename = { "Input" };

	initialize(elementsCount, maxRAM, fileCount);
	randomizer(filename, elementsCount);

	//printBinaryFile(filename);
	//system("pause");

	std::vector<int> expected_output = readBinaryFile(filename);
	std::sort(expected_output.begin(), expected_output.end());

	std::pair<std::vector<int>, int> my_pair = chunkNumberDeterminer(fileCount, elementsCount, maxRAM, total_chunks);
	vector<int> chunks = my_pair.first;

	int realChunkSize = std::ceil((double)elementsCount / my_pair.second);

	vector<char*> filenames = chunkSeparator(filename, realChunkSize, chunks);

	//printAllBinaries(filenames, 2);
	//system("pause");

	double t = clock();
	
	int res = fileMerge(filenames, realChunkSize, total_chunks);

	std::vector<int> real_output = readBinaryFile(filenames[res]);

	//printBinaryFile(fileMerge(filenames, realChunkSize, total_chunks));

	t = (clock() - t) * 0.001;
	cout << endl << t << endl;

	for (int i = 0; i < filenames.size(); ++i) {
		if (i == res)
			continue;
		remove(filenames[i]);
	}

	if (real_output == expected_output) {
		std::cout << "\nSuccessful sort!\n\n";
	}
	else std::cout << "\nError!\n\n";


	//system("pause");

	return 0;
}



void printBinaryFile(const char* filename) {
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15);
	int x;
	int counter = 0;
	FILE* file;
	if (isEmptyFile(filename)) {
		std::cout << "\nEmpty file!\n\n";
		return;
	}
	fopen_s(&file, filename, "rb");
	while (fread_s(&x, sizeof(int), sizeof(int), 1, file)) {
		std::cout << setw(5) << x << ' ';
		++counter;
	}
	std::cout << "\nEnd of file!\n\nThere are " << counter << " integers.\n";
	fclose(file);
}

void initialize(int& numb, int& maxRAM, int& fileCount) {
	do {
		system("cls");
		std::cout << "Number of integers: ";
		std::cin >> numb;
		if (numb <= 0) {
			std::cout << "Must be positive!\n";
			system("pause");
		}
	} while (numb <= 0);
	do {
		system("cls");
		std::cout << "MaxRAM: ";
		std::cin >> maxRAM;
		if (maxRAM < 2) {
			std::cout << "Must be at least 2!\n";
			system("pause");
		}
	} while (maxRAM < 2);
	do {
		system("cls");
		std::cout << "Number of files: ";
		std::cin >> fileCount;
		if (fileCount < 3) {
			std::cout << "Must be at least 3!\n";
			system("pause");
		}
	} while (fileCount < 3);
	/*std::cout << "Enter lower border set of numbers: ";
	std::cin >> low;
	std::cout << "Enter upper border set of numbers: ";
	std::cin >> high;*/
	return;
}

void randomizer(const char* filename, int count)
{
	FILE* f;
	fopen_s(&f, filename, "wb");
	int* mas = new int[count];
	int gap = count;

	for (int i = 0; i < count; ++i) {
		mas[i] = rand() % gap - count / 2;
	}

	fwrite(mas, sizeof(int), count, f);

	delete[] mas;
	fclose(f);
	return;
}

std::pair<std::vector<int>, int> chunkNumberDeterminer(const int& fileCount, const int& elementsCount, const int& maxRAM, int& total_chunks) {
	std::vector<int> chunks(fileCount);
	chunks[0] = 1;
	total_chunks++;

	int capacity = 1, amountOfChunks = std::ceil((double)elementsCount / maxRAM);

	while (capacity < amountOfChunks) {
		std::sort(chunks.begin(), chunks.end());

		int temp = chunks[fileCount - 1];

		capacity -= temp;

		chunks[fileCount - 1] = 0;

		for (int i = 0; i < fileCount - 1; i++) {
			chunks[i] += temp;
			total_chunks += temp;
			capacity += temp;
		}
	}

	return make_pair(chunks, capacity);
}

std::vector<char*> chunkSeparator(const char* filename, const int& chunkSize, const std::vector<int>& chunks) {
	FILE* origin;

	fopen_s(&origin, filename, "rb");

	int* memory = new int[chunkSize];

	std::vector<char*> filenames(chunks.size());

	for (size_t i = 0, n = filenames.size(); i < n; i++) {
		filenames[i] = new char[10];
		char temp[10];
		_itoa_s(i, temp, 10);
		strcpy_s(filenames[i], sizeof(filenames[i]), temp);
	}

	for (size_t i = 0, n = chunks.size(); i < n; i++) {
		FILE* tempFile;
		int size = 0;
		fopen_s(&tempFile, filenames[i], "wb");

		for (int j = 0, m = chunks[i]; origin && tempFile && j < m; j++) {
			size = fread(memory, sizeof(int), chunkSize, origin);

			//InsertSort(memory, size);

			std::sort(memory, memory + size);

			fwrite(memory, sizeof(int), size, tempFile);
		}

		fclose(tempFile);
	}

	delete[] memory;

	fclose(origin);

	return filenames;
}

void removeIntFromTheTop(const char* filename, const int N) {

	FILE* file;
	fopen_s(&file, filename, "rb");

	if (file)
		fseek(file, N * sizeof(int), SEEK_SET);

	FILE* temp;
	fopen_s(&temp, "Temp", "wb");

	int num;

	while (file && temp && fread(&num, sizeof(int), 1, file))
		fwrite(&num, sizeof(int), 1, temp);

	freopen_s(&file, filename, "wb", file);
	freopen_s(&temp, "Temp", "rb", temp);

	while (file && temp && fread(&num, sizeof(int), 1, temp))
		fwrite(&num, sizeof(int), 1, file);

	if (temp)
		fclose(temp);
	if (file)
		fclose(file);
	remove("Temp");
}

bool isEmptyFile(const char* filename) {
	FILE* file;
	fopen_s(&file, filename, "rb");
	int x;
	if (fread_s(&x, sizeof(int), sizeof(int), 1, file) == 0) {
		if (file)
			fclose(file);
		return 1;
	}
	if (file)
		fclose(file);
	return 0;
}

int minIndex(const vector<int> input, const int former_int) {
	int min = 0;
	for (int i = 0; i < input.size(); ++i) {
		if (input[i] < input[min] && former_int <= input[i]) {
			min = i;
		}
	}
	if (input[min] == INT_MAX)
		return -1;
	return min;
}

void printPercentage(const double& percentage) {
	int numb = (int)percentage % 100;
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 2);
	std::cout << percentage << " %" << '\n';
	for (int i = 0; i < numb; ++i) {
		std::cout << "#";
	}

	SetConsoleTextAttribute(hConsole, 4);

	for (int i = numb; i < 100; ++i) {
		std::cout << "#";
	}

	SetConsoleTextAttribute(hConsole, 2);
	std::cout << '\n';
}

int nonEmptyFilesCount(vector<char*> filenames) {
	int cnt = 0;
	FILE* iter_file;
	for (int i = 0; i < filenames.size(); ++i) {
		int x;
		fopen_s(&iter_file, filenames[i], "rb");
		if (fread_s(&x, sizeof(int), sizeof(int), 1, iter_file) != 0)
			cnt++;
		fclose(iter_file);
	}

	return cnt;
}

int emptyFilesCount(const int nonEmpty, const int fileCount) {
	return fileCount - nonEmpty;
}

bool isChunksRead(std::vector<bool> isChunk) {
	for (int i = 0; i < isChunk.size(); ++i) {
		if (!isChunk[i])
			return false;
	}
	return true;
}

bool isAllFilesRead(std::vector<bool> isFile) {
	int cnt = 0;

	for (int i = 0; i < isFile.size(); ++i) {
		if (isFile[i])
			cnt++;
	}

	if (cnt == isFile.size() - 1)
		return true;

	return false;
}

void printAllBinaries(std::vector<char*> filenames, int outputIndex) {
	for (int i = 0, n = filenames.size(); i < n; i++) {
		std::cout << "[" << i << "]";
		if (i == outputIndex)
			std::cout << " OUTPUT";
		std::cout << '\n';

		printBinaryFile(filenames[i]);
		std::cout << "\n";
	}
}

void copyFile(const char* result, const char* source) {
	FILE* input, * output;
	fopen_s(&input, source, "rb");
	fopen_s(&output, result, "wb");
	int temp;
	while (input && output && fread_s(&temp, sizeof(int), sizeof(int), 1, input))
		fwrite(&temp, sizeof(int), 1, output);
	fclose(input);
	fclose(output);
}

const char* fileMerge_1(std::vector<char*> filenames, const int& chunkSize, const int& total_chunks) {
	int fileCount = filenames.size(), outputIndex = 0;
	bool isEnd = 0;
	std::vector<FILE*> files(fileCount);
	std::vector<int> container(fileCount);
	std::vector<bool> isChunk(fileCount);

	int chunks = 0;
	//int cur_iter = 0;
	//double percentage = ((double)chunks / total_chunks) * 100;

	for (int i = 0; i < fileCount; ++i) {
		if (isEmptyFile(filenames[i])) {
			outputIndex = i;
			container[i] = INT_MAX;
			isChunk[i] = 1;
			fopen_s(&files[i], filenames[i], "wb");
		}
		else {
			fopen_s(&files[i], filenames[i], "rb");
			if (fread_s(&container[i], sizeof(int), sizeof(int), 1, files[i])) {
				isChunk[i] = 0;
			}
			else {
				isChunk[i] = 1;
				container[i] = INT_MAX;
			}
		}
	}
	int former_int = INT_MIN;
	int empty = INT_MIN;
	int chunksnumb = 0;
	while (!isEnd) {
		int min = minIndex(container, former_int);
		former_int = container[min];
		if (min == -1) {
			for (int i = 0; i < fileCount; ++i)
				fclose(files[i]);
			//const char* output = new char[7];
			//output = { "Output" };
			rename(filenames[outputIndex], "Output");
			//copyFile(output, filenames[outputIndex]);
			return filenames[outputIndex];
		}

		fwrite(&container[min], sizeof(int), 1, files[outputIndex]);

		if (fread_s(&container[min], sizeof(int), sizeof(int), 1, files[min])) {
			if (container[min] < former_int) {
				isChunk[min] = 1;
				chunksnumb++;
			}
		}
		else {
			container[min] = INT_MAX;
			empty = min;
			chunksnumb++;
			isChunk[min] = 1;
		}

		if (isChunksRead(isChunk) && empty != INT_MIN) {
			freopen_s(&files[outputIndex], filenames[outputIndex], "rb", files[outputIndex]);
			freopen_s(&files[empty], filenames[empty], "wb", files[empty]);
			former_int = INT_MIN;
			int tmp = outputIndex;
			outputIndex = empty;
			empty = tmp;
			fread_s(&container[empty], sizeof(int), sizeof(int), 1, files[empty]);
			int chunkNumb = 0;
			for (int i = 0; i < fileCount; ++i) {
				if (i != outputIndex) {
					if (container[i] == INT_MAX) {
						isChunk[i] = 1;
						chunkNumb++;
					}
					else {
						isChunk[i] = 0;
					}
				}
				else {
					isChunk[i] = 1;
				}
			}
			if (chunkNumb == (fileCount - 2)) {
				for (int i = 0; i < fileCount; ++i)
					fclose(files[i]);
				const char* output = new char[7];
				output = { "Output" };
				copyFile(output, filenames[empty]);
				return output;
			}
			empty = INT_MIN;
		}
		else if (isChunksRead(isChunk) && empty == INT_MIN) {
			former_int = INT_MIN;
		}
	}

	system("cls");
	for (int i = 0; i < filenames.size(); ++i) {
		if (!isEmptyFile(filenames[i])) {
			const char* output = new char[7];
			output = { "Output" };
			copyFile(output, filenames[i]);
			return output;
		}
	}
}

int fileMerge(std::vector<char*> filenames, const int& chunkSize, const int& total_chunks) {
	int fileCount = filenames.size(), outputIndex = 0;
	bool isEnd = 0;
	int result;
	std::vector<FILE*> files(fileCount);
	std::vector<int> container(fileCount);
	std::vector<bool> isChunk(fileCount);

	int chunks = 0;
	double percentage = ((double)chunks / total_chunks) * 100;

	for (int i = 0; i < fileCount; ++i) {
		if (isEmptyFile(filenames[i])) {
			outputIndex = i;
			container[i] = INT_MAX;
			isChunk[i] = 1;
			fopen_s(&files[i], filenames[i], "wb");
		}
		else {
			fopen_s(&files[i], filenames[i], "rb");
			if (fread_s(&container[i], sizeof(int), sizeof(int), 1, files[i])) {
				isChunk[i] = 0;
			}
			else {
				isChunk[i] = 1;
				container[i] = INT_MAX;
			}
		}
	}
	int former_int = INT_MAX;
	int empty = INT_MAX;
	int min = -1;
	while (true) {
		isChunk.assign(fileCount, false);
		while (true) {
			min = -1;

			for (int i = 0; i < fileCount; ++i) {
				if (i == outputIndex || isChunk[i] == true || container[i] == INT_MAX)
					continue;
				if (min != -1) {
					if (container[i] < container[min])
						min = i;
				}
				else
					min = i;
			}

			if (min == -1) {
				if (isEnd == true)
					break;
				else {
					isChunk.assign(fileCount, false);
					continue;
				}
			}

			former_int = container[min];

			fwrite(&container[min], sizeof(int), 1, files[outputIndex]);

			if (fread_s(&container[min], sizeof(int), sizeof(int), 1, files[min])) {
				if (container[min] < former_int) {
					isChunk[min] = true;
				}
			}
			else {
				container[min] = INT_MAX;
				empty = min;
				isChunk[min] = true;
				isEnd = true;
			}
		}

		for (int i = 0; i < fileCount; ++i)
			if (container[i] != INT_MAX) {
				isEnd = false;
				break;
			}

		if (isEnd)
		{
			result = outputIndex;
			break;
		}

		fclose(files[outputIndex]);
		fclose(files[empty]);
		fopen_s(&files[outputIndex], filenames[outputIndex], "rb");
		fopen_s(&files[empty], filenames[empty], "wb");

		fread_s(&container[outputIndex], sizeof(int), sizeof(int), 1, files[outputIndex]);

		container[empty] = INT_MAX;
		outputIndex = empty;
	}
	for (int i = 0; i < fileCount; ++i)
		fclose(files[i]);

	return result;
}