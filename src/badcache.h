#pragma once
#include "stdafx.h"

/*
Small OpenMP program that has very poor locality
David Gregg, April 2015

The program takes two command line parameters:
- the base 2 log of the size of the area of memory that will be used
(the bigger this area, the greater scope for cache misses)
- the number of "iterations" or memory acesses in the space

Note that the standard C random function may not be thread safe

To compile on Linux use:
gcc bad-cache.c -o bad-cache -O3 -fopenmp

To run with a memory-region size of 1024 ints and 20000 iterations:
./bad-cache 10 20
*/


// allocate array of size
int * allocate_array(int size)
{
	int * result = (int*)malloc(sizeof(int) * size);
	//assert(result != NULL);
	return result;
}



int BadCache(int arraySize, int iter)
{
	long long size, mask;
	int * array;
	int sum = 0;

	// allocate the big array
	size = 1LL << arraySize;
	mask = size - 1; // depends on size being a power of 2
	array = allocate_array(size);

	// now jump randomly around the array
	int iterations = iter * 1000;
	srand(time(NULL));

	EventWriteBegin();
	{
		//#pragma omp parallel for reduction (+:sum)
		//for (i = 0; i < iterations; i++) {
		parallel_for(0, iterations, [&](int i) {
			long long index = ((unsigned long long) rand() & mask);
			sum = sum + array[index];
		});
	}
	EventWriteEnd();

	return sum;
}
