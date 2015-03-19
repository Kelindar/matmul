#pragma once
#include "stdafx.h"


/**
* Performs a matrix multiplication in I, J, K order.
*/
void Sequential_IJK(int n, int** a, int** b, int** c){
	// Begin measurement
	std::cout << "Sequential IJK Multiply... ";
	clock_t t0 = clock();

	for (int i = 0; i<n; i++) {
		for (int j = 0; j<n; j++) {
			int sum = 0;
			for (int k = 0; k<n; k++)
				sum += a[i][k] * b[k][j];
			c[i][j] = sum;
		}
	}

	// End measurement
	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
};

/**
* Performs a matrix multiplication in J, I, K order.
*/
void Sequential_JIK(int n, int** a, int** b, int** c){
	// Begin measurement
	std::cout << "Sequential JIK Multiply... ";
	clock_t t0 = clock();

	for (int j = 0; j<n; j++) {
		for (int i = 0; i<n; i++) {
			int sum = 0.0;
			for (int k = 0; k<n; k++)
				sum += a[i][k] * b[k][j];
			c[i][j] = sum;
		}
	}

	// End measurement
	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
};

/**
* Performs a matrix multiplication in K, I, J order.
*/
void Sequential_KIJ(int n, int** a, int** b, int** c){
	// Begin measurement
	std::cout << "Sequential KIJ Multiply... ";
	clock_t t0 = clock();

	for (int k = 0; k<n; k++) {
		for (int i = 0; i<n; i++) {
			int r = a[i][k];
			for (int j = 0; j<n; j++)
				c[i][j] += r * b[k][j];
		}
	}

	// End measurement
	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
};

/**
* Performs a matrix multiplication in I, K, J order.
*/
void Sequential_IKJ(int n, int** a, int** b, int** c){
	// Begin measurement
	std::cout << "Sequential IKJ Multiply... ";
	clock_t t0 = clock();

	for (int i = 0; i<n; i++) {
		for (int k = 0; k<n; k++) {
			int r = a[i][k];
			for (int j = 0; j<n; j++)
				c[i][j] += r * b[k][j];
		}
	}

	// End measurement
	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
};


/**
* Performs a matrix multiplication in J, K, I order.
*/
void Sequential_JKI(int n, int** a, int** b, int** c){
	// Begin measurement
	std::cout << "Sequential JKI Multiply... ";
	clock_t t0 = clock();

	for (int j = 0; j<n; j++) {
		for (int k = 0; k<n; k++) {
			int r = b[k][j];
			for (int i = 0; i<n; i++)
				c[i][j] += a[i][k] * r;
		}
	}

	// End measurement
	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
};



/**
* Performs a matrix multiplication in K, J, I order.
*/
void Sequential_KJI(int n, int** a, int** b, int** c){
	// Begin measurement
	std::cout << "Sequential KJI Multiply... ";
	clock_t t0 = clock();

	for (int k = 0; k<n; k++) {
		for (int j = 0; j<n; j++) {
			int r = b[k][j];
			for (int i = 0; i<n; i++)
				c[i][j] += a[i][k] * r;
		}
	}

	// End measurement
	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
};



/**
* Performs a parallelized matrix multiplication in I, J, K order.
*/
void Parallel_IJK(int n, int** a, int** b, int** c){
	// Begin measurement
	std::cout << "Parallel IJK Multiply... ";
	clock_t t0 = clock();

	parallel_for(0, n, [&](int i)
	{
		for (int j = 0; j<n; j++) {
			int sum = 0;
			for (int k = 0; k<n; k++)
				sum += a[i][k] * b[k][j];
			c[i][j] = sum;
		}
	});

	// End measurement
	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
};


/**
* Performs a matrix multiplication in J, I, K order.
*/
void Parallel_JIK(int n, int** a, int** b, int** c){
	// Begin measurement
	std::cout << "Parallel JIK Multiply... ";
	clock_t t0 = clock();

	parallel_for(0, n, [&](int j)
	{
		for (int i = 0; i<n; i++) {
			int sum = 0.0;
			for (int k = 0; k<n; k++)
				sum += a[i][k] * b[k][j];
			c[i][j] = sum;
		}
	});

	// End measurement
	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
};

/**
* Performs a matrix multiplication in K, I, J order.
*/
void Parallel_KIJ(int n, int** a, int** b, int** c){
	// Begin measurement
	std::cout << "Parallel KIJ Multiply... ";
	clock_t t0 = clock();

	parallel_for(0, n, [&](int k)
	{
		for (int i = 0; i<n; i++) {
			int r = a[i][k];
			for (int j = 0; j<n; j++)
				c[i][j] += r * b[k][j];
		}
	});

	// End measurement
	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
};

/**
* Performs a matrix multiplication in I, K, J order.
*/
void Parallel_IKJ(int n, int** a, int** b, int** c){
	// Begin measurement
	std::cout << "Parallel IKJ Multiply... ";
	clock_t t0 = clock();

	parallel_for(0, n, [&](int i)
	{
		for (int k = 0; k<n; k++) {
			int r = a[i][k];
			for (int j = 0; j<n; j++)
				c[i][j] += r * b[k][j];
		}
	});

	// End measurement
	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
};


/**
* Performs a matrix multiplication in J, K, I order.
*/
void Parallel_JKI(int n, int** a, int** b, int** c){
	// Begin measurement
	std::cout << "Parallel JKI Multiply... ";
	clock_t t0 = clock();

	parallel_for(0, n, [&](int j)
	{
		for (int k = 0; k<n; k++) {
			int r = b[k][j];
			for (int i = 0; i<n; i++)
				c[i][j] += a[i][k] * r;
		}
	});

	// End measurement
	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
};


/**
* Performs a matrix multiplication in K, J, I order.
*/
void Parallel_KJI(int n, int** a, int** b, int** c){
	// Begin measurement
	std::cout << "Parallel KJI Multiply... ";
	clock_t t0 = clock();

	parallel_for(0, n, [&](int k)
	{
		for (int j = 0; j<n; j++) {
			int r = b[k][j];
			for (int i = 0; i<n; i++)
				c[i][j] += a[i][k] * r;
		}
	});

	// End measurement
	clock_t t1 = clock();
	std::cout << "Time : " << (double)(t1 - t0) / CLOCKS_PER_SEC << "\n";
};