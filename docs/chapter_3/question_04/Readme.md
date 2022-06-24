# Questão 4

Modify the program that just prints a line of output from each process (_mpi_output.c_) so that the output is printed in process rank order: process 0s output first, then process 1s, and so on.

```c
#include <stdio.h>
#include <mpi.h>
int main(void)
{
    int my_rank, comm_sz;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    printf("Proc %d of %d > Does anyone have a toothpick?\n",
           my_rank, comm_sz);

    MPI_Finalize();
    return 0;
}
```

Sabendo que o acesso ao stdout está sendo requisitado por todos os
processos e que não existe por padrão um mecanismo que ordene as mensagens. então uma proposta para resolver o problema é justamente definir que a responsabilidade de exibir mensagens no stdout será data ao processo de rank 0 e que todo processo que
deseja utilizar o stdout, enviará uma mensagem para o processo de rank 0.

```c
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
```
```bash
mpicc -g -Wall -o questio_4 main.c;mpiexec -n 4 ./questio_4
# saída
Proc 0 of 4 > Does anyone have a toothpick?
Proc 1 of 4 > Does anyone have a toothpick?
Proc 2 of 4 > Does anyone have a toothpick?
Proc 3 of 4 > Does anyone have a toothpick?
```


