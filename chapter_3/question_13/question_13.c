/*
mpicc question_13.c -o question_13 && mpirun -n 4 ./question_13 5 "1,1,1,1,1,1" "2,2,2,2,2,2" 6 && rm question_13
*/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct input
{
    double *v_1;
    double *v_2;
    double scalar;
    int vector_size;
    int input_size;
} input;

input read_input(int argc, char *argv[]);
input read_local_input(int argc, char *argv[], int my_rank, int comm_sz);
double *read_vector(char *input, int vector_size);

double local_dot(double *v_1, double *v_2, int size);
double *local_scalar_multiply(double *v, int size, double scalar);

double get_global_dot(double *local_dot);
double *send_global_vector_to_root(double *local_vector, int local_size, int global_size, int my_rank, int comm_sz);
void print_vector(double *vector, int size);

void generate_counts_and_displacements(int *counts, int *displacements, int input_size, int comm_sz);

typedef struct range
{
    int first;
    int last;
} range;

range range_block_partition(int my_rank, int total_process, int input_size);

int main(int argc, char *argv[])
{

    int my_rank;
    int comm_sz;
    int input_size;

    int message_tag = 0;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    input local_input = read_local_input(argc, argv, my_rank, comm_sz);

    double local_dot_result = local_dot(local_input.v_1, local_input.v_2, local_input.vector_size);

    double *local_scalar_v1 = local_scalar_multiply(local_input.v_1, local_input.vector_size, local_input.scalar);
    double *local_scalar_v2 = local_scalar_multiply(local_input.v_2, local_input.vector_size, local_input.scalar);

    double dot_result = get_global_dot(&local_dot_result);

    double *scalar_v1 = send_global_vector_to_root(local_scalar_v1, local_input.vector_size,
                                                   local_input.input_size, my_rank, comm_sz);

    double *scalar_v2 = send_global_vector_to_root(local_scalar_v2, local_input.vector_size,
                                                   local_input.input_size, my_rank, comm_sz);

    double *input_v_1 = send_global_vector_to_root(local_input.v_1, local_input.vector_size,
                                                   local_input.input_size, my_rank, comm_sz);

    double *input_v_2 = send_global_vector_to_root(local_input.v_2, local_input.vector_size,
                                                   local_input.input_size, my_rank, comm_sz);

    if (my_rank == 0)
    {

        printf("\t\tInput\n");
        printf("vector size %i\n", local_input.input_size);
        printf("v_1: ");
        print_vector(input_v_1, local_input.input_size);
        printf("\n");
        printf("v_2: ");
        print_vector(input_v_2, local_input.input_size);
        printf("\n");
        printf("Scalar: %lf\n", local_input.scalar);

        printf("\t\t Result\n");
        printf("Dot: %lf\n", dot_result);
        printf("scalar v1: ");
        print_vector(scalar_v1, local_input.input_size);
        printf("\n");
        printf("scalar v2: ");
        print_vector(scalar_v2, local_input.input_size);
        printf("\n");
    }

    MPI_Finalize();

    return 0;
}

double local_dot(double *v_1, double *v_2, int size)
{
    double result = 0;
    for (int i = 0; i < size; i++)
    {
        result += v_1[i] * v_2[i];
    }

    return result;
}

double *local_scalar_multiply(double *v, int size, double scalar)
{
    double *result = malloc(size * sizeof(double));

    for (int i = 0; i < size; i++)
    {
        result[i] = v[i] * scalar;
    }

    return result;
}

double get_global_dot(double *local_dot)
{
    double result_dot;
    MPI_Reduce(local_dot, &result_dot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    return result_dot;
}

double *send_global_vector_to_root(double *local_vector, int local_size, int global_size, int my_rank, int comm_sz)
{
    double *global_vector = NULL;
    int *displacements = NULL;
    int *counts = NULL;
    if (my_rank == 0)
    {
        global_vector = malloc(global_size * sizeof(double));
        displacements = malloc(comm_sz * sizeof(int));
        counts = malloc(comm_sz * sizeof(int));
        generate_counts_and_displacements(counts, displacements, global_size, comm_sz);
    }

    MPI_Gatherv(local_vector, local_size, MPI_DOUBLE, global_vector, counts, displacements,
                MPI_DOUBLE, 0, MPI_COMM_WORLD);

    free(displacements);
    free(counts);
    return global_vector;
}
input read_local_input(int argc, char *argv[], int my_rank, int comm_sz)
{
    input local_input;
    input total_input;
    int *displacements = NULL;
    int *counts = NULL;
    if (my_rank == 0)
    {
        total_input = read_input(argc, argv);
        displacements = malloc(comm_sz * sizeof(int));
        counts = malloc(comm_sz * sizeof(int));
        generate_counts_and_displacements(counts, displacements, total_input.vector_size, comm_sz);
    }

    MPI_Bcast(&total_input.vector_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&total_input.scalar, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    range r = range_block_partition(my_rank, comm_sz, total_input.vector_size);

    local_input.vector_size = r.last - r.first;
    local_input.v_1 = malloc(local_input.vector_size * sizeof(double));
    local_input.v_2 = malloc(local_input.vector_size * sizeof(double));
    local_input.scalar = total_input.scalar;
    local_input.input_size = total_input.vector_size;

    MPI_Scatterv(total_input.v_1, counts, displacements, MPI_DOUBLE,
                 local_input.v_1, local_input.vector_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Scatterv(total_input.v_2, counts, displacements, MPI_DOUBLE,
                 local_input.v_2, local_input.vector_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    free(counts);
    free(displacements);

    return local_input;
}

void generate_counts_and_displacements(int *counts, int *displacements, int input_size, int comm_sz)
{

    for (size_t rank = 0; rank < comm_sz; rank++)
    {
        range r = range_block_partition(rank, comm_sz, input_size);
        /*
                           0 1 2 3 4
            input_vector =[a,b,c,d,e]

            p_0 = [a,b], counts[p_0] == 2, displacement[p_0] =0
            p_1 = [c], counts[p_1] == 1, displacement[p_1] == 2
            p_2 = [d], counts[p_2] == 1, displacement[p_2] == 3
            p_3 = [e], counts[p_3] == 1, displacement[p_3] == 4

            p_0 = input_vector[ displacement[p_0]: displacement[p_0] + counts[p_0] ] = [a,b]
        */
        counts[rank] = r.last - r.first;
        displacements[rank] = r.first;
    }
}

range range_block_partition(int my_rank, int total_process, int input_size)
{
    range r;

    int division = input_size / total_process;
    int rest = input_size % total_process;

    if (my_rank < rest)
    {
        r.first = my_rank * (division + 1);
        r.last = r.first + division + 1;
    }
    else
    {
        r.first = my_rank * division + rest;
        r.last = r.first + division;
    }

    return r;
}

double *read_vector(char *input, int vector_size)
{

    double *v = malloc(vector_size * sizeof(double));
    int current_vector_index = 0;

    for (size_t i = 0; input[i] != '\0'; i++)
    {

        if (input[i] != ',')
        {

            v[current_vector_index] = atoi(&input[i]);
            current_vector_index++;
        }
    }

    return v;
}

void print_vector(double *vector, int size)
{
    printf("[");
    for (int i = 0; i < size - 1; i++)
    {
        printf("%lf,", vector[i]);
    }
    printf("%lf]", vector[size - 1]);
}

input read_input(int argc, char *argv[])
{

    if (argc != 5)
    {
        printf("Número de entradas erradas\n");
        exit(1);
    }

    input result;

    result.scalar = atoi(argv[1]);

    result.vector_size = atoi(argv[4]);

    result.v_1 = read_vector(argv[2], result.vector_size);
    result.v_2 = read_vector(argv[3], result.vector_size);

    return result;
}
