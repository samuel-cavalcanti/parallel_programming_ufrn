/*
mpicc  bench.c -o bench && mpirun  -n 4 ./bench 8 1,2,3,4,5,6,7,8  && rm bench
*/
#define _GNU_SOURCE

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct input
{
    double *v_1;
    int vector_size;
    int input_size;
} input;

input read_input(int argc, char *argv[]);
input read_local_input(int argc, char *argv[], int my_rank, int comm_sz);
double *read_vector(char *input, int vector_size);

typedef struct range
{
    int first;
    int last;
} range;

range range_block_partition(int my_rank, int total_process, int input_size);

void print_vector(double *vector, int size);

double *send_global_vector_to_root(double *local_vector, int local_size,
                                   int global_size, int my_rank);

double sum_elements(double *vector, int size);

double global_sum_ring_pass(double local_sum, int my_rank,
                            int comm_sz, int message_tag);

double global_sum_butterfly(double local_sum, int my_rank,
                            int comm_sz, int message_tag);

int main(int argc, char *argv[])
{

    int my_rank;
    int comm_sz;

    int message_tag = 0;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    input local_input = read_local_input(argc, argv, my_rank, comm_sz);

    double sum = sum_elements(local_input.v_1, local_input.vector_size);

    double ring_pass_start, ring_pass_end;

    MPI_Barrier(MPI_COMM_WORLD);
    ring_pass_start = MPI_Wtime();
    double sum_ring_pass = global_sum_ring_pass(sum, my_rank, comm_sz, message_tag);
    ring_pass_end = MPI_Wtime();

    // Blocks until all processes in the communicator have reached this routine.
    MPI_Barrier(MPI_COMM_WORLD); // https://www.mpich.org/static/docs/v3.2/www3/MPI_Barrier.html

    double butterfly_start, butterfly_end;

    // returns Time in seconds since an arbitrary time in the past.
    butterfly_start = MPI_Wtime(); // https://www.mpich.org/static/docs/v3.2/www3/MPI_Wtime.html
    double sum_butterfly = global_sum_butterfly(sum, my_rank, comm_sz, message_tag);
    butterfly_end = MPI_Wtime();

    double ring_pass_time = ring_pass_end - ring_pass_start;
    double butterfly_time = butterfly_end - butterfly_start;

    double *vector_input = send_global_vector_to_root(local_input.v_1, local_input.vector_size,
                                                      local_input.input_size, my_rank);

    double *ring_pass_time_vector = send_global_vector_to_root(&ring_pass_time,
                                                               1, comm_sz, my_rank);

    double *butterfly_time_vector = send_global_vector_to_root(&butterfly_time,
                                                               1, comm_sz, my_rank);

    if (my_rank == 0)
    {
        double mean_time_butterfly = sum_elements(butterfly_time_vector, comm_sz) / comm_sz;
        double mean_time_ring_pass = sum_elements(ring_pass_time_vector, comm_sz) / comm_sz;

        // printf("\t\tInput\n");
        // printf("vector size %i\n", local_input.input_size);
        // printf("vector input: ");
        // print_vector(vector_input, local_input.input_size);
        // printf("\n");

        // printf("\t\tResult\n");

        // printf("sum-ring-pass: %lf sum-butterfly: %lf\n", sum_ring_pass, sum_butterfly);
        // printf("#butterfly-time vector: ");
        // print_vector(butterfly_time_vector, comm_sz);
        // printf("\n");

        // printf("#ring-pass-time vector: ");
        // print_vector(ring_pass_time_vector, comm_sz);
        // printf("\n");

        //     algorithm   time-in-nsec   number-process

        printf("butterfly-time %lf %i\n", mean_time_butterfly, comm_sz);
        printf("ring-pass-time %lf %i\n", mean_time_ring_pass, comm_sz);
    }

    MPI_Finalize();

    return 0;
}

double sum_elements(double *vector, int size)
{
    double sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += vector[i];
    }
    return sum;
}

double global_sum_ring_pass(double local_sum, int my_rank, int comm_sz, int message_tag)
{
    /*  seja p, o número de processos, e q um rank de um processo.

       processo q enviar dado para q + 1, caso  q + 1 > p então q envia para 0

       analogamente, processo q recebe dado q -1,caso q == 0 então q recebe p-1
   */
    int last_process = comm_sz - 1;

    int dest = my_rank == last_process ? 0 : my_rank + 1;

    int source = my_rank == 0 ? last_process : my_rank - 1;

    double global_sum = 0;
    double value = local_sum;

    for (int phase = 0; phase < comm_sz; phase++)
    {
        MPI_Sendrecv_replace(&value, 1, MPI_DOUBLE, dest, message_tag,
                             source, message_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        global_sum += value;
    }

    return global_sum;
}

double global_sum_butterfly(double local_sum, int my_rank,
                            int comm_sz, int message_tag)
{

    double global_sum = local_sum;
    for (int k = 0; (1 << k) < comm_sz; k++)
    // k varia entre  0,1,2,4,8,16 ... até ser maior que comm_sz
    {
        int partner = my_rank ^ (1 << k); // por algum motivo isso funciona.
        double last_sum_partner;

        MPI_Sendrecv(&global_sum, 1, MPI_DOUBLE, partner, message_tag,
                     &last_sum_partner, 1, MPI_DOUBLE, partner, message_tag,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        /*
            mesmo que processo 0 não atualiza seu vetor local com o último valor do processo 1.
            o processo 0 deve enviar o seu valor + o do processo 1 para o processo 2.
        */
        global_sum += last_sum_partner;
    }

    return global_sum;
}

double *send_global_vector_to_root(double *local_vector, int local_size,
                                   int global_size, int my_rank)
{
    double *global_vector = NULL;

    if (my_rank == 0)
    {
        global_vector = malloc(global_size * sizeof(double));
    }
    MPI_Gather(local_vector, local_size, MPI_DOUBLE, global_vector,
               local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    return global_vector;
}
input read_local_input(int argc, char *argv[], int my_rank, int comm_sz)
{
    input local_input;
    input total_input;

    total_input.v_1 = NULL;

    if (my_rank == 0)
    {
        total_input = read_input(argc, argv);
    }

    MPI_Bcast(&total_input.vector_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    range r = range_block_partition(my_rank, comm_sz, total_input.vector_size);

    local_input.vector_size = r.last - r.first;
    local_input.v_1 = malloc(local_input.vector_size * sizeof(double));
    local_input.input_size = total_input.vector_size;

    MPI_Scatter(total_input.v_1, local_input.vector_size, MPI_DOUBLE,
                local_input.v_1, local_input.vector_size, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

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

    if (argc != 3)
    {
        printf("Número de entradas erradas\n");
        exit(1);
    }

    input result;

    result.vector_size = atoi(argv[1]);
    result.v_1 = read_vector(argv[2], result.vector_size);

    return result;
}
