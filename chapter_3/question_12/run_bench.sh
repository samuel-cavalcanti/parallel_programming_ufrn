#!/bin/bash




A="-n 4 ./bench 12 1,2,3,4,5,6,7,8,9,0,1,2"
B="-n 4 ./bench 8 1,2,3,4,5,6,7,8"
C="-n 4 ./bench 4 1,2,3,4"
# D="-n 3 ./bench 9 1,2,3,4,5,6,7,8,9"
# E="-n 3 ./bench 6 1,2,3,4,5,6"
# F="-n 3 ./bench 3 1,2,3" 
G="-n 2 ./bench 8 1,2,3,4,5,6,7,8"
H="-n 2 ./bench 4 1,2,3,4"
I="-n 2 ./bench 2 1,2"
J="-n 1 ./bench 8 1,2,3,4,5,6,7,8"
K="-n 1 ./bench 4 1,2,3,4"
L="-n 1 ./bench 2 1,2"

inputs_mpirun=("$A" "$B" "$C" "$G" "$H" "$I" "$J" "$K" "$L")


echo "algorithm time process" > data.dat
for input_mpi_run in "$A" "$B" "$C" "$G" "$H" "$I" "$J" "$K" "$L";
do
    mpicc  bench.c -o bench
    mpirun $input_mpi_run >> data.dat 
done

# creating graphs
./plot_data.r


# cleaning
rm bench
rm Rplots.pdf