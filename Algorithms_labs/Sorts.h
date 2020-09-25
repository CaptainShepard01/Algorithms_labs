#pragma once
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include "Header.h"

template<typename T>
void InsertSort(T* a, int size) {
    T x;
    int i, j;
    for (i = 0; i < size; i++) {
        x = a[i];
        for (j = i - 1; j >= 0 && a[j] > x; j--) a[j + 1] = a[j];
        a[j + 1] = x;
    }
}

template<typename T>
void BubbleSort(T* a, int size) {
    int i, j;
    T x;
    for (i = 0; i < size; i++) {
        for (j = size - 1; j > i; j--) {
            if (a[j - 1] > a[j]) {
                x = a[j - 1]; a[j - 1] = a[j]; a[j] = x;
            }
        }
    }
}

template<typename T>
void ShakerSort(T* a, int size) {
    int j, k = size - 1;
    int lb = 1, ub = size - 1;
    T x;
    do {
        for (j = ub; j > 0; j--) {
            if (a[j - 1] > a[j]) {
                x = a[j - 1]; a[j - 1] = a[j]; a[j] = x;
                k = j;
            }
        }

        lb = k + 1;

        for (j = 1; j <= ub; j++) {
            if (a[j - 1] > a[j]) {
                x = a[j - 1]; a[j - 1] = a[j]; a[j] = x;
                k = j;
            }
        }

        ub = k - 1;
    } while (lb < ub);
}

template<typename T>
void QuickSort(T* a, int N) {
    int i = 0, j = N - 1;
    T temp, p;
    p = a[N >> 1];
    do {
        while (a[i] < p) i++;
        while (a[j] > p) j--;

        if (i <= j) {
            temp = a[i]; a[i] = a[j]; a[j] = temp;
            i++; j--;
        }
    } while (i <= j);

    if (j > 0) QuickSort(a, j);
    if (N > i) QuickSort(a + i, N - i);
}

template<typename T>
void merge(T* a, int lb, int split, int ub) {
    long pos1 = lb;
    long pos2 = split + 1;

    long pos3 = 0;

    T* temp = new T[ub - lb + 1];

    while (pos1 <= split && pos2 <= ub) {
        if (a[pos1] < a[pos2])
            temp[pos3++] = a[pos1++];
        else
            temp[pos3++] = a[pos2++];
    }

    while (pos2 <= ub)
        temp[pos3++] = a[pos2++];
    while (pos1 <= split)
        temp[pos3++] = a[pos1++];

    for (pos3 = 0; pos3 < ub - lb + 1; pos3++)
        a[lb + pos3] = temp[pos3];

    delete temp;
}

template<typename T>
void MergeSort(T* a, int lb, int ub) {
    int split;

    if (lb < ub) {
        split = (lb + ub) / 2;

        MergeSort(a, lb, split);
        MergeSort(a, split + 1, ub);
        merge(a, lb, split, ub);
    }
}

int increment(int inc[], int size) {
    int p1, p2, p3, s;

    p1 = p2 = p3 = 1;
    s = -1;
    do {
        if (++s % 2) {
            inc[s] = 8 * p1 - 6 * p2 + 1;
        }
        else {
            inc[s] = 9 * p1 - 9 * p3 + 1;
            p2 *= 2;
            p3 *= 2;
        }
        p1 *= 2;
    } while (3 * inc[s] < size);

    return s > 0 ? --s : 0;
}

template<typename T>
void ShellSort(T* a, int size) {
    int inc, i, j, seq[40];
    int s;

    s = increment(seq, size);
    while (s >= 0) {
        inc = seq[s--];

        for (i = inc; i < size; i++) {
            T temp = a[i];
            for (j = i - inc; (j >= 0) && (a[j] > temp); j -= inc)
                a[j + inc] = a[j];
            a[j + inc] = temp;
        }
    }
}