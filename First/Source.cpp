#include<iostream>
#include <omp.h>

using namespace std;

int main()
{
#ifdef _OPENMP
	cout << "OpenMP";
#endif // _OPENMP

	cout << "not parallel 1 \n";
	int k = 0;
	omp_set_num_threads(10);

#pragma omp parallel firstprivate(k)
	{
		int muberThread = omp_get_thread_num();
		int numberThreads = omp_get_num_threads();
		printf("Parallel obl %d  START\n", muberThread);
		

		printf("%d \n", muberThread);
		printf("%d \n", numberThreads);
		printf("Parallel obl %d  FINISH\n", muberThread);
	}

	cout << "not parallel obl 2 \n";

#pragma omp parallel num_threads(2)
	{
		int muberThread = omp_get_thread_num();
		int numberThreads = omp_get_num_threads();
		printf("Parallel obl %d  START\n", muberThread);
		printf("%d \n", muberThread);
		printf("%d \n", numberThreads);
		k++;
		printf("Parallel obl %d  FINISH\n", muberThread);
	}
}