#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <conio.h>
#include <vector>
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
const char* fileMerge(std::vector<char*> filenames, const int& chunkSize, const int& total_chunks);
const char* fileMerge_1(std::vector<char*> filenames, const int& chunkSize, const int& total_chunks);

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
	int elementsCount = 500, maxRAM = 10, fileCount = 15;
	const char* filename = { "Input" };

	initialize(elementsCount, maxRAM, fileCount);
	randomizer(filename, elementsCount);

	printBinaryFile(filename);
	system("pause");

	std::vector<int> expected_output = readBinaryFile(filename);
	std::sort(expected_output.begin(), expected_output.end());

	std::pair<std::vector<int>, int> my_pair = chunkNumberDeterminer(fileCount, elementsCount, maxRAM, total_chunks);
	vector<int> chunks = my_pair.first;

	int realChunkSize = std::ceil((double)elementsCount / my_pair.second);

	vector<char*> filenames = chunkSeparator(filename, realChunkSize, chunks);

	std::vector<int> real_output = readBinaryFile(fileMerge(filenames, realChunkSize, total_chunks));

	printBinaryFile("Output");

	if (real_output == expected_output) {
		std::cout << "\nSuccessful sort!\n\n";
	}
	else std::cout << "\nError!\n\n";

	for (int i = 0; i < filenames.size(); ++i) {
		remove(filenames[i]);
	}

	system("pause");

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

	int capacity = 1, amountOfChunks = elementsCount / maxRAM;

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

			InsertSort(memory, size);

			//std::sort(memory, memory + size);

			fwrite(memory, sizeof(int), size, tempFile);
		}

		if (tempFile)
			fclose(tempFile);
	}

	delete[] memory;

	if (origin)
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

int minIndex(const vector<int> input, const int output_ind) {
	int min = 0;
	for (int i = 0; i < input.size(); ++i) {
		if (i != output_ind && input[i] < input[min])
			min = i;
	}
	if (min == output_ind)
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

const char* fileMerge(std::vector<char*> filenames, const int& chunkSize, const int& total_chunks) {
	int fileCount = filenames.size(), outputIndex = 0;

	std::vector<FILE*> files(fileCount);           //files
	std::vector<int> container(fileCount);         //vector of int
	std::vector<int> iters(fileCount);             //current numb of read int's from a certain file
	std::vector<int> chunkSizes(fileCount);        //current chunk size for a certain file

	int chunks = 0;
	double percentage = ((double)chunks / total_chunks) * 100;

	for (int i = 0; i < fileCount; ++i)
		chunkSizes[i] = chunkSize;


	while (nonEmptyFilesCount(filenames) != 1) {       
		int out_ind;
		bool is_outputChunk_set = 0;
		for (int i = 0; i < fileCount; ++i) {
			if (isEmptyFile(filenames[i])) {
				out_ind = i;
				iters[out_ind] = 0;
				chunkSizes[out_ind] = 0;
				break;
			}
		}


		do {        
		int min = 0;
		int chunk_numb = 0;
			//system("cls");
			//printAllBinaries(filenames, out_ind);

			fopen_s(&files[out_ind], filenames[out_ind], "ab");
			for (int i = 0; i < fileCount; ++i) {
				if (i == out_ind) {
					container[i] = INT_MAX;
					continue;
				}
				if (!isEmptyFile(filenames[i])) {
					fopen_s(&files[i], filenames[i], "rb");
					fread_s(&container[i], sizeof(int), sizeof(int), 1, files[i]);
					fclose(files[i]);
					iters[i]++;
				}
				else {
					container[i] = INT_MAX;
					chunk_numb++;
					chunks++;
				}
			}
			while (true) {
				min = minIndex(container, out_ind);
				fopen_s(&files[min], filenames[min], "rb");



				fwrite(&container[min], sizeof(int), 1, files[out_ind]);
				iters[out_ind]++;

				if (iters[min] < chunkSizes[min]) {
					fseek(files[min], iters[min] * sizeof(int), SEEK_SET);
					if (fread_s(&container[min], sizeof(int), sizeof(int), 1, files[min]) != 0) {
						fclose(files[min]);

						iters[min]++;
					}
					else {
						container[min] = INT_MAX;
						iters[min]++;
						chunk_numb++;

						chunks++;
						system("cls");
						percentage = ((double)chunks / total_chunks) * 100;
						printPercentage(percentage);
					}
				}
				else {
					container[min] = INT_MAX;

					chunk_numb++;
					system("cls");

					chunks++;
					percentage = ((double)chunks / total_chunks) * 100;
					printPercentage(percentage);
				}

				fclose(files[min]);


				if (chunk_numb == fileCount - 1)
					break;
			}
			fclose(files[out_ind]);


			for (int i = 0; i < fileCount; ++i) {
				if (i != out_ind) {
					removeIntFromTheTop(filenames[i], iters[i]);
					iters[i] = 0;
				}
			}

			if (!is_outputChunk_set) {
				chunkSizes[out_ind] = iters[out_ind];
				is_outputChunk_set = 1;
			}
			iters[out_ind] = 0;

		} while (nonEmptyFilesCount(filenames) == fileCount);


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