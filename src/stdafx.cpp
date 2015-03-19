// stdafx.cpp : source file that includes just the standard includes
// Toy.Matmul.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

#include <windows.h>

void usleep(__int64 usec) 
{ 
    HANDLE timer; 
    LARGE_INTEGER ft; 

    ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL); 
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
    WaitForSingleObject(timer, INFINITE); 
    CloseHandle(timer); 
}

/**
* Reads a matrix from a file.
*/
std::vector<int> ReadVector(const char* filename, int size)
{
	auto buffer = new int[size];
	FILE *file = fopen(filename, "rb");

	// Read everything
	fread(buffer, sizeof(int), size, file);

	// Push to a vector
	std::vector<int> v;
	for (int i = 0; i < size; ++i)
		v.push_back(buffer[i]);

	return v;
};

/**
* Reads a matrix from a file.
*/
std::vector<float> ReadVectorF(const char* filename, int size)
{
	auto buffer = new int[size];
	FILE *file = fopen(filename, "rb");

	// Read everything
	fread(buffer, sizeof(int), size, file);

	// Push to a vector
	std::vector<float> v;
	for (int i = 0; i < size; ++i)
		v.push_back((float)buffer[i]);

	return v;
};

/**
* Reads a matrix from a file.
*/
int** ReadMatrix(const char* filename, int sizeX, int sizeY)
{
	// Allocate a new matrix of the required size
	int** matrix = new int*[sizeX];
	FILE *file = fopen(filename, "rb");
	for (int i = 0; i < sizeX; ++i)
	{
		// Allocate and read to the matrix
		matrix[i] = new int[sizeY];
		fread(&(matrix[i][0]), sizeof(int), sizeY, file);
	}

	// Return the matrix
	return matrix;
};

/**
* Allocates an empty matrix.
*/
int** CreateMatrix(int sizeX, int sizeY)
{
	// Allocate a new matrix of the required size
	int** matrix = new int*[sizeX];
	for (int i = 0; i < sizeX; ++i){
		matrix[i] = new int[sizeY];
		memset(matrix[i], 0, sizeY * sizeof(int));
	}
	return matrix;
};