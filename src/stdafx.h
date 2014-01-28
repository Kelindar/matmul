// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <ppl.h>
#include <iostream>
#include <ctime>

using namespace concurrency;

// TODO: reference additional headers your program requires here

void usleep(__int64 usec) ;
int** ReadMatrix(const char* filename, int sizeX, int sizeY);
int** CreateMatrix(int sizeX, int sizeY);