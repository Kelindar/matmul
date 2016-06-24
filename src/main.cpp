#include "stdafx.h"
#include "matrix.h"
#include "account.h"
#include "sort.h"
#include "particle.h"
#include "phase.h"
#include "sum.h"
#include "badcache.h"
#include "philosopher.h"
#include "wqueue.h"
#include "david.h"

void account();
void particle();
void matrix();
void phase();
void sum();
void badcache();
void mergeP();
void mergeS();
void david();

int wmain(int argc, wchar_t *argv[]) {
	// Register this program with the ETW system as a provider.
	ULONG result;
	result = EventRegisterHarvester(); 

	//account();
	//particle();
	//phase();
	//sum();
	//badcache();
	//philosopher(45);
	//philosopher(12);
	//wqueue(100, 100);
	//matrix();
	david(1000, 100, 100, 1000, 7, 500);
	//mergeP();

	// Unregister ETW provider
	result = EventUnregisterHarvester();
	return (0);
}

// Accounts benchmark
void account(){
	AccountA::test(1000000);
	//AccountB::test(1000000);
}

// Particle benchmark
void particle(){
	Sequential_ParticleSystem::test(1000000);
	//Parallel_ParticleSystem::test(1000000);
}

// Phase benchmark
void phase(){
	int size = 1024;
	auto vi = ReadVector("A.bin", size);
	PhaseA::test(vi);

	auto vf = ReadVectorF("A.bin", size);
	PhaseB::test(vf);
}

// Matmul benchmark
void matrix(){
	int size = 1024;
	int** A = ReadMatrix("A.bin", size, size);
	int** B = ReadMatrix("B.bin", size, size);
	int** C = CreateMatrix(size, size);

	//Parallel_IJK(size, A, B, C);
	Parallel_KIJ(size, A, B, C);
	//Parallel_KJI(size, A, B, C);
}

void sum(){
	int iterations = 100000000;
	//Sum_Local(iterations);
	Sum_Remote(iterations);
}

void badcache(){
	//To run with a memory - region size of 1024 ints and 20000 iterations:
	//bad-cache 10 20
	BadCache(10000, 500000);
}

void mergeP() {
	int size = 1024;
	auto v = ReadVector("A.bin", size);

	Parallel_MergeSort(v.begin(), v.end());
}

void mergeS() {
	int size = 1024;
	auto v = ReadVector("A.bin", size);

	Sequential_MergeSort(v.begin(), v.end());
}
