/*
mpicc  ring_pass_prefix_sums.c -o ring_pass_prefix_sums && mpirun  -n 4 ./ring_pass_prefix_sums 8 1,2,3,4,5,6,7,8  && rm ring_pass_prefix_sums
*/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
double *calculate_n_prefix_sums(double v[], int size);

void update_prefix_sums_vector(double vector[], int size, double value);
double receive_last_prefix_sum(int source_node, int message_tag);
void update_prefix_sums(int my_rank, double *local_prefix_sum,
                        int vector_size, int message_tag);

int main(int argc, char *argv[])
{

    int my_rank;
    int comm_sz;

    int message_tag = 0;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    input local_input = read_local_input(argc, argv, my_rank, comm_sz);
    double *local_prefix_sum = calculate_n_prefix_sums(local_input.v_1,
                                                       local_input.vector_size);

    /*  seja p, o número de processos, e q um rank de um processo.

     processo q enviar dado para q + 1, caso  q + 1 > p então q envia para 0

     analogamente, processo q recebe dado q -1,caso q == 0 então q recebe p-1
 */
    int last_process = comm_sz - 1;

    int dest = my_rank == last_process ? 0 : my_rank + 1;

    int source = my_rank == 0 ? last_process : my_rank - 1;

    double value;

    for (int phase = 1; phase < comm_sz; phase++)
    {
        if (phase != dest)
            value = 0;
        else
            value = local_prefix_sum[local_input.vector_size - 1];

        /*  [1,2] ,[3,4],[5,6], [7,8]
             [3]    [7]   [11]  [15]
            phase 1:
              p_0  3+0   -> 0
              p_1  7+3   -> 10
              p_2  11+0  -> 11
              p_3  15+0  -> 15
            phase 2:
              p_0  3+0   -> 0
              p_1  10+0  -> 10
              p_2  11+10 -> 21
              p_3  15+0  -> 15
            phase 3:
              p_0  3+0   -> 0
              p_1  10+0  -> 10
              p_2  21+0  -> 21
              p_3  15+21 -> 36
        */

        MPI_Sendrecv_replace(&value, 1, MPI_DOUBLE, dest, message_tag,
                             source, message_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        update_prefix_sums_vector(local_prefix_sum, local_input.vector_size, value);
    }

    double *global_prefix_sum = send_global_vector_to_root(local_prefix_sum,
                                                           local_input.vector_size,
                                                           local_input.input_size,
                                                           my_rank);

    double *vector_input = send_global_vector_to_root(local_input.v_1, local_input.vector_size,
                                                      local_input.input_size, my_rank);

    if (my_rank == 0)
    {
        printf("\t\tInput\n");
        printf("vector size %i\n", local_input.input_size);
        printf("vector input: ");
        print_vector(vector_input, local_input.input_size);
        printf("\n");

        printf("\t\tResult\n");
        printf("Prefix sum: ");
        print_vector(global_prefix_sum, local_input.input_size);
        printf("\n");
    }

    MPI_Finalize();

    return 0;
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

void update_prefix_sums(int my_rank, double *local_prefix_sum,
                        int vector_size, int message_tag)
{
    int source_node = my_rank - 1;
    double last_prefix_sum_source_node;
    MPI_Recv(&last_prefix_sum_source_node,
             1,
             MPI_DOUBLE,
             source_node,
             message_tag,
             MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
}

double receive_last_prefix_sum(int source_node, int message_tag)
{
    double last_prefix_sum_source_node;
    MPI_Recv(&last_prefix_sum_source_node, 1, MPI_DOUBLE,
             source_node, message_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    return last_prefix_sum_source_node;
}

double *calculate_n_prefix_sums(double v[], int size)
{

    double *result = malloc(size * sizeof(double));

    result[0] = v[0];

    for (int i = 1; i < size; i++)
        result[i] = result[i - 1] + v[i];

    return result;
}

void update_prefix_sums_vector(double vector[], int size, double value)
{
    for (int i = 0; i < size; i++)
        vector[i] += value;
}
