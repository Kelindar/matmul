#include "stdafx.h"
#include "matrix.h"
#include "account.h"
#include "sort.h"
#include "particle.h"
#include "phase.h"
#include "sum.h"

void account();
void particle();
void matrix();
void phase();
void sum();

int wmain(int argc, wchar_t *argv[]) {
	// Register this program with the ETW system as a provider.
	ULONG result;
	result = EventRegisterHarvester(); 

	// account();
	// particle();
	//phase();
	sum();

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
	//Sequential_ParticleSystem::test(1000000);
	Parallel_ParticleSystem::test(1000000);
}

// Phase benchmark
void phase(){
	int size = 50000000;
	//auto v = ReadVector("A.bin", size);
	//PhaseA::test(v);

	auto v = ReadVectorF("A.bin", size);
	PhaseB::test(v);
}


// Matmul benchmark
void matrix(){
	int size = 1000;
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

/*int main() {
	// The size of the vector to read
	int size = 5000000;
	auto v = ReadVector("A.bin", size);
	
	Parallel_MergeSort(v.begin(), v.end());
	//Sequential_MergeSort(v.begin(), v.end());
}*/
