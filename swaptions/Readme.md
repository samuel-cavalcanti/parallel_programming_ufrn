# Swaptions


## How to build

dependências: 
- pthreads
- cmake


```bash
mkdir build # creating a directory 
cd build
cmake .. # make sure you have cmake installed
make -j8
```

## testing Swaptions

```bash

cd build
./swaptions -ns 16 -sm 10000 -nt 4 
# expected output:
PARSEC Benchmark Suite
Number of Simulations: 10000,  Number of threads: 4 Number of swaptions: 16
Pthreads is enabled !!
Swaption 0: [SwaptionPrice: 6.9321719668 StdError: 0.0012757149] 
Swaption 1: [SwaptionPrice: 3.2391855907 StdError: 0.0008911209] 
Swaption 2: [SwaptionPrice: 0.8546285496 StdError: 0.0003314301] 
Swaption 3: [SwaptionPrice: 6.4801759395 StdError: 0.0016923922] 
Swaption 4: [SwaptionPrice: 79.7804902254 StdError: 0.0194846446] 
Swaption 5: [SwaptionPrice: 6.8645234215 StdError: 0.0023350763] 
Swaption 6: [SwaptionPrice: 81.3002423976 StdError: 0.0166488659] 
Swaption 7: [SwaptionPrice: 3.0594089064 StdError: 0.0006198656] 
Swaption 8: [SwaptionPrice: 112.3290190813 StdError: 0.0197234194] 
Swaption 9: [SwaptionPrice: 3.0611458995 StdError: 0.0006210262] 
Swaption 10: [SwaptionPrice: 12.0706190693 StdError: 0.0029064870] 
Swaption 11: [SwaptionPrice: 24.0315183137 StdError: 0.0043411367] 
Swaption 12: [SwaptionPrice: 96.6873061511 StdError: 0.0247704953] 
Swaption 13: [SwaptionPrice: 1.1507825687 StdError: 0.0003896316] 
Swaption 14: [SwaptionPrice: 9.2844397515 StdError: 0.0021512490] 
Swaption 15: [SwaptionPrice: 9.6495866271 StdError: 0.0020492623] 
```


