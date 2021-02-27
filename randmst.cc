#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <vector>
#include <array>
#include <ctime>
#include <math.h>

typedef struct {
	int vertex;
	double dist;
} tuple;


struct heap{
	tuple* min_heap;
	int size;
	int space;
};

typedef struct heap heap;



struct graph_node {
	int vertex;
	double weight;
	struct graph_node *next;
} ;

struct graph {
	int vertex_num;
	struct graph_node** adj_list;
};

struct graph* create_weighted_graph_00(int n) {
	double bound = 1;
	struct graph* g = (graph *) malloc(sizeof(struct graph));
	g->vertex_num = n;
	g->adj_list = (graph_node**) malloc(sizeof(struct graph_node*)*n);
	srand((unsigned) time(NULL));
	for (int i = 0; i < n; i++) {
		g->adj_list[i] = NULL;
		for (int j = 0; j< n; j++) {
			double r = ((double) rand() / (RAND_MAX));
			if (r < bound) {
				struct graph_node* node = (graph_node *) malloc(sizeof(struct graph_node));
				node->vertex = j;
				node->weight = r;
				node->next = g->adj_list[i];
				g->adj_list[i] = node;
			}
		}
	}
	return g;
}

struct graph* create_weighted_graph_cube(int n, int dim) {
	double bound = 10 * pow(n, -1/dim);
	struct graph* g = (graph *) malloc(sizeof(struct graph));
	g->vertex_num = n;
	g->adj_list = (graph_node**) malloc(sizeof(struct graph_node*)*n);
	srand((unsigned) time(NULL));
	for (int i = 0; i < n; i++) {
		g->adj_list[i] = NULL;
		for (int j = 0; j< n; j++) {
			double r1[dim]; 
			double r2[dim];
			double euc_dist = 0;
			for (int k = 0; k < dim; k++) {
				r1[k] = ((double) rand() / (RAND_MAX));
				r2[k] = ((double) rand() / (RAND_MAX));
				euc_dist = euc_dist + (r1[k] - r2[k]) * (r1[k] - r2[k]);
			}
			euc_dist = sqrt(euc_dist);
			if (euc_dist < bound) {
				struct graph_node* node = (graph_node *) malloc(sizeof(struct graph_node));
				node->vertex = j;
				node->weight = euc_dist;
				node->next = g->adj_list[i];
				g->adj_list[i] = node;
			}
		}
	}
	return g;
}



tuple create_tuple (int vertex, double dist) {
	tuple s;
	s.vertex = vertex;
	s.dist = dist;
	return s;
}

void min_heapify(heap *h, int node) {
	int left = node*2 + 1;
	int right = node*2 + 2;
	int smallest;
	if ((left < h->size) && (h->min_heap[left].dist < h->min_heap[node].dist)) {
		smallest = left;
	}
	else {
		smallest = node;
	}
	if (right < h->size && h->min_heap[right].dist < h->min_heap[smallest].dist) {
		smallest = right;
	}
	if (smallest != node) {
		tuple swapper = h->min_heap[smallest];
		h->min_heap[smallest] = h->min_heap[node];
		h->min_heap[node] = swapper;
		min_heapify(h, smallest);
	}
}

tuple extract_min(heap *h) {
	tuple min = h->min_heap[0];
	h->min_heap[0] = h->min_heap[h->size-1];
	h->size -= 1;
	min_heapify(h, 0);
	return min;
}

tuple peek(heap *h) {
	return h->min_heap[0];
}

void insert(heap *h, tuple value) {
	h->size += 1;
	h->min_heap[h->size - 1] = value;
	int node = h->size - 1;
	while (node != 0 && (h->min_heap[(node-1)/2].dist < h->min_heap[node].dist)) {
		tuple swapper = h->min_heap[(node-1)/2];
		h->min_heap[(node-1)/2] = h->min_heap[node];
		h->min_heap[node] = swapper;
		node = (node-1)/2;
	}
}

heap* create_heap(tuple value, int space) {
	heap *h = (heap *) malloc(sizeof(heap)); 
	h->space = space;
	h->min_heap = (tuple *) malloc(sizeof(tuple)*space);
	h->min_heap[0] = value;
	h->size = 1;
	return h;
}



double prim_algorithm_0(struct graph* graph, int n) {
	double dist[n];
	int prev[n];
	int S[n];
	bool alarm = false;
	for (int i = 0; i < n; i++) {
		S[i] = 0;
	} 
	tuple start = create_tuple(0,0);
	heap *h = create_heap(start, n);
	for (int i = 0; i < n; i++) {
		dist[i] = INT_MAX;
		prev[i] = -1;
	}
	dist[0] = 0;
	while (h->size != 0) {
		tuple min = extract_min(h);
		S[min.vertex] = 1;
		double min_dist = 2;
		int insert_vert = -1;
		struct graph_node* ptr = graph->adj_list[min.vertex];
		while (ptr != NULL) {
			if (S[ptr->vertex] == 0) {
				if (dist[ptr->vertex] > ptr->weight) {
					dist[ptr->vertex] = ptr->weight;
					prev[ptr->vertex] = min.vertex; 
					if (dist[ptr->vertex] < min_dist) {
						min_dist = dist[ptr->vertex];
						insert_vert = ptr->vertex;
					}
				}
			}
			ptr = ptr->next;
		}
		if (insert_vert > -1) {
			tuple next_insert = create_tuple(insert_vert, dist[insert_vert]);
			insert(h, next_insert);
		}
	}
	double sum = 0;
	double max_dist = 0;
	for (int i = 0; i < n; i++) {
		sum = sum + dist[i];
		if (dist[i] > max_dist) {
			max_dist = dist[i];
		}
	}
	return sum;
}

int main(int argc, char *argv[]) {
	if (argc != 5) {
		fprintf(stderr, "Error: expecting 4 arguments");
		return 1;
	}
	char* p1;
	int flag;
	long conv1 = strtol(argv[1], &p1, 10);
	flag = conv1;
	char* p2;
	int numpoints;
	long conv2 = strtol(argv[2], &p2, 10);
	numpoints = conv2;
	char* p3;
	int numtrials;
	long conv3 = strtol(argv[3], &p3, 10);
	numtrials = conv3;
	char* p4;
	int dimension;
	long conv4 = strtol(argv[4], &p4, 10);
	dimension = conv4;
	if (dimension == 0) {
		double trials[numtrials];
		for (int i = 0; i < numtrials; i++) {
			struct graph* g = create_weighted_graph_00(numpoints);
			trials[i] = prim_algorithm_0(g, numpoints);
		}
		double average = 0;
		for (int j = 0; j < numtrials; j++) {
			average += trials[j];
		}
		average = average/numtrials;
		fprintf(stdout, "Average weight of graph: %f\n", average);
	}
	else if (dimension < 2 || dimension > 4) {
		fprintf(stderr, "Error: expecting dimension to be 0, 2, 3, or 4");
	}
	else {
		double trials_cube[numtrials];
		for (int i = 0; i < numtrials; i++) {
			struct graph* g = create_weighted_graph_cube(numpoints, dimension);
			trials_cube[i] = prim_algorithm_0(g, numpoints);
		}
		double average_cube = 0;
		for (int j = 0; j < numtrials; j++) {
			average_cube += trials_cube[j];
		}
		average_cube = average_cube/numtrials;
		fprintf(stdout, "Average weight of graph: %f\n", average_cube);
	}
}
