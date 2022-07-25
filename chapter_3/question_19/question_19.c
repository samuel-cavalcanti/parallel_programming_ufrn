/*
mpicc -lm question_19.c -o question_19 && mpirun -n 2 ./question_19 "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15" 16 && rm question_19
*/
#include <mpi.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef struct input
{
    double *v_1;

    int vector_size;
    int input_size;
} input;

input read_input(int argc, char *argv[]);
input read_local_input(int argc, char *argv[], int my_rank, int comm_sz);
double *read_vector(char *input, int vector_size);

void print_vector(double *vector, int size);
void print_matrix(double *matrix, int rank);
void generate_counts_and_displacements(int *counts, int *displacements, int input_size, int comm_sz);

typedef struct range
{
    int first;
    int last;
} range;

range range_block_partition(int my_rank, int total_process, int input_size);

MPI_Datatype create_upper_triangular_type(int rank_matrix);

int main(int argc, char *argv[])
{

    int my_rank;
    int comm_sz;
    int input_size;

    // https://www.rookiehpc.com/mpi/docs/mpi_type_indexed.php

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (comm_sz != 2)
    {
        printf("esse exercício utiliza apenas de  2 processo.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    int matrix_rank;
    input local_input;
    int source = 0;
    int dest = 1;
    double *upper_triangle = NULL;

    int message_tag = 0;

    if (my_rank == source)
    {

        local_input = read_input(argc, argv);
        // o rank de uma matriz quadrada é a raiz do seu número de elementos
        matrix_rank = sqrt(local_input.input_size);
        printf("\t\tInput\n");
        printf("vector size %i\n", local_input.input_size);
        printf("rank %i\n", matrix_rank);
        printf("\t\tmatrix \n");
        print_matrix(local_input.v_1, matrix_rank);
        printf("\n");

        MPI_Datatype triangular_type = create_upper_triangular_type(matrix_rank);

        MPI_Send(local_input.v_1, 1, triangular_type, dest, 0, MPI_COMM_WORLD);
    }

    MPI_Bcast(&matrix_rank, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_rank == dest)
    {
        int size = matrix_rank*(matrix_rank +1)/2;
        upper_triangle = malloc(size * sizeof(double));
        for (size_t i = 0; i < size; i++)
        {
            upper_triangle[i] = 0.0;
        }
        MPI_Recv(upper_triangle, size, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("\t\tResult\n");
        printf("upper triangular part of a square matrix: \n");
        print_vector(upper_triangle, size);
        printf("\n");
    }

    MPI_Finalize();

    return 0;
}

MPI_Datatype create_upper_triangular_type(int rank_matrix)
{
    MPI_Datatype custom;

    /*
        v[rank_matrix][rank_matrix] =
        {
            {0, 1 ,2, 3}, v[0,0:4]
            {4, 5, 6, 7}, v[1,1:4]
            {8, 9, 10,11},v[2,2:4]
            {12,13,14,15} v[3,3:4]
        }
    */
    int *lengths = malloc(rank_matrix * sizeof(int));
    int *displacements = malloc(rank_matrix * sizeof(int));

    for (size_t i = 0; i < rank_matrix; i++)
    {
        int begin = i;
        int end = rank_matrix;
        lengths[i] = end - begin; // end - begin == len(v[i][i:4])
        /*
        mapeando  matrix v[row][col] para sua posição em um vetor 1d  v[row*width + col]
        portanto v[2][2] == v[2*rank_matrix + 2]
        */
        displacements[i] = rank_matrix * i + i;
    }
    /*
    o número de elementos da matrix triangular superior é
    metade do número da matriz, portanto: n*n/n == n
    */
    /*

    /*
     MPI_Type_indexed creates an MPI datatype as a sequence of blocks,
     each made by replicating an existing MPI_Datatype a certain number of times.
     Also, each block is located at a certain displacement from the beginning of the MPI datatype.

     https://www.rookiehpc.com/mpi/docs/mpi_type_indexed.php
    */
    MPI_Type_indexed(rank_matrix, lengths, displacements, MPI_DOUBLE, &custom);

    MPI_Type_commit(&custom);

    return custom;
}

double *read_vector(char *input, int vector_size)
{

    double *v = malloc(vector_size * sizeof(double));
    int current_vector_index = 0;

    const char delimiter[] = ",";

    for (size_t i = 0; i < vector_size; i++)
    {

        v[i] = strtod(input, &input);
        input++; // increment comma
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

void print_matrix(double *matrix, int rank)
{

    for (size_t i = 0; i < rank; i++)
    {
        print_vector(&matrix[i * rank], rank);
        printf("\n");
    }
}

input read_input(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Número de entradas erradas\n");
        exit(1);
    }

    input result;

    result.vector_size = atoi(argv[2]);
    result.input_size = result.vector_size;
    result.v_1 = read_vector(argv[1], result.vector_size);

    return result;
}
