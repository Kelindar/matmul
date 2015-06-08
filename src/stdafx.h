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
#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include "matmul.h"

#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef unsigned __int64 ulong64;
typedef signed __int64 long64;
#else
typedef unsigned long long ulong64;
typedef signed long long long64;
#endif

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


void my_lock(std::mutex &m);
void my_lock(std::mutex &m1, std::mutex &m2);