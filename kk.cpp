#include <stdio.h>
#include <stdlib.h>
#include <time.h>



long* bub_sort(long* array, int n) {
	for (int i = 0; i < n-1; i++) {
		for (int j = 0; j < n - i -1; j++) {
			if (array[j] < array[j+1]) {
				long temp_var = array[j+1];
				array[j+1] = array[j];
				array[j] = temp_var;
			}
		}
	}
	return array;
}

long kk_alg(long* array, int n) {
	for (int i = 0; i < n-1; i++) {
		long new_value = array[0] - array[1];
		array[0] = 0;
		array[1] = new_value;
		array = bub_sort(array, n);
	}
	return array[0]; 
}

long* gen_rand_sol(int n) {
	srand((unsigned) time(NULL));
	long array[n];
	for (int i = 0; i < n; i++) {
		long r = ((long) rand() % 2);
		if (r == 0) {
			array[i] = 1;
		}
		else if (r == 1) {
			array[i] = -1;
		}
		else {
			fprintf(stderr, "Houston, we have a problem\n");
		}
	}
	return array;
}

long rep_rand(long* array, int n, int max_iter) {
	fprintf(stdout, "start\n");
	long* rand_sol = gen_rand_sol(n);
	fprintf(stdout, "start2\n");
	long* temp_rand_sol;
	for (int iter = 1; iter < max_iter + 1; iter++) {
		fprintf(stdout, "here %i \n", iter);
		temp_rand_sol = gen_rand_sol(n);
		long res1 = 0;
		long res2 = 0;
		for (int i = 0; i < n; i++) {
			res1 += array[i]*rand_sol[i];
			res2 += array[i]*temp_rand_sol[i];
		}
		if (res2 < res1) {
			rand_sol = temp_rand_sol;
		}
	}
	long res = 0;
	for (int i = 0; i < n; i++) {
		res += array[i]*rand_sol[i];
	}
	return res;
}

void printArray(long arr[], int size) 
{ 
    int i; 
    for (i = 0; i < size; i++) {
    	fprintf(stdout, "%i : %ld\n", i, arr[i]);
    }
        
} 


int main(int argc, char *argv[]) {
	if (argc != 4) {
		fprintf(stderr, "Error: expecting 3 arguments\n");
		return 1;
	}
	int alg_num = atoi(argv[2]);

	FILE *file = fopen(argv[3], "r");
	if (file == 0) {
		fprintf(stderr, "Could not open input file");
		return 1;
	}
	clock_t begin = clock();

	long np_list[100]; 
	
	if (alg_num == 0) {
		for (int j = 0; j < 100; j++) {
			fscanf(file, "%ld", &np_list[j]);
		}

		long* list_sorted;
		list_sorted = bub_sort(np_list, 100);

		long residue = kk_alg(list_sorted, 100);

		fprintf(stdout, "%ld\n", residue);
	}
	
	if (alg_num == 1) {
		for (int j = 0; j < 100; j++) {
			fscanf(file, "%ld", &np_list[j]);
		}

		int max_iter = 25000;

		long residue = rep_rand(np_list, 100, max_iter);

		fprintf(stdout, "%ld\n", residue);
	}

	// printArray(list_sorted, 100);


	clock_t end = clock();
	double time = (double)(end - begin) / CLOCKS_PER_SEC;
	// fprintf(stdout, "time taken %f", time);
}