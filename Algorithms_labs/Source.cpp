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

int main() {
	HANDLE  hConsole;
	int k;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15);

	if (_chdir(".\\Files") == -1) {
		if (_mkdir(".\\Files") == 0)
			_chdir(".\\Files");
	}

	int total_chunks = 0;
	int elementsCount = 1000000, maxRAM = 1000, fileCount = 5;
	const char* filename = { "Input" };

	initialize(elementsCount, maxRAM, fileCount);
	randomizer(filename, elementsCount);

	std::vector<int> expected_output = readBinaryFile(filename);
	std::sort(expected_output.begin(), expected_output.end());

	std::pair<std::vector<int>, int> my_pair = chunkNumberDeterminer(fileCount, elementsCount, maxRAM, total_chunks);
	std::vector<int> chunks = my_pair.first;

	int realChunkSize = elementsCount / my_pair.second;

	std::vector<char*> filenames = chunkSeparator(filename, realChunkSize, chunks, (elementsCount - realChunkSize * (my_pair.second)));

	int res = fileMerge(filenames, realChunkSize, total_chunks);
	//int res = MergeSort_alt(filenames);
	
	std::vector<int> real_output = readBinaryFile(filenames[res]);

	for (int i = 0; i < filenames.size(); ++i) {
		if (i == res)
			continue;
		remove(filenames[i]);
	}

	if (real_output == expected_output) {
		std::cout << "\nSuccessful sort!\n\n";
	}
	else std::cout << "\nError!\n\n";


	system("pause");

	return 0;
}