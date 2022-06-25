# Questão 9

Write an MPI program that implements multiplication of a vector by a scalar and dot product. The user should enter two vectors and a scalar, all of which are read in by process 0 and distributed among the processes. The results are calculated and collected onto process 0, which prints them. You can assume that $n$, the order of the vectors, is evenly divisible by _comm_sz_


## código
```c
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
} input;

input read_input(int argc, char *argv[]);
input read_local_input(int argc, char *argv[], int my_rank, int comm_sz);
double *read_vector(char *input, int vector_size);

double local_dot(double *v_1, double *v_2, int size);
double *local_scalar_multiply(double *v, int size, double scalar);

double get_global_dot(double *local_dot);
double *get_global_vector(double *local_vector, int local_size, int global_size, int my_rank);
void print_vector(double *vector, int size);

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

    double *scalar_v1 = get_global_vector(local_scalar_v1, local_input.vector_size, local_input.vector_size * comm_sz, my_rank);
    double *scalar_v2 = get_global_vector(local_scalar_v2, local_input.vector_size, local_input.vector_size * comm_sz, my_rank);

    if (my_rank == 0)
    {
        printf("\t\t Result\n");
        printf("Dot: %lf\n", dot_result);
        printf("scalar v1: ");
        print_vector(scalar_v1, local_input.vector_size * comm_sz);
        printf("\n");
        printf("scalar v2: ");
        print_vector(scalar_v2, local_input.vector_size * comm_sz);
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

double *get_global_vector(double *local_vector, int local_size, int global_size, int my_rank)
{
    double *global_vector = NULL;

    if (my_rank == 0)
    {
        global_vector = malloc(global_size * sizeof(double));
    }
    MPI_Gather(local_vector, local_size, MPI_DOUBLE, global_vector, local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    return global_vector;
}
input read_local_input(int argc, char *argv[], int my_rank, int comm_sz)
{
    input local_input;
    input total_input;

    if (my_rank == 0)
    {
        total_input = read_input(argc, argv);

        printf("\t\tInput\n");
        printf("vector size %i\n", total_input.vector_size);
        printf("v_1: ");
        print_vector(total_input.v_1, total_input.vector_size);
        printf("\n");
        printf("v_2: ");
        print_vector(total_input.v_2, total_input.vector_size);
        printf("\n");
        printf("Scalar: %lf\n", total_input.scalar);
    }

    MPI_Bcast(&total_input.vector_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&total_input.scalar, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    range r = range_block_partition(my_rank, comm_sz, total_input.vector_size);

    local_input.vector_size = r.last - r.first;
    local_input.v_1 = malloc(local_input.vector_size * sizeof(double));
    local_input.v_2 = malloc(local_input.vector_size * sizeof(double));
    local_input.scalar = total_input.scalar;

    MPI_Scatter(total_input.v_1, local_input.vector_size, MPI_DOUBLE, local_input.v_1, local_input.vector_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(total_input.v_2, local_input.vector_size, MPI_DOUBLE, local_input.v_2, local_input.vector_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    return local_input;
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
```
## teste
```bash
mpirun -n 4 ./question_9 5 "1,1,1,1" "2,2,2,2" 4
                Input
vector size 4
v_1: [1.000000,1.000000,1.000000,1.000000]
v_2: [2.000000,2.000000,2.000000,2.000000]
Scalar: 5.000000
                 Result
Dot: 8.000000
scalar v1: [5.000000,5.000000,5.000000,5.000000]
scalar v2: [10.000000,10.000000,10.000000,10.000000]
```