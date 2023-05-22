/* Code for the Jacobi method of solving a system of linear equations 
 * by iteration.

 * Author: Naga Kandasamy
 * Date modified: APril 26, 2023
 *
 * Student name(s): FIXME
 * Date modified: FIXME
 *
 * Compile as follows:
 * gcc -o jacobi_solver jacobi_solver.c compute_gold.c -Wall -O3 -lpthread -lm 
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "jacobi_solver.h"
typedef struct {
    int thread_id;
    int num_threads;
    matrix_t A;
    matrix_t x_curr;
    matrix_t x_prev;
    matrix_t B;
    matrix_t temp;
} pthread_data_t;

pthread_barrier_t barrier;

/* Uncomment the line below to spit out debug information */ 
/* #define DEBUG */

int main(int argc, char **argv) 
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s matrix-size num-threads\n", argv[0]);
        fprintf(stderr, "matrix-size: width of the square matrix\n");
        fprintf(stderr, "num-threads: number of worker threads to create\n");
		exit(EXIT_FAILURE);
	}

    int matrix_size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    matrix_t  A;                    /* N x N constant matrix */
	matrix_t  B;                    /* N x 1 b matrix */
	matrix_t reference_x;           /* Reference solution */ 
    matrix_t mt_solution_x_v1;      /* Solution computed by pthread code using chunking */
    matrix_t mt_solution_x_v2;      /* Solution computed by pthread code using striding */

	/* Generate diagonally dominant matrix */
    fprintf(stderr, "\nCreating input matrices\n");
	srand(time(NULL));
	A = create_diagonally_dominant_matrix(matrix_size, matrix_size);
	if (A.elements == NULL) {
        fprintf(stderr, "Error creating matrix\n");
        exit(EXIT_FAILURE);
	}
	
    /* Create other matrices */
    B = allocate_matrix(matrix_size, 1, 1);
	reference_x = allocate_matrix(matrix_size, 1, 0);
	mt_solution_x_v1 = allocate_matrix(matrix_size, 1, 0);
    mt_solution_x_v2 = allocate_matrix(matrix_size, 1, 0);

#ifdef DEBUG
	print_matrix(A);
	print_matrix(B);
	print_matrix(reference_x);
#endif

    /* Compute Jacobi solution using reference code */
	fprintf(stderr, "Generating solution using reference code\n");
    int max_iter = 100000; /* Maximum number of iterations to run */
    compute_gold(A, reference_x, B, max_iter);
    display_jacobi_solution(A, reference_x, B); /* Display statistics */
	
	/* Compute the Jacobi solution using pthreads. 
     * Solutions are returned in mt_solution_x_v1 and mt_solution_x_v2.
     * */
    fprintf(stderr, "\nPerforming Jacobi iteration using pthreads using chunking\n");
	compute_using_pthreads_v1(A, mt_solution_x_v1, B, max_iter, num_threads);
    display_jacobi_solution(A, mt_solution_x_v1, B); /* Display statistics */
    
    fprintf(stderr, "\nPerforming Jacobi iteration using pthreads using striding\n");
	compute_using_pthreads_v2(A, mt_solution_x_v2, B, max_iter, num_threads);
    display_jacobi_solution(A, mt_solution_x_v2, B); /* Display statistics */

    free(A.elements); 
	free(B.elements); 
	free(reference_x.elements); 
	free(mt_solution_x_v1.elements);
    free(mt_solution_x_v2.elements);
	
    exit(EXIT_SUCCESS);
}

/* FIXME: Complete this function to perform the Jacobi calculation using pthreads using chunking. 
 * Result must be placed in mt_sol_x_v1. */
void* jacobi_v1(void* arg)
{
    pthread_data_t* data = (pthread_data_t*)arg;
    int thread_id = data->thread_id;
    int num_threads = data->num_threads;
    matrix_t A = data->A;
    matrix_t x_curr = data->x_curr;
    matrix_t x_prev = data->x_prev;
    matrix_t B = data->B;
    matrix_t temp = data->temp;
	int chunk_size = ceil((double)A.num_rows / num_threads);
    int start_row = thread_id * chunk_size;
    int end_row = start_row + chunk_size;

    int i, j;
    for (i = start_row; i < end_row; i++) {
        temp.elements[i] = B.elements[i];
        for (j = 0; j < A.num_columns; j++) {
            if (i != j)
                temp.elements[i] -= A.elements[i * A.num_columns + j] * x_prev.elements[j];
        }
        temp.elements[i] /= A.elements[i * A.num_columns + i];
    }

    // Wait for all threads to complete the chunk
    pthread_barrier_wait(&barrier);

    // Copy temp values to the x_curr matrix
    for (i = start_row; i < end_row; i++) {
        x_curr.elements[i] = temp.elements[i];
    }

    pthread_exit(NULL);
}

void* jacobi_v2(void* arg)
{
    pthread_data_t* data = (pthread_data_t*)arg;
    int thread_id = data->thread_id;
    int num_threads = data->num_threads;
    matrix_t A = data->A;
    matrix_t x_curr = data->x_curr;
    matrix_t x_prev = data->x_prev;
    matrix_t B = data->B;
    matrix_t temp = data->temp;

    int i, j;
    float sum;

    for (i = thread_id; i < A.num_rows; i += num_threads) {
        sum = 0.0;
        for (j = 0; j < A.num_columns; j++) {
            if (i != j)
                sum += A.elements[i * A.num_columns + j] * x_prev.elements[j];
        }

        temp.elements[i] = (B.elements[i] - sum) / A.elements[i * A.num_columns + i];
    }

    // Wait for all threads to complete their stride
    pthread_barrier_wait(&barrier);

    // Copy temp values to the x_curr matrix
    for (i = thread_id; i < A.num_rows; i += num_threads) {
        x_curr.elements[i] = temp.elements[i];
    }

    pthread_exit(NULL);
}

void compute_using_pthreads_v1(const matrix_t A, matrix_t mt_sol_x_v1, const matrix_t B, int max_iter, int num_threads)
{
    int i, k;
    matrix_t x_prev = allocate_matrix(A.num_rows, 1, 0);
    matrix_t temp = allocate_matrix(A.num_rows, 1, 0);
    pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    pthread_data_t* thread_data = (pthread_data_t*)malloc(num_threads * sizeof(pthread_data_t));
    pthread_barrier_init(&barrier, NULL, num_threads);

    // Perform Jacobi iteration
    for (k = 0; k < max_iter; k++) {
        // Swap x_prev and mt_sol_x_v1
        matrix_t temp_matrix = x_prev;
        x_prev = mt_sol_x_v1;
        mt_sol_x_v1 = temp_matrix;

        // Create threads and distribute the work (chunking)
        for (i = 0; i < num_threads; i++) {
            thread_data[i].thread_id = i;
            thread_data[i].num_threads = num_threads;
            thread_data[i].A = A;
            thread_data[i].x_curr = mt_sol_x_v1;
            thread_data[i].x_prev = x_prev;
            thread_data[i].B = B;
            thread_data[i].temp = temp;

            pthread_create(&threads[i], NULL, jacobi_v1, (void*)&thread_data[i]);
        }

        // Wait for all threads to complete
        for (i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
        }
    }

    // Free allocated memory
    free(x_prev.elements);
    free(temp.elements);
    free(threads);
    free(thread_data);
    pthread_barrier_destroy(&barrier);
}

/* FIXME: Complete this function to perform the Jacobi calculation using pthreads using striding. 
 * Result must be placed in mt_sol_x_v2. */
void compute_using_pthreads_v2(const matrix_t A, matrix_t mt_sol_x_v2, const matrix_t B, int max_iter, int num_threads)
{
    // Variable declarations
    int i, k;
    matrix_t x_prev = allocate_matrix(A.num_rows, 1, 0);
    matrix_t temp = allocate_matrix(A.num_rows, 1, 0);
    pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    pthread_data_t* thread_data = (pthread_data_t*)malloc(num_threads * sizeof(pthread_data_t));
    pthread_barrier_init(&barrier, NULL, num_threads);

    // Perform Jacobi iteration
    for (k = 0; k < max_iter; k++) {
        // Swap x_prev and mt_sol_x_v2
        matrix_t temp_matrix = x_prev;
        x_prev = mt_sol_x_v2;
        mt_sol_x_v2 = temp_matrix;

        // Create threads and distribute the work (striding)
        for (i = 0; i < num_threads; i++) {
            thread_data[i].thread_id = i;
            thread_data[i].num_threads = num_threads;
            thread_data[i].A = A;
            thread_data[i].x_curr = mt_sol_x_v2;
            thread_data[i].x_prev = x_prev;
            thread_data[i].B = B;
            thread_data[i].temp = temp;

            pthread_create(&threads[i], NULL, jacobi_v2, (void*)&thread_data[i]);
        }

        // Wait for all threads to complete
        for (i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
        }
    }

    // Free allocated memory
    free(x_prev.elements);
    free(temp.elements);
    free(threads);
    free(thread_data);
    pthread_barrier_destroy(&barrier);
}



/* Allocate a matrix of dimensions height * width.
   If init == 0, initialize to all zeroes.  
   If init == 1, perform random initialization.
*/
matrix_t allocate_matrix(int num_rows, int num_columns, int init)
{
    int i;    
    matrix_t M;
    M.num_columns = num_columns;
    M.num_rows = num_rows;
    int size = M.num_rows * M.num_columns;
		
	M.elements = (float *)malloc(size * sizeof(float));
	for (i = 0; i < size; i++) {
		if (init == 0) 
            M.elements[i] = 0; 
		else
            M.elements[i] = get_random_number(MIN_NUMBER, MAX_NUMBER);
	}
    
    return M;
}	

/* Print matrix to screen */
void print_matrix(const matrix_t M)
{
    int i, j;
	for (i = 0; i < M.num_rows; i++) {
        for (j = 0; j < M.num_columns; j++) {
			fprintf(stderr, "%f ", M.elements[i * M.num_columns + j]);
        }
		
        fprintf(stderr, "\n");
	} 
	
    fprintf(stderr, "\n");
    return;
}

/* Return a floating-point value between [min, max] */
float get_random_number(int min, int max)
{
    float r = rand ()/(float)RAND_MAX;
	return (float)floor((double)(min + (max - min + 1) * r));
}

/* Check if matrix is diagonally dominant */
int check_if_diagonal_dominant(const matrix_t M)
{
    int i, j;
	float diag_element;
	float sum;
	for (i = 0; i < M.num_rows; i++) {
		sum = 0.0; 
		diag_element = M.elements[i * M.num_rows + i];
		for (j = 0; j < M.num_columns; j++) {
			if (i != j)
				sum += abs(M.elements[i * M.num_rows + j]);
		}
		
        if (diag_element <= sum)
			return -1;
	}

	return 0;
}

/* Create diagonally dominant matrix */
matrix_t create_diagonally_dominant_matrix(int num_rows, int num_columns)
{
	matrix_t M;
	M.num_columns = num_columns;
	M.num_rows = num_rows; 
	int size = M.num_rows * M.num_columns;
	M.elements = (float *)malloc(size * sizeof(float));

    int i, j;
	fprintf(stderr, "Generating %d x %d matrix with numbers between [-.5, .5]\n", num_rows, num_columns);
	for (i = 0; i < size; i++)
        M.elements[i] = get_random_number(MIN_NUMBER, MAX_NUMBER);
	
	/* Make diagonal entries large with respect to the entries on each row. */
    float row_sum;
	for (i = 0; i < num_rows; i++) {
		row_sum = 0.0;		
		for (j = 0; j < num_columns; j++) {
			row_sum += fabs(M.elements[i * M.num_rows + j]);
		}
		
        M.elements[i * M.num_rows + i] = 0.5 + row_sum;
	}

    /* Check if matrix is diagonal dominant */
	if (check_if_diagonal_dominant(M) < 0) {
		free(M.elements);
		M.elements = NULL;
	}
	
    return M;
}



