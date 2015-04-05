#pragma once
#include "stdafx.h"

//------------------------------------------------------------------------------------
// Program A
//------------------------------------------------------------------------------------
static void Sum_Local(int count)
{
	// Make some data
	int values[10];
	for (int i = 0; i < 10; ++i) {
		values[i] = 0;
	}

	std::cout << "Sum (A)... ";
	clock_t t0 = clock();

	// Start monitoring
	EventWriteBegin();
	{
		parallel_for(0, count, [&](int c) {
			int sum = 0;
			for (int i = 0; i < 10; ++i){
				sum += values[i];
			}
		});
	}
	EventWriteEnd();

	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
}

//------------------------------------------------------------------------------------
// Program B
//------------------------------------------------------------------------------------

static void Sum_Remote(int count)
{
	// Allocate a bit chunk
	int  size = 1024 * 1024 * 1024;
	auto buffer = (int*)calloc(sizeof(int), size);
	if (!buffer)
		std::cout << "Unable to allocate memory" << std::endl;

	std::cout << "Sum (B)... ";
	clock_t t0 = clock();

	srand(static_cast <unsigned> (time(0)));

	//int size = 5000000;
	//auto v = ReadVector("A.bin", size);

	// Start monitoring
	EventWriteBegin();
	{
		parallel_for(0, count, [&](int c) {
			int sum = 0;
			for (int i = 0; i < 10; ++i){
				sum += *(buffer + i * (rand() % 50 + 50) * 1024 * 1024);
			}
		});
	}
	EventWriteEnd();

	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
	free(buffer);
}