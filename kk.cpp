#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double e = 2.718281828459045;

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
		long new_value = array[i] - array[i+1];
		array[i] = 0;
		array[i+1] = new_value;
		for (int j = i+1; j < n-1; j++) {
			if (array[j] < array[j+1]) {
				long temp_var = array[j+1];
				array[j+1] = array[j];
				array[j] = temp_var;
			}
		}
	}
	return array[n-1]; 
}

long* gen_rand_sol(long* array, int n) {
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

long* gen_rand_sol_pp(long* array, int n) {
	for (int i = 0; i < n; i++) {
		long r = ((long) rand() % n);
		array[i] = r;
		// fprintf(stdout, "%i : %ld\n", i, r);
	}
	return array;
}

long calc_res_pp(long* arrayP, long* arrayA, int n) {
	long start_sol[n];
	for (int i = 0; i < n; i++) {
		start_sol[i] = 0;
	}
	for (int j = 0; j < n; j++) {
		start_sol[arrayP[j]] += arrayA[j];
	}
	long* sol_array = bub_sort(start_sol, n);
	return kk_alg(sol_array, n);
}

long* rand_neighbor(long* array, int n) {
	long index1 = ((long) rand() % n);
	long index2 = index1;
	while (index2 == index1) {
		index2 = ((long) rand() % n);
	}
	long coin = ((long) rand() % 2);
	if (coin == 0 ) {
		array[index1] = -array[index1];
	}
	array[index2] = -array[index2];
	return array;
}

long* rand_neighbor_pp(long* array, int n) {
	long index1 = ((long) rand() % n);
	long index2 = index1;
	while (index2 == array[index1]) {
		index2 = ((long) rand() % n);
	}
	array[index1] = index2;
	return array;
}

long rep_rand(long* array, int n, int max_iter) {
	long start_sol[n];
	for (int i = 0; i < n; i++) {
		start_sol[i] = 0;
	}
	long* rand_sol = gen_rand_sol(start_sol, n);
	long res1 = 0;
	for (int i = 0; i < n; i++) {
		res1 += array[i]*rand_sol[i];
	}
	res1 = abs(res1);
	long* temp_rand_sol = gen_rand_sol(start_sol, n);
	for (int iter = 1; iter < max_iter + 1; iter++) {
		// fprintf(stdout, "here %i \n", iter);
		temp_rand_sol = gen_rand_sol(start_sol, n);
		long res2 = 0;
		for (int i = 0; i < n; i++) {
			res2 += array[i]*temp_rand_sol[i];
		}
		res2 = abs(res2);
		if (res2 < res1) {
			res1 = res2;
		}
	}
	return res1;
}

long hill_climb(long* array, int n, int max_iter) {
	long start_sol[n];
	for (int i = 0; i < n; i++) {
		start_sol[i] = 0;
	}
	long* rand_sol = gen_rand_sol(start_sol, n);
	long res1 = 0;
	for (int i = 0; i < n; i++) {
		res1 += array[i]*rand_sol[i];
	}
	res1 = abs(res1);
	long* temp_rand_sol = gen_rand_sol(start_sol, n);
	for (int iter = 1; iter < max_iter + 1; iter++) {
		temp_rand_sol = rand_neighbor(rand_sol, n);
		long res2 = 0;
		for (int i = 0; i < n; i++) {
			res2 += array[i]*temp_rand_sol[i];
		}
		res2 = abs(res2);
		if (res2 < res1) {
			res1 = res2;
			for (int i = 0; i < n; i++) {
				rand_sol[i] = temp_rand_sol[i];
			}
		}
	}
	return res1;
}

double power(double x1, int x2) {
	double solution = 1;
	int counter = x2;
	while (counter > 0) {
		solution = solution*x1;
		counter -= 1;
	}
	return solution;
}

int round_down (double x) {
	return (int) x;
}

double T(int iter) {
	return power(10, 10)*power(.8, round_down(((double) iter)/300));
}

long sim_anneal (long* array, int n, int max_iter) {
	long start_sol[n];
	for (int i = 0; i < n; i++) {
		start_sol[i] = 0;
	}
	long last_sol[n];
	long* rand_sol = gen_rand_sol(start_sol, n);
	long res1 = 0;
	long residue = 0;
	for (int i = 0; i < n; i++) {
		res1 += array[i]*rand_sol[i];
		last_sol[i] = rand_sol[i];
	}
	res1 = abs(res1);
	residue = res1;
	long* temp_rand_sol = gen_rand_sol(start_sol, n);
	for (int iter = 1; iter <= max_iter; iter++) {
		temp_rand_sol = rand_neighbor(rand_sol, n);
		long res2 = 0;
		for (int i = 0; i < n; i++) {
			res2 += array[i]*temp_rand_sol[i];
		}
		res2 = abs(res2);
		if (res2 < res1) {
			res1 = res2;
			for (int i = 0; i < n; i++) {
				rand_sol[i] = temp_rand_sol[i];
			}
		}
		else {
			double p = power(e, -(res2 - res1)/T(iter));
			double r = ((double) rand() / (RAND_MAX));
			if (r <= p) {
				res1 = res2;
				for (int i = 0; i < n; i++) {
					rand_sol[i] = temp_rand_sol[i];
				}
			}
		}
		if (res1 < residue) {
			residue = res1;
		}
	}
	return residue;
}

long rep_rand_pp(long* array, int n, int max_iter) {
	long start_sol[n];
	for (int i = 0; i < n; i++) {
		start_sol[i] = 0;
	}
	long* rand_sol = gen_rand_sol_pp(start_sol, n);
	long res1 = calc_res_pp(rand_sol, array, n);
	long* temp_rand_sol = gen_rand_sol(start_sol, n);
	for (int iter = 1; iter < max_iter + 1; iter++) {
		temp_rand_sol = gen_rand_sol_pp(start_sol, n);
		long res2 = calc_res_pp(temp_rand_sol, array, n);
		if (res2 < res1) {
			res1 = res2;
		}
	}
	return res1;
}

long hill_climb_pp(long* array, int n, int max_iter) {
	long start_sol[n];
	for (int i = 0; i < n; i++) {
		start_sol[i] = 0;
	}
	long* rand_sol = gen_rand_sol_pp(start_sol, n);
	long res1 = calc_res_pp(rand_sol, array, n);
	long* temp_rand_sol = gen_rand_sol_pp(start_sol, n);
	for (int iter = 1; iter < max_iter + 1; iter++) {
		temp_rand_sol = rand_neighbor_pp(rand_sol, n);
		long res2 = calc_res_pp(temp_rand_sol, array, n);
		if (res2 < res1) {
			res1 = res2;
			for (int i = 0; i < n; i++) {
				rand_sol[i] = temp_rand_sol[i];
			}
		}
	}
	return res1;
}

long sim_anneal_pp(long* array, int n, int max_iter) {
	long start_sol[n];
	for (int i = 0; i < n; i++) {
		start_sol[i] = 0;
	}
	long last_sol[n];
	long* rand_sol = gen_rand_sol_pp(start_sol, n);
	long res1 = 0;
	long residue = 0;
	for (int i = 0; i < n; i++) {
		last_sol[i] = rand_sol[i];
	}
	res1 = calc_res_pp(rand_sol, array, n);
	residue = res1;
	long* temp_rand_sol = gen_rand_sol_pp(start_sol, n);
	for (int iter = 1; iter <= max_iter; iter++) {
		temp_rand_sol = rand_neighbor_pp(rand_sol, n);
		long res2 = calc_res_pp(temp_rand_sol, array, n);
		if (res2 < res1) {
			res1 = res2;
			for (int i = 0; i < n; i++) {
				rand_sol[i] = temp_rand_sol[i];
			}
		}
		else {
			double p = power(e, -(res2 - res1)/T(iter));
			double r = ((double) rand() / (RAND_MAX));
			if (r <= p) {
				res1 = res2;
				for (int i = 0; i < n; i++) {
					rand_sol[i] = temp_rand_sol[i];
				}
			}
		}
		if (res1 < residue) {
			residue = res1;
		}
	}
	return residue;
}


void printArray(long arr[], int size) 
{ 
    int i; 
    for (i = 0; i < size; i++) {
    	fprintf(stdout, "%i : %ld\n", i, arr[i]);
    }
        
} 

long find_max(long* array) {
	long* sorted_array = bub_sort(array, 100);
	return sorted_array[0];
}

long find_min(long* array) {
	long* sorted_array = bub_sort(array, 100);
	return sorted_array[99];
}

double find_avg(long* array) {
	double total = 0;
	for (int i = 0; i < 100; i++) {
		total += (double) array[i];
	}
	double avg = (total/100);
	return avg;
}

double find_tot_time(double* array) {
	double total = 0;
	for (int i = 0; i < 100; i++) {
		total += array[i];
	}
	return total;
}


int main(int argc, char *argv[]) {
	srand(time(NULL));
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
	// determine if 100 random cases or just one case from a file.
	int path_determine = atoi(argv[1]);
	if (path_determine == 0) {
		long np_list[100]; 
		for (int j = 0; j < 100; j++) {
			fscanf(file, "%ld", &np_list[j]);
		}
		int max_iter = 25000;
		long residue;

		if (alg_num == 0) {
			long* list_sorted;
			list_sorted = bub_sort(np_list, 100);
			residue = kk_alg(list_sorted, 100);
		}
		else if (alg_num == 1) {
			residue = rep_rand(np_list, 100, max_iter);
		}
		else if (alg_num == 2) {
			residue = hill_climb(np_list, 100, max_iter);
		}
		else if (alg_num == 3) {
			residue = sim_anneal(np_list, 100, max_iter);
		}
		else if (alg_num == 11) {
			residue = rep_rand_pp(np_list, 100, max_iter);
		}
		else if (alg_num == 12) {
			residue = hill_climb_pp(np_list, 100, max_iter);
		}
		if (alg_num == 13) {
			residue = sim_anneal_pp(np_list, 100, max_iter);
		}

		fprintf(stdout, "%ld\n", residue);
	}
	else if (path_determine == 1) {
		// generate 100 random instances
		long** rand_probs = 0;
		rand_probs = (long**) malloc(sizeof(long*) * 100);
		for(int i = 0; i < 100; i++) {
			rand_probs[i] = (long*) malloc(sizeof(long) * 100);
			for(int j = 0; j < 100; j++) {
				double r = ((double) rand() / (RAND_MAX));
				fprintf(stdout, "%f\n", r);
				r = (r* power(10, 12)) + 1;
				long r_final = (long) r;
				rand_probs[i][j] = r_final;
				fprintf(stdout, "%ld\n", r_final);
			}
		}
		// these will hold the residues for each algorithm
		long kk_res[100];
		long rep_rand_res[100];
		long hill_climb_res[100];
		long sim_anneal_res[100];
		long rep_rand_pp_res[100];
		long hill_climb_pp_res[100];
		long sim_anneal_pp_res[100];

		// these will track the time of the algorithm

		double kk_time[100];
		double rep_rand_time[100];
		double hill_climb_time[100];
		double sim_anneal_time[100];
		double rep_rand_pp_time[100];
		double hill_climb_pp_time[100];
		double sim_anneal_pp_time[100];

		long current_prob1[100];
		long current_prob2[100];
		long current_prob3[100];
		long current_prob4[100];
		long current_prob5[100];
		long current_prob6[100];
		long current_prob7[100];

		int max_iter = 25000;

		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 100; j++) {
				current_prob1[j] = rand_probs[i][j];
				current_prob2[j] = rand_probs[i][j];
				current_prob3[j] = rand_probs[i][j];
				current_prob4[j] = rand_probs[i][j];
				current_prob5[j] = rand_probs[i][j];
				current_prob6[j] = rand_probs[i][j];
				current_prob7[j] = rand_probs[i][j];
			}
			clock_t begin1 = clock();
			long* sorted_prob = bub_sort(current_prob1, 100);
			kk_res[i] = kk_alg(sorted_prob, 100);
			fprintf(stdout, "current_prob1[0] %ld\n", current_prob1[0]);
			clock_t end1 = clock();
			kk_time[i] = (double)(end1 - begin1) / CLOCKS_PER_SEC;

			clock_t begin2 = clock();
			rep_rand_res[i] = rep_rand(current_prob2, 100, max_iter);
			fprintf(stdout, "current_prob2[0] %ld\n", current_prob2[0]);
			clock_t end2 = clock();
			rep_rand_time[i] = (double)(end2 - begin2) / CLOCKS_PER_SEC;

			clock_t begin3 = clock();
			hill_climb_res[i] = hill_climb(current_prob3, 100, max_iter);
			fprintf(stdout, "current_prob3[0] %ld\n", current_prob3[0]);
			clock_t end3 = clock();
			hill_climb_time[i] = (double)(end3 - begin3) / CLOCKS_PER_SEC;

			clock_t begin4 = clock();
			sim_anneal_res[i] = sim_anneal(current_prob4, 100, max_iter);
			fprintf(stdout, "current_prob4[0] %ld\n", current_prob4[0]);
			clock_t end4 = clock();
			sim_anneal_time[i] = (double)(end4 - begin4) / CLOCKS_PER_SEC;

			clock_t begin5 = clock();
			rep_rand_pp_res[i] = rep_rand_pp(current_prob5, 100, max_iter);
			fprintf(stdout, "current_prob5[0] %ld\n", current_prob5[0]);
			clock_t end5 = clock();
			rep_rand_pp_time[i] = (double)(end5 - begin5) / CLOCKS_PER_SEC;

			clock_t begin6 = clock();
			hill_climb_pp_res[i] = hill_climb_pp(current_prob6, 100, max_iter);
			fprintf(stdout, "current_prob6[0] %ld\n", current_prob6[0]);
			clock_t end6 = clock();
			hill_climb_pp_time[i] = (double)(end6 - begin6) / CLOCKS_PER_SEC;

			clock_t begin7 = clock();
			sim_anneal_pp_res[i] = sim_anneal_pp(current_prob7, 100, max_iter);
			fprintf(stdout, "current_prob7[0] %ld\n", current_prob7[0]);
			clock_t end7 = clock();
			sim_anneal_pp_time[i] = (double)(end7 - begin7) / CLOCKS_PER_SEC;

		}
		
		
		fprintf(stdout, "kk:\n max: %ld \n min: %ld \n avg: %f\n time: %f\n\n", find_max(kk_res), find_min(kk_res), find_avg(kk_res), find_tot_time(kk_time));
		fprintf(stdout, "rep_rand:\n max: %ld \n min: %ld \n avg: %f\n time: %f\n\n", find_max(rep_rand_res), find_min(rep_rand_res), find_avg(rep_rand_res), find_tot_time(rep_rand_time));
		fprintf(stdout, "hill_climb:\n max: %ld \n min: %ld \n avg: %f\n time: %f\n\n", find_max(hill_climb_res), find_min(hill_climb_res), find_avg(hill_climb_res), find_tot_time(hill_climb_time));
		fprintf(stdout, "sim_anneal:\n max: %ld \n min: %ld \n avg: %f\n time: %f\n\n", find_max(sim_anneal_res), find_min(sim_anneal_res), find_avg(sim_anneal_res), find_tot_time(sim_anneal_time));
		fprintf(stdout, "rep_rand_pp:\n max: %ld \n min: %ld \n avg: %f\n time: %f\n\n", find_max(rep_rand_pp_res), find_min(rep_rand_pp_res), find_avg(rep_rand_pp_res), find_tot_time(rep_rand_pp_time));
		fprintf(stdout, "hill_climb_pp:\n max: %ld \n min: %ld \n avg: %f\n time: %f\n\n", find_max(hill_climb_pp_res), find_min(hill_climb_pp_res), find_avg(hill_climb_pp_res), find_tot_time(hill_climb_pp_time));
		fprintf(stdout, "sim_anneal_pp:\n max: %ld \n min: %ld \n avg: %f\n time: %f\n\n", find_max(sim_anneal_pp_res), find_min(sim_anneal_pp_res), find_avg(sim_anneal_pp_res), find_tot_time(sim_anneal_pp_time));

	}
	

	// fprintf(stdout, "time taken %f", time);
}