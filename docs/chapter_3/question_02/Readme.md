# Questão 2

Modify the trapezoidal rule so that it will correctly estimate the integral even if _comm_sz_ doesn’t evenly divide n. (You can still assume that n $\geq$ _comm_sz_.).

## Código antes da alteração

```c
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


double trap(double a, double b, long int n);

double function(double x);

int main(int argc, char *argv[])
{

  int my_rank;
  int comm_sz;

  int message_tag = 0;
  // resceber argumentos globais via linha de commando
  MPI_Init(&argc, &argv);

  const double A = atof(argv[1]);
  const double B = atof(argv[2]);
  const int N = atoi(argv[3]);

  

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  double h = (B - A) / N;

  int local_n = N / comm_sz;
  /*
    Como podemos observar que quando a divisão
    inteira N/comm_sz o n local, não corresponde
    ao número total de trapézios desejados.
    Ex: 1024/3 ==  341, sendo que 341*3 == 1023
   */
  double local_a = A + my_rank * local_n * h´;
  double local_b = local_a + local_n * h;
  double result_integral = trap(local_a, local_b, local_n);

  if (my_rank != 0)
  {
    MPI_Send(&result_integral, 1, MPI_DOUBLE, 0, message_tag, MPI_COMM_WORLD);
  }
  else // my_rank ==0
  {

    for (int source = 1; source < comm_sz; source++)
    {
      double result_integral_source;
      MPI_Recv(&result_integral_source, 1, MPI_DOUBLE, source, message_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      result_integral += result_integral_source;
    }
    printf("with n = %d trapezoids, our estimate\n", N);
    printf("of the integral from %f to %f = %.15e\n", A, B, result_integral);
  }

  MPI_Finalize();

  return 0;
}

double trap(double a, double b, long int n)
{

  double h = (b - a) / n;

  double approx = (function(a) + function(b)) / 2.0;
  for (int i = 1; i < n - 1; i++)
  {
    // printf("f_%i \n", i);
    double x_i = a + i * h;

    approx += function(x_i);
  }

  return h * approx;
}

double function(double x) { return x * x; }
```
compilação e execução
```bash
mpicc trap_rule.c -o trap
# a =0, b =3, n = 1024
mpiexec -n 4 ./trap 0 3 1024
```


## Alteração no código

Sabendo que pode ocorrer divisões cujo o número não pertence aos números racionais, por tanto se faz necessário utilizar a seguinte
expressão:
$$
 \frac{N}{D} = a \times D   +\text{resto}
$$
exemplo:
$$
    \frac{1024}{3} = 341 \times 3 + 1
$$
Podemos resolver o problema simplesmente despejando os trapézios restantes em um processo ou podemos dividir a "carga" entre os processos. Dividir a carga entre os processos de forma mais igualitária possível, foi resolvido no [exercício 1 capítulo 1](../../chapter_1/question_1/Readme.md).

```c++
struct range
{
    int first;
    int last;
};

struct range new_range_2(int thread_index, int p, int n)
{
    struct range r;

    int division = n / p;
    int rest = n % p;

    if (thread_index < rest)
    {
        r.first = thread_index * (division + 1);
        r.last = r.first + division + 1;
    }
    else
    {
        r.first = thread_index * division + rest;
        r.last = r.first + division;
    }

    return r;
}
...

int main(int argc, char *argv[])
{

    ...
    /*
    onde thread_index é o rank,
    o número de cores é o número de processos,
    o tamanho do vetor é o número de trapézios
    */
    struct range r = new_range_2(my_rank,comm_sz,N);
  
    double h = (B - A) / N;

    /*
    perceba que o número local de trapézios é
    o tamanho do intervalo calculado  
     */
    int local_n = r.last -r.first;
    double local_a = A + r.first * h;
    double local_b = A + r.last * h;
    double result_integral = trap(local_a, local_b, local_n);

    printf("local n: %i local a: %f local b %f \n", local_n, local_a, local_b);
}
```
## Simulações feitas offline:

```bash
mpicc trap_rule.c -o trap; mpiexec -n 3 ./trap 1 3 1024
local n: 342 local a: 1.000000 local b 1.667969 
local n: 341 local a: 1.667969 local b 2.333984 
local n: 341 local a: 2.333984 local b 3.000000 
with n = 1024 trapezoids, our estimate
of the integral from 1.000000 to 3.000000 = 8.633069768548012e+00
```

```bash
mpicc trap_rule.c -o trap; mpiexec -n 4 ./trap 1 3 2024
local n: 506 local a: 1.000000 local b 1.500000 
local n: 506 local a: 1.500000 local b 2.000000 
local n: 506 local a: 2.000000 local b 2.500000 
local n: 506 local a: 2.500000 local b 3.000000 
with n = 2024 trapezoids, our estimate
of the integral from 1.000000 to 3.000000 = 8.645439504647232e+00
```

```bash
mpicc trap_rule.c -o trap; mpiexec -n 3 ./trap 0 3 1024
local n: 342 local a: 0.000000 local b 1.001953 
local n: 341 local a: 1.001953 local b 2.000977 
local n: 341 local a: 2.000977 local b 3.000000 
with n = 1024 trapezoids, our estimate
of the integral from 0.000000 to 3.000000 = 8.959068736061454e+00
```

## código final

```c
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

struct range
{
  int first;
  int last;
};

struct range new_range_2(int thread_index, int p, int n)
{
  struct range r;

  int division = n / p;
  int rest = n % p;

  if (thread_index < rest)
  {
    r.first = thread_index * (division + 1);
    r.last = r.first + division + 1;
  }
  else
  {
    r.first = thread_index * division + rest;
    r.last = r.first + division;
  }

  return r;
}
``
double trap(double a, double b, long int n);

double function(double x);

int main(int argc, char *argv[])
{

  int my_rank;
  int comm_sz;

  int message_tag = 0;

  MPI_Init(&argc, &argv);

  const double A = atof(argv[1]);
  const double B = atof(argv[2]);
  const int N = atoi(argv[3]);

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  struct range r = new_range_2(my_rank, comm_sz, N);

  double h = (B - A) / N;

  int local_n = r.last - r.first;
  double local_a = A + r.first * h;
  double local_b = A + r.last * h;
  double result_integral = trap(local_a, local_b, local_n);

  printf("local n: %i local a: %f local b %f \n", local_n, local_a, local_b);

  if (my_rank != 0)
  {
    MPI_Send(&result_integral, 1, MPI_DOUBLE, 0, message_tag, MPI_COMM_WORLD);
  }
  else // my_rank ==0
  {

    for (int source = 1; source < comm_sz; source++)
    {
      double result_integral_source;
      MPI_Recv(&result_integral_source, 1, MPI_DOUBLE, source, message_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      result_integral += result_integral_source;
    }
    printf("with n = %d trapezoids, our estimate\n", N);
    printf("of the integral from %f to %f = %.15e\n", A, B, result_integral);
  }

  MPI_Finalize();

  return 0;
}

double trap(double a, double b, long int n)
{

  double h = (b - a) / n;
  // printf("H: %f\n",h);

  double approx = (function(a) + function(b)) / 2.0;
  for (int i = 1; i < n - 1; i++)
  {
    // printf("f_%i \n", i);
    double x_i = a + i * h;

    approx += function(x_i);
  }

  return h * approx;
}

double function(double x) { return x * x; }
```