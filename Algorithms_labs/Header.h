#pragma once
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

std::vector<int> readBinaryFile(const char* filename);
void printBinaryFile(const char* filename);
void initialize(int& numb, int& maxRAM, int& fileCount);
void randomizer(const char* filename, int count);
std::pair<std::vector<int>, int> chunkNumberDeterminer(const int& fileCount, const int& elementsCount, const int& maxRAM, int& total_chunks);
std::vector<char*> chunkSeparator(const char* filename, const int& chunkSize, const std::vector<int>& chunks, int temp);
bool isEmptyFile(const char* filename);
int fileMerge(std::vector<char*> filenames, const int& chunkSize);
void printAllBinaries(std::vector<char*> filenames, int outputIndex);
int MergeSort_alt(std::vector <char*> names);