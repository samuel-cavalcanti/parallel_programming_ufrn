# Swaptions

Refatoração no código do swaptions, mas apenas as versões **Pthreads** e **OpenMP** são suportadas


## How to build

dependencies: 
- pthreads
- openmp
- cmake VERSION 3.10


```bash
mkdir build # creating a directory 
cd build
cmake .. # make sure you have cmake installed
make -j8
```

## testing Swaptions

```bash
chhmod +x run_tests.sh 
./run_tests.sh
```


