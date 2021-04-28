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

void printArray(long arr[], int size) 
{ 
    int i; 
    for (i = 0; i < size; i++) {
    	fprintf(stdout, "%i : %ld\n", i, arr[i]);
    }
        
} 


int main(int argc, char *argv[]) {
	if (argc != 4) {
		fprintf(stderr, "Error: expecting 1 arguments\n");
		return 1;
	}
	FILE *file = fopen(argv[3], "r");
	if (file == 0) {
		fprintf(stderr, "Could not open input file");
		return 1;
	}
	clock_t begin = clock();

	long np_list[100]; 
	
	for (int j = 0; j < 100; j++) {
		fscanf(file, "%ld", &np_list[j]);
	}

	long* list_sorted;
	list_sorted = bub_sort(np_list, 100);

	long residue = kk_alg(list_sorted, 100);

	fprintf(stdout, "%ld\n", residue);

	// printArray(list_sorted, 100);


	clock_t end = clock();
	double time = (double)(end - begin) / CLOCKS_PER_SEC;
	// fprintf(stdout, "time taken %f", time);
}