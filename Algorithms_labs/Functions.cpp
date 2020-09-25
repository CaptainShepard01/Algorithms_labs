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
#include "Sorts.h"

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
		std::cout << x << ' ';
		++counter;
	}
	std::cout << "\nEnd of file!\n\nThere are " << counter << " integers.\n";
	fclose(file);
}

void initialize(int& numb, int& maxRAM, int& fileCount) {
	do {
		//system("cls");
		std::cout << "Number of integers: ";
		std::cin >> numb;
		if (numb <= 0) {
			std::cout << "Must be positive!\n";
			system("pause");
		}
	} while (numb <= 0);
	do {
		//system("cls");
		std::cout << "MaxRAM: ";
		std::cin >> maxRAM;
		if (maxRAM < 2) {
			std::cout << "Must be at least 2!\n";
			system("pause");
		}
	} while (maxRAM < 2);
	do {
		//system("cls");
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

std::vector<char*> chunkSeparator(const char* filename, const int& chunkSize, const std::vector<int>& chunks, int temp) {
	FILE* origin;

	fopen_s(&origin, filename, "rb");

	int pos_def_chunk = 0;

	int* memory_great = new int[chunkSize + 1];
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
			if (pos_def_chunk < temp) {
				size = fread(memory_great, sizeof(int), chunkSize + 1, origin);

				InsertSort<int>(memory_great, size);

				//std::sort(memory, memory + size);

				fwrite(memory_great, sizeof(int), size, tempFile);
				pos_def_chunk++;
			}
			else {
				size = fread(memory, sizeof(int), chunkSize, origin);

				InsertSort<int>(memory, size);

				//std::sort(memory, memory + size);

				fwrite(memory, sizeof(int), size, tempFile);
			}
		}

		fclose(tempFile);
	}

	delete[] memory, memory_great;

	fclose(origin);

	return filenames;
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

int fileMerge(std::vector<char*> filenames, const int& chunkSize, const int& total_chunks) {
	double t = clock();

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

	t = (clock() - t) * 0.001;
	std::cout << "\nTotal time of Merge function: " << t << std::endl;

	return result;
}

int MergeSort_alt(std::vector <char*> names)
{
	unsigned int start_time = clock();

	std::vector <FILE*> files(names.size());

	int input = 0;

	for (int i = 0; i < names.size(); ++i) {
		if (isEmptyFile(names[i])) {
			input = i;
			fopen_s(&files[i], names[i], "wb");
		}
		else {
			fopen_s(&files[i], names[i], "rb");
		}
	}

	int tempinput = input;
	int resultfile = 0;

	std::vector <int> elements(names.size());
	elements.assign(names.size(), INT32_MIN);

	for (int i = 0; i < names.size(); i++)
	{
		if (i == input)
			continue;
		fread_s(&elements[i], sizeof(int), sizeof(int), 1, files[i]);
	}

	bool exit = false;

	while (true)
	{
		std::vector <bool> endOfChunk;
		endOfChunk.assign(names.size(), false);

		while (true)
		{
			int iter = -1;

			for (int i = 0; i < elements.size(); i++)
			{
				if (i == input || endOfChunk[i] == true || elements[i] == INT32_MIN)
					continue;

				if (iter != -1)
				{
					if (elements[iter] > elements[i])
					{
						iter = i;
					}
				}
				else
					iter = i;
			}

			if (iter == -1)
				if (exit == true)
					break;
				else
				{
					endOfChunk.assign(names.size(), false);
					continue;
				}

			int tempel = elements[iter];

			fwrite(&elements[iter], sizeof(int), 1, files[input]);

			if (fread_s(&elements[iter], sizeof(int), sizeof(int), 1, files[iter]))
			{
				if (tempel > elements[iter])
				{
					endOfChunk[iter] = true;
				}
			}
			else
			{
				tempinput = iter;
				elements[iter] = INT32_MIN;
				exit = true;
			}
		}

		for (int i = 0; i < elements.size(); i++)
		{
			if (elements[i] != INT32_MIN)
			{
				exit = false;

				break;
			}
		}

		if (exit == true)
		{
			resultfile = input;
			break;
		}

		fclose(files[input]);
		fclose(files[tempinput]);

		fopen_s(&files[input], names[input], "rb");
		fread_s(&elements[input], sizeof(int), sizeof(int), 1, files[input]);
		fopen_s(&files[tempinput], names[tempinput], "wb");

		elements[tempinput] = INT32_MIN;

		input = tempinput;
	}

	for (int i = 0; i < names.size(); i++)
	{
		fclose(files[i]);
	}

	//fcloseall();

	unsigned int end_time = clock();

	unsigned int search_time = end_time - start_time;

	double time = (double)(end_time - start_time) / 1000;

	std::cout << std::endl << "Time : " << time << std::endl;

	return resultfile;
}