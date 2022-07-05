# Lista de Execícios de Programação Concorrente e distribuída

- [x] [Capítulo 1](chapter_1/Readme.md) (7 questões)
- [x] [Capítulo 2](chapter_2/Readme.md) (13 questões)
- [ ] [Capítulo 3](chapter_3/Readme.md) (16 questões)
- [ ] Capítulo 5  (12 questões)


Capítulo 3 foram realizadas as questões: 2, 4, 6, 9, 11, 12, 13, 16, 17, 19

<!-- ## Questão 16
Quando o compilador não é capaz de vetorizar automaticamente, ou vetoriza de forma ineficiente, o OpenMP provê a diretiva** omp simd**, com a qual o programador pode indicar um laço explicitamente para o compilador vetorizar. No código abaixo, a inclusão da cláusula reduction funciona de forma similar a flag **-ffast-math**, indicando que a redução na variável soma é segura e deve ser feita.
```c++
#pragma omp simd reduction(+:soma)
for(i=0;i<n;i++){
   x = (i+0.5)*h;
   soma += 4.0/(1.0+x*x);
}
```
Por que não é necessário usar a cláusula **private(x)** neste caso mas seria caso a diretiva **omp simd** fosse combinada com a diretiva **omp parallel for** ? -->
