#include "stdafx.h"
#include "matmul.h"
#include "sort.h"
#include "particle.h"

int main() {
	int size = 1000000;
	//Sequential_ParticleSystem::test(size);
	Parallel_ParticleSystem::test(size);

}

/*int main() {
	// The size of the vector to read
	int size = 5000000;
	auto v = ReadVector("A.bin", size);
	
	Parallel_MergeSort(v.begin(), v.end());
	//Sequential_MergeSort(v.begin(), v.end());
}*/

/*int main() {
	int size = 1000;
	int** A = ReadMatrix("A.bin", size, size);
	int** B = ReadMatrix("B.bin", size, size);
	int** C = CreateMatrix(size, size);

	Parallel_IJK(size, A, B, C);
	//Parallel_KIJ(size, A, B, C);
	//Parallel_KJI(size, A, B, C);
}*/

/*
int main() {
	// The size of matrices A, B and C is the same
	int size = 1000;

	// Read Matrix A and B
	int** A = ReadMatrix("A.bin", size, size);
	int** B = ReadMatrix("B.bin", size, size);

	// Allocate Matrix C
	int** C = CreateMatrix(size, size);

	std::cout << "/!\ If you want to also measure hardware counters, make sure to run this in low priority (see start.bat)" << std::endl;
	std::cout << "Press any key to start...";
	getchar();

	// Multiply sequentially
	Sequential_IJK(size, A, B, C);
	Sequential_JIK(size, A, B, C);
	Sequential_KIJ(size, A, B, C);
	Sequential_IKJ(size, A, B, C);
	Sequential_JKI(size, A, B, C);
	Sequential_KJI(size, A, B, C);
	//
	//// Multiply in parallel
	Parallel_IJK(size, A, B, C);
	Parallel_JIK(size, A, B, C);
	Parallel_KIJ(size, A, B, C);
	Parallel_IKJ(size, A, B, C);
	Parallel_JKI(size, A, B, C);
	Parallel_KJI(size, A, B, C);

	// Exit
	std::cout << "Press any key to exit...";
	getchar();
}
*/