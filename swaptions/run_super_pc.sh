#!/bin/bash 
#SBATCH --job-name=swaptions 
#SBATCH --time=2-0:0
#SBATCH --cpus-per-task=32
#SBATCH --hint=compute_bound
#SBATCH --exclusive
export OMP_NUM_THREADS=32


cd pascal-releases
source env.sh
cd ..


NTHREADS="__nt__" 
# tenho que passar esse parâmetro  __nt__
# para os cores.

# simsmall
simsmall="-ns 16 -sm 10000 -nt ${NTHREADS}"

# simmedium
simmedium="-ns 32 -sm 20000 -nt ${NTHREADS}"

#simlarge
simlarge="-ns 64 -sm 40000 -nt ${NTHREADS}"

#native
native="-ns 128 -sm 1000000 -nt ${NTHREADS}"


# -ns == nSwaptions número de simulações
# -sm == NUM_TRIALS

#número de tentativas por simulaçãoimage.png
# -nt == nThreads  número de Threads

# tenho 8 threads, portanto
MY_CORES=32;# colocar 32 caso usando o super computador

# -t man é para informar que estou utilizando pascalops.h para isolar a região paralelizada
pascalanalyzer -t man -c 1:$MY_CORES --ipts "$simsmall,$simmedium,$simlarge,$native" "./build/swaptions-pthreads" -o "swaptions-pthreads-custom.json"
pascalanalyzer -t man -c 1:$MY_CORES --ipts "$simsmall,$simmedium,$simlarge,$native" "./build/swaptions-openmp" -o "swaptions-openmp-custom.json"