// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <windows.h>
#include <evntprov.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <ppl.h>
#include <ppltasks.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <iterator>
#include <vector>
#include <ctime>
#include "matmul.h"

using namespace concurrency;

// TODO: reference additional headers your program requires here

void usleep(__int64 usec) ;


/**
* Reads a matrix from a file.
*/
int** ReadMatrix(const char* filename, int sizeX, int sizeY);

/**
* Allocates an empty matrix.
*/
int** CreateMatrix(int sizeX, int sizeY);


/**
* Reads a vector from a file.
*/
std::vector<int> ReadVector(const char* filename, int size);
std::vector<float> ReadVectorF(const char* filename, int size);