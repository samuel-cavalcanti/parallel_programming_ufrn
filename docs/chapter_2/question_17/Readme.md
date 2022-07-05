# Questão 17

A parallel program that obtains a speedup greater than p—the number of processes or threads—is sometimes said to have **superlinear speedup**. However, many authors don’t count programs that overcome “resource limitations” as having superlinear speedup. For example, a program that must use secondary storage for its data when it’s run on a single processor system might be able to fit all its data into main memory when run on a large distributed-memory system. Give another example of how a program might overcome a resource limitation and obtain speedups greater than p

## Multiplicação Matricial

Como demonstrado por george hotz em **can you multiply a matrix? (noob lesson)** quando otimizado o algoritmo de Multiplicação de matrizes de forma que busca extrair ao máximo o uso da cache,
se obtém mais que o dobro de de Gflops. No entanto também é observado Thermal Throttling que reduz a frequência do processador, impossibilitado o superlinear speedup com todos os cores. O que aconteceu, foi que toda a matriz é capaz
de ser carregada na cache compartilhada, mas não é capaz de ser
totalmente carregada na cache exclusiva do core, então quando
se dividiu a matriz entre os cores a performance mais que dobra
pois ela se soma com a vantagem da cache.

vídeo da implementação https://www.youtube.com/watch?v=VgSQ1GOC86s.