24. If you haven’t already done so in Chapter 1, try to write pseudo-code for our tree-structured global sum, which sums the elements of loc_bin_cts. First consider how this might be done in a shared-memory setting. Then consider how this might be done in a distributed-memory setting. In the shared-memory setting, which variables are shared and which are private ?

Um código em C++, mas sem o uso de estruturas de dados de programação paralela, pode ser observado na resposta da questão 5 [Question_5.cpp](../../chapter_1/question_5/question_5.cpp). Também foi implementado o **mesmo** algoritmo em rust e nessa implementação foi utilizado threads, smart pointers e mutex para
resolver o problema. O código pode ser observado aqui: [main.rs](src/main.rs)

## Observação
caso tenha o rust instalado você pode observar a execução do caso de teste
com o comando **cargo**.

```shell
cargo test
```

O teste é mesmo teste feito em c++.