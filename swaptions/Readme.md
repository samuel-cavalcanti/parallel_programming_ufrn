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


## Modificações para utilizar o pascalops.h

### CMakeLists.txt

Para adicionar uma biblioteca dinâmica utilizado CMakeLists.
É necessário dois passos, o primeiro é localizar a biblioteca
e segundo linka-la, ou seja, connecta-la ao arquivo executável.

#### Encontrando a biblioteca

Primeiro eu clonei o projeto pascal releases nesse repositório
o comando git

```shell
git clone https://gitlab.com/lappsufrn/pascal-releases.git
```

desde modo o repositório pode ser encontrado na minha pasta [pascal-releases](pascal-releases). Após baixar o analyzer, eu adicionei o commando **find_library**  no meu  CMakeLists.txt

```cmake
# find the pascal library   
find_library(PASCAL manualinst PATHS "pascal-releases/lib/" REQUIRED)
# exibe uma messagem no terminal dizendo que encontrou o PASCAL
message(STATUS "Found Pascal: ${PASCAL}")
```

neste comando recebe como parâmetro 4 argumentos principais
- PASCAL
- manualinst
- PATHS "pascal-releases/lib/"
- REQUIRED
fazendo uma analogia a uma função escrita em c, seria algo assim:

```c
// assinatura da função
dylib find_library(const char* libName,const char* paths[],int required);

// chamada da função
char* dirs[MAX_DIRS];
dirs[0] = "pascal-releases/lib/";
int REQUIRED =1;
PASCAL = find_library("manualinst",dirs,REQUIRED);
```

*PASCAL* é a variável onde será armazenado o retorno da função.

manualinst é nome da biblioteca. 

REQUIRED é um argumento opcional dizendo que essa biblioteca é requerida,
ou seja, caso ela não seja encontrada a aplicação não deve compilar.

PATHS "pascal-releases/lib/", é uma lista de possíveis caminhos onde
pode ser encontrado a biblioteca o arquivo libmanualinst.so.

#### Linkando a lib nos executáveis

neste projeto eu tenho 4 executáveis:

- swaptions-pthreads
- swaptions-openmp
- swaptions-pthreads-test
- swaptions-openmp-test

os executáveis com o final *-test*. implica que são os executáveis
dos testes automatizados. Como os testes automatizados não é o objetivo
vamos ignora-los assim como os executáveis -openmp.

Para linkar a lib no binário executável basta chamar a função **target_link_libraries**.

```cmake
#adding pascal
target_link_libraries(swaptions-pthreads PRIVATE ${PASCAL})
```

algo semelhante a essa função seria a flag de compilação.

```shell
gcc main.cpp -lmanualinst -o swaptions-pthreads
```

após a linkagem, você já pode utilizar as funções **pascal_start** e   **pascal_stop** no código.

No caso do swaptions, a única região paralela usando o Pthreads
pode ser encontrada no arquivo [pthread_runner.cpp](src/runners/pthread_runner.cpp)

#### Observações extras

ao executar o *pascalanalyzer* não se esqueça de passar a flag  **-t man**
para informar que existe uma região no código que foi manualmente identificada.

