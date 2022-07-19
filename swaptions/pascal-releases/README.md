# Pascal Analyzer
[![pipeline status](https://gitlab.com/lappsufrn/pascal_analyzer/badges/development/pipeline.svg)](https://gitlab.com/lappsufrn/pascal_analyzer/commits/development) [![coverage report](https://gitlab.com/lappsufrn/pascal_analyzer/badges/development/coverage.svg)](https://gitlab.com/lappsufrn/pascal_analyzer/commits/development)

Python library to control an application execution triggers and processing the
output measures times data to calculate speedups.


## Table of contents

- [What is Pascal Analyzer](#description)
- [Installation](#installation)
- [Usage](#usage)
- [Options available](#options-available)
- [Build Binary](#build-binary-package)


## Description

  **Pascal Analyzer**  profiles an application for measuring and comparing executions with different
  alternative configurations with the objective of understanding the scalability capacity of  a
  parallel  application by  observing  how it  makes use of the available computational resources
  across different configurations. In addition, we show that the tool has a low level of intrusion
  in the programs under analysis, which is a fundamental aspect in understanding their behavior
  and scalability capacity accurately.

### Features
  - Runs applications with multiple frequencies, number of cores, input sizes
  and, optionally, repet the execution to better find outs.
  - Manipulate of resulting data from logs process or online execution, obtained
  by module run script itself.
  - Calculate the speedups and efficency of applications, if it's possible,
   using the measured times of execution.

## Installation
```bash
git clone https://gitlab.com/lappsufrn/pascalsuite-analyzer.git
cd pascalsuite-analyzer/
./build.sh
```

### Prerequisites
  - libpfm4
  - g++
  - swig
  - make
  - Python3 or newer
    - cpufreq
    - netipmi
    - psutil
    - numpy
    - pandas
    - scipy
    - sklearn
    - ghalton
    - performance-features

## Usage:

### From command line
  ```bash
  pascalanalyzer ./myapp --inst aut --idtm 5 --cores 1,4  --frqs 3000000,2800000 --verb 3
  pascalanalyzer ./myapp -c 1,32 -v 2 -o myoutput.json
  ```

### Data analysis library

  Class used to generate the measured times structure, to save such data in a
  "json" file, to load a previously saved json data file, to calculate the
  speedup or efficiency of application and to plot 2D or 3D graph of time,
  speedup or efficiency versus the number of cores and frequency or input size.
  ```python
  from pascalanalyzer import PascalData
  d = PascalData("path_to_datafile")
  print(d)        # Print summary informations
  d.times()       # Show a Dataframe with mesures times
  d.speedups()    # Show a Dataframe with speedups
  d.energy()      # Show a Dataframe with energy
  ```
### Instrumenting source code

Manual instrumentation is avialable using the functions pascal_start(id) and pascal_stop(id) defined at `include/pascalops.h`.

- `pascal_start`: receive 1 arguments, the identification of the region.
It marks the beginning of the parallel area to be measured.
- `pascal_stop`: receive 1 arguments, the identification of the region.
It marks the end of the parallel area to be measured.

Example:

```C++
#include "pascalops.h"
...
int main()
{
	pascal_start(1);
	#pragma omp parallel
	{
		...
	}
	pascal_stop(1);
	...
}
```


A option to automatically instrument openmp code is also available using the command line:
`./pascalanalyzer --imnt teste/ .cpp,.c`, this will mark all `pragma omp parallel` regions on the cpp and c files on the folder teste.

## Options available

  Command line tool to run application collecting information while executing
  program with different configurations of cores, frequencies, inputs arguments.

    usage: pascalanalyzer [-h] [-c CORES] [-f FREQUENCIES] [-i INPUTS] [-o OUTPUT] [-r RPTS] [-g]
                      [-t {aut,AUT,man,MAN}] [-a LEVEL] [--mpi RUNTIME] [--ragt TYPE] [-v VERBOSE]
                      [--dcrs] [--dhpt] [--domp] [--dout] [--govr GOVERNOR] [--idtm TIME]
                      [--fgpe EVENT] [--fgps EVENT] [--rple {sysfs,perf}]
                      [--rpls {sysfs,scontrol,perf}] [--ipmi SERVER USER PASSWORD] [--modl NPTS MODE]
                      [--prcs] [--lpcs] [--imnt PATH EXTENSIONS]
                      [application]

    Script to run application collecting information while executinga program with different
    configurations of cores, frequencies, inputs arguments.

    positional arguments:
      application           Application Name to run

    optional arguments:
      -h, --help            show this help message and exit
      -c CORES, --cors CORES
                            List of cores numbers to be used. Ex: 1,2,4
      -f FREQUENCIES, --frqs FREQUENCIES
                            List of frequencies (KHz). Ex: 2000000,2100000
      -i INPUTS, --ipts INPUTS
                            Input name to be used on run. (Default: None). Syntax:
                            inputsetname[<initialnumber>:<finalnumber>]. From lowest to highest size.
      -o OUTPUT, --outp OUTPUT
                            Output file name
      -r RPTS, --rpts RPTS  Number of repetitions for a specific run. (Default: 1)
      -g, --gapr            Include as regions the gaps among regions already identified
      -t {aut,AUT,man,MAN}, --inst {aut,AUT,man,MAN}
                            Instrumentation type to identify code regions (auto or manual). When not used
                            the code regions will not identified.
      -a LEVEL, --ragl LEVEL
                            Set the regions hierarchy level to aggregate measures. 0 = No level.
      --mpi RUNTIME         Enable mpi and set the runtimer
      --ragt TYPE           Region aggregation type. The 'acc' type means to use a accumulated time
                            variable that reduces memory consume and calculation time.
      -v VERBOSE, --verb VERBOSE
                            verbosity level. 0 = No verbose
      --dcrs                Disable cores
      --dhpt                Enable hyperthread (disabled by default)
      --domp                Set OMP_NUM_THREADS variable with -c parameter values. (enabled by default)
      --dout                Discard the output
      --govr GOVERNOR       Set the cpu governor
      --idtm TIME           Idle time between runs. (Default: 0)
      --fgpe EVENT          Collect performance counters
      --fgps EVENT          Sample performance counters
      --rple {sysfs,perf}   Enable rapl energy measuments
      --rpls {sysfs,scontrol,perf}
                            Enable rapl sample energy measuments
      --ipmi SERVER USER PASSWORD
                            Enable ipmi measuments
      --modl NPTS MODE      Run pascal with random configurations to create a model
      --prcs                Track cpu cores
      --lpcs                List performance counters
      --imnt PATH EXTENSIONS
                            Instrument code


## Build binary package
```bash
git clone git@gitlab.com:lappsufrn/pascalsuite-analyzer.git
cd pascalsuite-analyzer/
git fetch origin development
./deploy
```

## Site

- <https://lappsufrn.gitlab.io/pascalsuite-analyzer/>

### License

The entire suite is licensed under the GPL v3.0.
