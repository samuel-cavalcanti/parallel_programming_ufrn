#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <mpi.h>
int main(void)
{
    int my_rank, comm_sz;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int tag = 0;
    const char template[] = "Proc %d of %d > Does anyone have a toothpick?\n";
    size_t template_size = 47;

    if (my_rank == 0)
    {
        printf(template, my_rank, comm_sz);
        for (size_t i = 1; i < comm_sz; i++)
        {
            char message[template_size];
            MPI_Recv(message, template_size + 1, MPI_CHAR, i, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s", message);
        }
    }
    else
    {
        char message[template_size];
        sprintf(message, template, my_rank, comm_sz);
        MPI_Send(message, template_size + 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
