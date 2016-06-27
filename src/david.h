#pragma once

#include "stdafx.h"


/* Test and timing harness program for developing a dense matrix
multiplication routine for the CS3014 module */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <omp.h>
#include <thread>
#include <mutex>
#include <random>

using namespace std;

/* the following two definitions of DEBUGGING control whether or not
debugging information is written out. To put the program into
debugging mode, uncomment the following line: */
/*#define DEBUGGING(_x) _x */
/* to stop the printing of debugging information, use the following line: */
#define DEBUGGING(_x)
#define TESTING(_x) _x

/* write matrix to stdout */
void write_out(double ** a, int dim1, int dim2)
{
	int i, j;

	for (i = 0; i < dim1; i++) {
		for (j = 0; j < dim2 - 1; j++) {
			//printf("%f, ", a[i][j]);
			cout << a[i][j] << ", ";
		}
		//printf("%f\n", a[i][dim2-1]);
		cout << a[i][dim2 - 1] << endl;
	}
}


/* create new empty matrix */
double ** new_empty_matrix(int dim1, int dim2)
{
	//double ** result = malloc(sizeof(double*) * dim1);
	//double * new_matrix = malloc(sizeof(double) * dim1 * dim2);
	double ** result = new double*[dim1];
	double * new_matrix = new double[dim1*dim2];

	int i;

	for (i = 0; i < dim1; i++) {
		result[i] = &(new_matrix[i*dim2]);
	}

	return result;
}

/* take a copy of the matrix and return in a newly allocated matrix */
double ** copy_matrix(double ** source_matrix, int dim1, int dim2)
{
	int i, j;
	double ** result = new_empty_matrix(dim1, dim2);

	for (i = 0; i < dim1; i++) {
		for (j = 0; j < dim2; j++) {
			result[i][j] = source_matrix[i][j];
		}
	}

	return result;
}

/* create a matrix and fill it with random numbers */
double ** gen_random_matrix(int dim1, int dim2)
{
	double ** result;
	int i, j;
	//struct timeval seedtime;
	int seed;

	result = new_empty_matrix(dim1, dim2);

	/* use the microsecond part of the current time as a pseudorandom seed */
//	gettimeofday(&seedtime, NULL);
//	seed = seedtime.tv_usec;
	seed = 42;
	srand(seed);

	/* fill the matrix with random numbers */
	for (i = 0; i < dim1; i++) {
		for (j = 0; j < dim2; j++) {
			long long upper = rand();
			long long lower = rand();
			result[i][j] = (double)((upper << 32) | lower);
		}
	}

	return result;
}

/* absolute value because C++ can't understand abs */
double my_abs(double num)
{
	if (num < 0.0) {
		return -num;
	}
	else {
		return num;
	}
}

/* check the sum of absolute differences is within reasonable epsilon */
void check_result(double ** result, double ** control, int dim1, int dim2)
{
	int i, j;
	double sum_abs_diff = 0.0;
	const double EPSILON = 0.0625;

	for (i = 0; i < dim1; i++) {
		for (j = 0; j < dim2; j++) {
			double diff = my_abs(control[i][j] - result[i][j]);
			sum_abs_diff = sum_abs_diff + diff;
		}
	}

	if (sum_abs_diff > EPSILON) {
		//fprintf(stderr, "WARNING: sum of absolute differences (%f) > EPSILON (%f)\n", sum_abs_diff, EPSILON);
		cerr << "WARNING: sum of absolute differences (" << sum_abs_diff << ") > EPSILON (" << EPSILON << ")\n";
	}
}

/* multiply matrix A times matrix B and put result in matrix C */
void matmul(double ** A, double ** B, double ** C, int a_dim1, int a_dim2, int b_dim2)
{
	int i, j, k;

	for (i = 0; i < a_dim1; i++) {
		for (j = 0; j < b_dim2; j++) {
			double sum = 0.0;
			for (k = 0; k < a_dim2; k++) {
				sum += A[i][k] * B[k][j];
			}
			C[i][j] = sum;
		}
	}
}


/* a routine that does a subset of the outer loop iterations, so that it
can be invoked multiple times for different parts of the matrix */
void parallel_matmul_loop(double ** A, double ** B, double ** C, int a_dim1, int a_dim2, int b_dim2, int outer_start, int outer_end, int stride)
{
	int i, j, k;

	for (i = outer_start; i < outer_end; i+=stride) {
		for (j = 0; j < b_dim2; j++) {
			double sum = 0.0;
			for (k = 0; k < a_dim2; k++) {
				sum += A[i][k] * B[k][j];
			}
			C[i][j] = sum;
		}
	}
}

class workq {
private:
	int * starts;
	int * ends;
	int top;
	mutex mtx;
public:
	workq(int size);
	void push(int start, int end);
	void pop(int & start, int & end);
	bool empty() { return top == 0; };
	void lock() { my_lock(mtx); };
	void unlock() { my_unlock(mtx); };
};

workq::workq(int size)
{
	starts = new int[size];
	ends = new int[size];
	top = 0;
}

void workq::push(int start, int end)
{
	starts[top] = start;
	ends[top] = end;
	top++;
}

void workq::pop(int & start, int & end)
{
	top--;
	start = starts[top];
	end = ends[top];
}

void worker(double ** A, double ** B, double ** C, int a_dim1, int a_dim2, int b_dim2, workq * pool)
{
	int start, end;

	while (!pool->empty()) {
		pool->lock();
		pool->pop(start, end);
		/*std::cout << "\nLocked region - (" << start << ", " << end << ")";
		fflush(stdout);*/
		pool->unlock();
		
		parallel_matmul_loop(A, B, C, a_dim1, a_dim2, b_dim2, start, end, 1);
	}
}

/* the parallel version of matmul */
void workq_matmul(double ** A, double ** B, double ** C, int a_dim1, int a_dim2, int b_dim2, int nthreads, int npartitions)
{
	thread *threads = new thread[nthreads];
	workq pool(npartitions);
	int i;
	int increment = a_dim1 / npartitions;
	int start = 0;
	int end = increment;

	// partition the work into chunks 0..npartitions-2
	for (i = 0; i < npartitions - 2; i++) {
		//parallel_matmul_loop(A, B, C, a_dim1, a_dim2, b_dim2, start, end);
		//threads[i] = thread(parallel_matmul_loop, A, B, C, a_dim1, a_dim2,
		//		       b_dim2, start, end);
		pool.push(start, end);
		start = end;
		end = end + increment;
	}
	// add last partition to deal with odd-sized leftover
	//parallel_matmul_loop(A, B, C, a_dim1, a_dim2, b_dim2, start, a_dim1);
	pool.push(start, a_dim1);

	// start worker threads
	for (i = 0; i < nthreads - 1; i++) {
		threads[i] = thread(worker, A, B, C, a_dim1, a_dim2, b_dim2, &pool);
	}

	// wait for all the threads to complete
	for (i = 0; i < nthreads - 1; i++) {
		threads[i].join();
	}
}

/* the parallel version of matmul */
void parallel_matmul(double ** A, double ** B, double ** C, int a_dim1, int a_dim2, int b_dim2, int nthreads, int unbalanced, int interleaved)
{
	thread *threads = new thread[nthreads];
	int i;
	int increment = a_dim1 / nthreads;
	int start = 0;
	int end = increment;
	int stride = interleaved ? nthreads : 1;

	// start threads 0..nthreads-2
	for (i = 0; i < nthreads - 1; i++) {
		//parallel_matmul_loop(A, B, C, a_dim1, a_dim2, b_dim2, start, end);
		threads[i] = thread(parallel_matmul_loop, A, B, C, a_dim1, a_dim2,
			b_dim2, start, end, stride);
		start = end;
		end = end + increment;
		if (unbalanced) {
			increment = (increment / 8) * 7;
		}
	}
	// start the last thread to deal with odd-sized leftover using the
	// main thread
	parallel_matmul_loop(A, B, C, a_dim1, a_dim2, b_dim2, start, a_dim1, 1);


	// wait for all the threads to complete
	for (i = 0; i < nthreads - 1; i++) {
		threads[i].join();
	}
}

int david(int a_dim1, int a_dim2, int b_dim1, int b_dim2, int nthreads, int npartitions)
{
	double ** A, ** B, ** C;
	double ** control_matrix;
	double mul_time;
	//int a_dim1, a_dim2, b_dim1, b_dim2;
	//int nthreads, npartitions;
	//struct timeval start_time;
	//struct timeval stop_time;
	bool unbalanced, interleaved;

	//if (argc != 7) {
	//	// fprintf(stderr, "Usage: matmul-harness <A nrows> <A ncols> <B nrows> <B ncols> <nthreads> <workq entries>\n");
	//	cerr << "Usage: matmul-harness <A nrows> <A ncols> <B nrows> <B ncols> <nthreads> <workq_entries>\n";
	//	cerr << "Uses workq if workq_entries > 0, unbalanced simple parallel thread if\n";
	//	cerr << "workq_entries < 0, otherwise simple parallel thread\n";
	//	exit(1);
	//}
	//else {
	//	a_dim1 = atoi(argv[1]);
	//	a_dim2 = atoi(argv[2]);
	//	b_dim1 = atoi(argv[3]);
	//	b_dim2 = atoi(argv[4]);
	//	nthreads = atoi(argv[5]);
	//	npartitions = atoi(argv[6]);
	//}

	/* check the matrix sizes are compatible */
	if (a_dim2 != b_dim1) {
		// fprintf(stderr, "FATAL number of columns of A (%d) does not match number of rows of B (%d)\n", a_dim2, b_dim1);
		cerr << "FATAL number of columns of A (" << a_dim2 <<
			") does not match number of rows of B (" << b_dim1 << ")\n";
		exit(1);
	}

	/* allocate the matrices */
	A = gen_random_matrix(a_dim1, a_dim2);
	B = gen_random_matrix(b_dim1, b_dim2);
	C = new_empty_matrix(a_dim1, b_dim2);
	TESTING(control_matrix = new_empty_matrix(a_dim1, b_dim2));

	DEBUGGING(write_out(A, a_dim1, a_dim2));

	/* use a simple matmul routine to produce control result */
	TESTING(matmul(A, B, control_matrix, a_dim1, a_dim2, b_dim2));
	EventWriteBegin();
	{
		/* record starting time */
		clock_t t0 = clock();

		/* perform matrix multiplication */
		if (npartitions <= 0) {
			unbalanced = npartitions == -1;
			interleaved = npartitions == -2;
			parallel_matmul(A, B, C, a_dim1, a_dim2, b_dim2, nthreads, unbalanced, interleaved);
		}
		else {
			workq_matmul(A, B, C, a_dim1, a_dim2, b_dim2, nthreads, npartitions);
		}

		/* record finishing time */
		clock_t t1 = clock();
		mul_time = (double)(t1 - t0) / CLOCKS_PER_SEC;
		//printf("Matmul time: %lld microseconds\n", mul_time);
		cout << "Time : " << mul_time << "\n";
	}
	EventWriteEnd();
		
	DEBUGGING(write_out(C, a_dim1, b_dim2));

	/* now check that the team's matmul routine gives the same answer
	as the known working version */
	TESTING(check_result(C, control_matrix, a_dim1, b_dim2));

	return 0;
}
