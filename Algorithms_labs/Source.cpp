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
void initialize(int& numb, int& maxRAM, int& fileCount, int& low, int& high);
void randomizer(const char* filename, int count, const int low, const int high);
std::pair<std::vector<int>, int> chunkNumberDeterminer(const int& fileCount, const int& elementsCount, const int& maxRAM, int& total_chunks);
std::vector<char*> chunkSeparator(const char* filename, const int& chunkSize, const std::vector<int>& chunks);
void removeIntFromTheTop(const char* filename, const int N);
bool isEmptyFile(const char* filename);
const char* fileMerge(std::vector<char*> filenames, const int& chunkSize, const int& total_chunks);

int minIndex_alt(const vector<char*> filenames, int output_ind);
const char* fileMerge_alt(std::vector<char*> filenames, const int& chunkSize, const int& total_chunks);

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

//void initialize(int& numbersAmount, int& RAM, int& filesAmount) {
//	std::cout << "Numbers amount : ";
//	std::cin >> numbersAmount;
//
//	std::cout << "RAM limit : ";
//	std::cin >> RAM;
//
//	std::cout << "Files amount : ";
//	std::cin >> filesAmount;
//}
//
//const char* getMixedFile(int recordAmount) {
//	const char* filename = "input";
//
//	FILE* file;
//
//	if (fopen_s(&file, filename, "wb"))
//		throw std::runtime_error("Cant open file!");
//
//	std::default_random_engine generator;
//	std::uniform_int_distribution<int> distribution(recordAmount * (-1), recordAmount + 2);
//
//	int randomNumber = 0;
//
//	while (file && recordAmount--) {
//		randomNumber = distribution(generator);
//		fwrite(&randomNumber, sizeof(int), 1, file);
//	}
//
//	if (file)
//		fclose(file);
//
//	return filename;
//}
//
//void printBinaryFile(const char* filename) {
//	FILE* file;
//
//	fopen_s(&file, filename, "rb");
//
//	if (file == nullptr)
//		return;
//
//	rewind(file);
//
//	int tempNumber = 0;
//
//	while (fread_s(&tempNumber, sizeof(int), sizeof(int), 1, file)) {
//		std::cout << tempNumber << " ";
//	}
//
//	std::cout << '\n';
//
//	if (file)
//		fclose(file);
//}
//
//std::vector<int> readBinaryFile(const char* filename) {
//	FILE* file;
//
//	std::vector<int> result;
//
//	fopen_s(&file, filename, "rb");
//
//	if (file == nullptr)
//		throw std::runtime_error("");
//
//	int tempNumber = 0;
//
//	while (fread_s(&tempNumber, sizeof(int), sizeof(int), 1, file)) {
//		result.push_back(tempNumber);
//	}
//
//	if (file)
//		fclose(file);
//
//	return result;
//}
//
//std::pair<std::vector<int>, int> chunkNumberDeterminer(const int& fileCount, const int& elementsCount, const int& maxRAM) {
//	std::vector<int> chunks(fileCount);
//	chunks[0] = 1;
//
//	int capacity = 1, amountOfChunks = elementsCount / maxRAM;
//
//	while (capacity < amountOfChunks) {
//		std::sort(chunks.begin(), chunks.end());
//
//		int temp = chunks[fileCount - 1];
//
//		capacity -= temp;
//		chunks[fileCount - 1] = 0;
//
//		for (int i = 0; i < fileCount - 1; i++) {
//			chunks[i] += temp;
//			capacity += temp;
//		}
//	}
//
//	return make_pair(chunks, capacity);
//}
//
//std::vector<char*> chunkSeparator(const char* filename, const int& chunkSize, const std::vector<int>& chunks) {
//	FILE* origin;
//
//	fopen_s(&origin, filename, "rb");
//
//	int* memory = new int[chunkSize];
//
//	std::vector<char*> filenames(chunks.size());
//
//	for (size_t i = 0, n = filenames.size(); i < n; i++) {
//		filenames[i] = new char[10];
//		char temp[] = { (char)('0' + i), '\0' }; // filename: "i"
//		strcpy_s(filenames[i], sizeof(filenames[i]), temp);
//	}
//
//	for (size_t i = 0, n = chunks.size(); i < n; i++) {
//		FILE* tempFile;
//		int size = 0;
//		fopen_s(&tempFile, filenames[i], "wb");
//
//		for (int j = 0, m = chunks[i]; origin && tempFile && j < m; j++) {
//			size = fread(memory, sizeof(int), chunkSize, origin);
//
//			std::sort(memory, memory + size);
//
//			fwrite(memory, sizeof(int), size, tempFile);
//		}
//
//		if (tempFile)
//			fclose(tempFile);
//	}
//
//	delete[] memory;
//
//	if (origin)
//		fclose(origin);
//
//	return filenames;
//}
//
//bool isEmptyFile(const char* filename) {
//	FILE* tFile;
//
//	fopen_s(&tFile, filename, "rb");
//
//	int x;
//
//	if (fread_s(&x, sizeof(int), sizeof(int), 1, tFile) == 0) {
//		if (tFile)
//			fclose(tFile);
//		return true;
//	}
//
//	if (tFile)
//		fclose(tFile);
//	return false;
//}
//
//int nonEmptyFilesCount(std::vector<char*> filenames) {
//	int count = 0;
//	for (const auto& fname : filenames) {
//		if (!isEmptyFile(fname))
//			count++;
//	}
//	return count;
//}
//
//int findMinIntPosition(const std::vector<int>& currentNumber, int outputIndex, int target = INT32_MIN) {
//	int min = INT32_MAX;
//	int position = -1;
//	for (int i = 0, n = currentNumber.size(); i < n; i++) {
//		if (i == outputIndex)
//			continue;
//
//		if (currentNumber[i] >= target) {
//			min = std::fmin(min, currentNumber[i]);
//			if (min == currentNumber[i])
//				position = i;
//		}
//	}
//
//	return position;
//}
//
//void deleteNFromTheTop(const char* filename, int N) {
//	FILE* file;
//	fopen_s(&file, filename, "rb");
//
//	if (file)
//		fseek(file, N * sizeof(int), SEEK_SET);
//
//	FILE* temp;
//	fopen_s(&temp, "temp", "wb");
//
//	int num;
//
//	while (file && temp && fread(&num, sizeof(int), 1, file))
//		fwrite(&num, sizeof(int), 1, temp);
//
//	freopen_s(&file, filename, "wb", file);
//	freopen_s(&temp, "temp", "rb", temp);
//
//	while (file && temp && fread(&num, sizeof(int), 1, temp))
//		fwrite(&num, sizeof(int), 1, file);
//
//	if (temp)
//		fclose(temp);
//	if (file)
//		fclose(file);
//}
//
//const char* fileMerge(std::vector<char*> filenames) {
//	int fileCount = filenames.size(), outputIndex = 0;
//
//	std::vector<FILE*> files(fileCount);
//	std::vector<int> currentNumber(fileCount);
//
//	for (int i = 0; i < fileCount; i++) {
//		if (isEmptyFile(filenames[i])) {
//			outputIndex = i;
//		}
//		else {
//			fopen_s(&files[i], filenames[i], "rb");
//			fread_s(&currentNumber[i], sizeof(int), sizeof(int), 1, files[i]);
//			fclose(files[i]);
//			deleteNFromTheTop(filenames[i], 1);
//		}
//	}
//
//	while (nonEmptyFilesCount(filenames) != 1) {
//
//		//TO DO implement like a function
//		for (int i = 0; i < fileCount; i++) {
//			if (isEmptyFile(filenames[i])) {
//				outputIndex = i;
//				break;
//			}
//		}
//
//		int prev = currentNumber[findMinIntPosition(currentNumber, outputIndex)] - 1;
//
//		do {
//			int minPosition = findMinIntPosition(currentNumber, outputIndex, prev);
//
//			if (minPosition == -1) {
//				prev = currentNumber[findMinIntPosition(currentNumber, outputIndex)] - 1;
//				continue;
//			}
//
//			fopen_s(&files[minPosition], filenames[minPosition], "rb");
//			fopen_s(&files[outputIndex], filenames[outputIndex], "ab");
//
//			if (currentNumber[minPosition] != INT32_MAX)
//				fwrite(&currentNumber[minPosition], sizeof(int), 1, files[outputIndex]);
//			else {
//				fclose(files[minPosition]);
//				fclose(files[outputIndex]);
//				break;
//			}
//
//			prev = currentNumber[minPosition];
//
//			if (fread_s(&currentNumber[minPosition], sizeof(int), sizeof(int), 1, files[minPosition]) == 0) {
//				currentNumber[minPosition] = INT32_MAX;
//				fclose(files[minPosition]);
//				fclose(files[outputIndex]);
//				continue;
//			}
//
//			fclose(files[minPosition]);
//			fclose(files[outputIndex]);
//
//			deleteNFromTheTop(filenames[minPosition], 1);
//
//			//std::cout << "\n\n";
//
//			//printBinaryFile(filenames[outputIndex]);
//
//		} while (true);
//
//		fopen_s(&files[outputIndex], filenames[outputIndex], "rb");
//		fread_s(&currentNumber[outputIndex], sizeof(int), sizeof(int), 1, files[outputIndex]);
//		fclose(files[outputIndex]);
//		//deleteNFromTheTop(filenames[outputIndex], 1);
//
//		for (int i = 0; i < fileCount; i++) {
//			if (files[i])
//				fclose(files[i]);
//
//		}
//	}
//	deleteNFromTheTop(filenames[outputIndex], 1);
//	return filenames[outputIndex];
//}



int main() {
	HANDLE  hConsole;
	int k;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//_chdir("E:\\Visual studio\\Algorithms_labs\\Algorithms_labs\\Files");

	//if (_chdir(".\\Files") == -1) {
	//	if (_mkdir(".\\Files") == 0)
	//		_chdir(".\\Files");
	//}
	
	SetConsoleTextAttribute(hConsole, 15);

	int total_chunks = 0;

	int elementsCount = 3000, maxRAM = 10, fileCount = 15, low = -400, high = 400;
	const char* filename = { "Input" };

	//initialize(elementsCount, maxRAM, fileCount, low, high);
	randomizer(filename, elementsCount, low, high);

	std::vector<int> expected_output = readBinaryFile(filename);
	std::sort(expected_output.begin(), expected_output.end());

	std::pair<std::vector<int>, int> my_pair = chunkNumberDeterminer(fileCount, elementsCount, maxRAM, total_chunks);
	vector<int> chunks = my_pair.first;

	int realChunkSize = std::ceil((double)elementsCount / my_pair.second);

	vector<char*> filenames = chunkSeparator(filename, realChunkSize, chunks);

	std::vector<int> real_output = readBinaryFile(fileMerge(filenames, realChunkSize, total_chunks));

	printBinaryFile(fileMerge_alt(filenames, realChunkSize, total_chunks));

	if (real_output == expected_output) {
		std::cout << "\nSuccessful sort!\n\n";
	}
	else std::cout << "\nError!\n\n";
	system("pause");
	//int numbersAmount = 200, RAM = 10, filesAmount = 6;
	////initialize(numbersAmount, RAM, filesAmount);
	//const char* filename = getMixedFile(numbersAmount);
	//printBinaryFile(filename);
	//
	//std::vector<int> expected = readBinaryFile(filename);
	//std::sort(expected.begin(), expected.end());
	//
	//std::pair<std::vector<int>, int> my_pair = chunkNumberDeterminer(filesAmount, numbersAmount, RAM);
	//vector<int> chunks = my_pair.first;
	//
	//int realChunkSize = std::ceil((double)filesAmount / my_pair.second);
	//
	//vector<char*> filenames = chunkSeparator(filename, realChunkSize, chunks);
	//
	//std::vector<int> real = readBinaryFile(fileMerge(filenames));
	//
	//if (expected == real) {
	//	std::cout << "Correct!\n\n";
	//}
	//else std::cout << "Incorrect!\n\n";
	//system("pause");
	//
	////printBinaryFile(fileMerge(filenames));

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
		std::cout << setw(4) << x << ' ';
		++counter;
	}
	std::cout << "\nEnd of file!\n\nThere are " << counter << " integers.\n";
	fclose(file);
}

void initialize(int& numb, int& maxRAM, int& fileCount, int& low, int& high) {
	do {
		std::cout << "Number of integers: ";
		std::cin >> numb;
		if (numb <= 0) {
			std::cout << "Must be positive!\n";
			system("pause");
			system("cls");
		}
	} while (numb <= 0);
	do {
		std::cout << "MaxRAM: ";
		std::cin >> maxRAM;
		if (maxRAM <= 1) {
			std::cout << "Must be at least 1!\n";
		}
	} while (maxRAM <= 1);
	do {
		std::cout << "Number of files: ";
		std::cin >> fileCount;
		if (fileCount <= 3) {
			std::cout << "Must be at least 3!\n";
		}
	} while (fileCount <= 3);
	std::cout << "Enter lower border set of numbers: ";
	std::cin >> low;
	std::cout << "Enter upper border set of numbers: ";
	std::cin >> high;
	return;
}

void randomizer(const char* filename, int count, const int low, const int high)
{
	FILE* f;
	fopen_s(&f, filename, "wb");
	int* mas = new int[count];
	int gap = high - low;

	for (int i = 0; i < count; ++i) {
		mas[i] = rand() % gap + low;
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
		filenames[i] = new char[100];
		//filenames[i] = (char*)"E:\\Visual studio\\Algorithms_labs\\Algorithms_labs\\Files";
		char temp[] = { (char)'A' + i, '\0' }; // filename: "i"
		strcpy_s(filenames[i], sizeof(filenames[i]), temp);
	}

	for (size_t i = 0, n = chunks.size(); i < n; i++) {
		FILE* tempFile;
		int size = 0;
		fopen_s(&tempFile, filenames[i], "wb");

		for (int j = 0, m = chunks[i]; origin && tempFile && j < m; j++) {
			size = fread(memory, sizeof(int), chunkSize, origin);

			//ShakerSort(memory, size);
			std::sort(memory, memory + size);

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

//TO DO: optimize this function (removeIntFromTheTop)
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

int minIndex(const vector<int> input, int output_ind) {
	int min = 0;
	for (int i = 0; i < input.size(); ++i) {
		if (i != output_ind && input[i] < input[min])
			min = i;
	}
	if (min == output_ind)
		return -1;
	return min;
}

int minIndex_alt(const vector<char*> filenames, int output_ind) {
	int min = INT_MAX;
	int index = -1;
	FILE* file;
	for (int i = 0; i < filenames.size(); ++i) {
		int x;
		fopen_s(&file, filenames[i], "rb");
		if (fread(&x, sizeof(int), 1, file)) {
			if (x < min && i != output_ind) {
				min = x;
				index = i;
			}
		}
		fclose(file);
	}
	return index;
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

void printAllBinaries(std::vector<char*> filenames, int outputIndex) {
	for (int i = 0, n = filenames.size(); i < n; i++) {
		std::cout << "[" << i << "]";
		if (i == outputIndex)
			std::cout << " OUTPUT";
		std::cout << '\n';

		printBinaryFile(filenames[i]);
	}
}

const char* fileMerge(std::vector<char*> filenames, const int& chunkSize, const int& total_chunks) {
	int fileCount = filenames.size(), outputIndex = 0;

	std::vector<FILE*> files(fileCount);           //files
	std::vector<int> container(fileCount);         //vector of int
	std::vector<bool> chunkEnd(fileCount);         //end of chunk in certain file
	std::vector<int> iters(fileCount);             //current numb of read int's from a certain file
	std::vector<int> chunkSizes(fileCount);        //current chunk size for a certain file

	int chunks = 0;
	double percentage = ((double)chunks / total_chunks) * 100;

	for (int i = 0; i < fileCount; ++i)
		chunkSizes[i] = chunkSize;


	while (nonEmptyFilesCount(filenames) != 1) {          //while not 1 file with sorted array
		int out_ind;
		bool is_outputChunk_set = 0;
		for (int i = 0; i < fileCount; ++i) {
			if (isEmptyFile(filenames[i])) {
				out_ind = i;
				iters[out_ind] = 0;
				chunkEnd[out_ind] = 0;
				chunkSizes[out_ind] = 0;
				break;
			}
		}

		do {                //while not 1 empty file among others   
			int chunk_numb = 0;
			for (int i = 0; i < fileCount; ++i) {
				if (i != out_ind) {
					if (!isEmptyFile(filenames[i])) {
						fopen_s(&files[i], filenames[i], "rb");
						fread_s(&container[i], sizeof(int), sizeof(int), 1, files[i]);
						fclose(files[i]);
						iters[i]++;
						removeIntFromTheTop(filenames[i], 1);


					}
					else {
						container[i] = INT_MAX;
						chunk_numb++;
						chunks++;
					}
				}

				if (i == out_ind) {
					container[i] = INT_MAX;
				}
			}


			while (true) {                                         //while not read 1 chunk from each file                                           
				int min = minIndex(container, out_ind);

				fopen_s(&files[min], filenames[min], "rb");
				fopen_s(&files[out_ind], filenames[out_ind], "ab");

				fwrite(&container[min], sizeof(int), 1, files[out_ind]);
				iters[out_ind]++;

				if (!chunkEnd[min]) {
					if (fread_s(&container[min], sizeof(int), sizeof(int), 1, files[min]) != 0) {
						fclose(files[min]);
						removeIntFromTheTop(filenames[min], 1);

						iters[min]++;
					}
					else {
						container[min] = INT_MAX;
						chunkEnd[min] = 1;
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
				fclose(files[out_ind]);

				if (iters[min] >= chunkSizes[min])
					chunkEnd[min] = 1;

				if (chunk_numb == fileCount - 1)
					break;

			}
			for (int i = 0; i < fileCount; ++i) {
				if (i != out_ind) {
					chunkEnd[i] = 0;
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
		if (!isEmptyFile(filenames[i]))
			return filenames[i];
	}
}

const char* fileMerge_alt(std::vector<char*> filenames, const int& chunkSize, const int& total_chunks) {
	int fileCount = filenames.size(), outputIndex = 0;

	std::vector<FILE*> files(fileCount);           //files
	std::vector<int> container(fileCount);         //vector of int
	std::vector<bool> chunkEnd(fileCount);         //end of chunk in certain file
	std::vector<int> iters(fileCount);             //current numb of read int's from a certain file
	std::vector<int> chunkSizes(fileCount);        //current chunk size for a certain file

	int chunks = 0;
	double percentage = ((double)chunks / total_chunks) * 100;

	for (int i = 0; i < fileCount; ++i)
		chunkSizes[i] = chunkSize;


	while (nonEmptyFilesCount(filenames) != 1) {          //while not 1 file with sorted array
		int out_ind;

		do {                //while not 1 empty file among others   
			int chunk_numb = 0;
			for (int i = 0; i < fileCount; ++i) {
				if (isEmptyFile(filenames[i])) {
					out_ind = i;
					iters[out_ind] = 0;
					chunkEnd[out_ind] = 0;
					chunkSizes[out_ind] = 0;
					break;
				}
			}



			while (true) {                                         //while not read 1 chunk from each file                                           
				int min = minIndex_alt(filenames, out_ind);
				if (min == -1) {
					break;
				}

				fopen_s(&files[min], filenames[min], "rb");
				fopen_s(&files[out_ind], filenames[out_ind], "ab");

				fread_s(&container, sizeof(int), sizeof(int), 1, files[min]);

				fwrite(&container, sizeof(int), 1, files[out_ind]);
				iters[out_ind]++;
				iters[min]++;

				fclose(files[min]);
				fclose(files[out_ind]);

				removeIntFromTheTop(filenames[min], 1);


				if (iters[min] == chunkSizes[min]) {
					chunk_numb++;

					chunks++;
					system("cls");
					percentage = ((double)chunks / total_chunks) * 100;
					printPercentage(percentage);
				}

				if (chunk_numb == fileCount - 1) {
					break;
				}

			}
			for (int i = 0; i < fileCount; ++i) {
				if (i != out_ind) {
					chunkEnd[i] = 0;
					iters[i] = 0;
				}
			}

			chunkSizes[out_ind] = iters[out_ind];
			iters[out_ind] = 0;
		} while (nonEmptyFilesCount(filenames) == fileCount);

	}
	system("cls");
	for (int i = 0; i < filenames.size(); ++i) {
		if (!isEmptyFile(filenames[i])) {
			/*printBinaryFile(filenames[i]);
			system("pause");*/
			return filenames[i];
		}
	}
}