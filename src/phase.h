#pragma once
#include "stdafx.h"


//------------------------------------------------------------------------------------
// Program A
//------------------------------------------------------------------------------------
struct PhaseA {

	static void test(std::vector<int> v)
	{
		int* matrix = &v[0];
		int  count = v.size();
		std::cout << "Phase (A)... ";
		clock_t t0 = clock();

		// Start monitoring
		EventWriteBegin();
		{
			// for loop on every 100th element of the vector
			for (int multiplier = 0; multiplier < 1000; multiplier++){
				parallel_for(0, count, 100, [&](int i) {
					matrix[i] = matrix[i] * multiplier;
				});
			}

			// for loop on every element of the vector
			for (int multiplier = 0; multiplier < 10; multiplier++){
				parallel_for(0, count, [&](int i) {
					matrix[i] = matrix[i] * multiplier;
				});
			}
		}
		EventWriteEnd();

		clock_t t1 = clock();
		std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
	}
};


//------------------------------------------------------------------------------------
// Program B
//------------------------------------------------------------------------------------
struct PhaseB {

	static void test(std::vector<float> v)
	{
		float* matrix = &v[0];
		int  count = v.size();
		std::cout << "Phase (B)... ";
		clock_t t0 = clock();

		// Start monitoring
		EventWriteBegin();
		{
			// for loop on every 100th element of the vector
			for (int multiplier = 0; multiplier < 1000; multiplier++){
				parallel_for(0, count, 100, [&](int i) {
					matrix[i] = matrix[i] * multiplier;
				});
			}

			// for loop on every element of the vector
			for (int multiplier = 0; multiplier < 10; multiplier++){
				parallel_for(0, count, [&](int i) {
					matrix[i] = matrix[i] * multiplier;
				});
			}
		}
		EventWriteEnd();

		clock_t t1 = clock();
		std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
	}
};
