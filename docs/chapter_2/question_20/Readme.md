20. Is a program that obtains linear speedup **strongly** scalable ? Explain your answer.

Dado a definição do autor sim. Para o autor escalável é quando a eficiência de um programa paralelo se mantém constante. Linear speedup
pode ser escrito pela seguinte expressão:

$S = \frac{T_{serial}}{T_{parallel}} = p$, onde $p$ é número de cores e $S$ é o speedup.

Portanto dado que eficiência é dado pela seguinte expressão:

$E = \frac{T_{serial}}{pT_{parallel}}$, onde $T_{serial}$ é o tempo da aplicação em serial e $T_{parallel}$ o tempo da aplicação em paralelizada.

se o speedup for linear, ou seja, $S=p$, temos que

$E = \frac{S}{p}$, portanto

$E = \frac{p}{p} = 1$, Como a eficiência é constante, logo, por definição a aplicação é fortemente escalável (**_strongly scalable_**).