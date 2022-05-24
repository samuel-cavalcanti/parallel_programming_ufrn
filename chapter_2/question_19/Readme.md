19. Suppose $T_{serial} = n$  and $T_{parallel} = \frac{n}{p} + log_2 (p)$, where times are in microseconds. If we increase $p$ by a factor of $k$, find a formula for how much we’ll need to increase n in order to maintain constant efficiency. How much should we increase $n$ by if we double the number of processes from 8 to 16 ? Is the parallel program scalable ?

##  Encontre uma formula para o quanto nós teremos que aumentar $n$ para obter uma eficiência constante.

$E(p) = \frac{T_{serial}}{pT_{parallel}}$

$E(p) = \frac{n}{p(\frac{n}{p} + log_2(p))}$ 

$E(p) = \frac{n}{n + plog_2(p)}$

$E(kp) = \frac{n}{n + kplog_2(kp)}$

Se considerarmos a constante $A$ o número de vezes que temos que aumentar $n$ para
obter uma eficiência constante, logo:

$E_{a} (kp) = \frac{An}{An + kplog_2(kp)}$

$E_{a}(kp) = E(p)$

$\frac{An}{An + kplog_2(kp)} =  \frac{n}{n + plog_2(p)}$

$An =\frac{n(nA + kplog_2(kp))}{n + plog_2(p)}$

$A = \frac{nA + kplog_2(kp)}{n + plog_2(p)}$

$A =  \frac{nA}{n + plog_2(p)} +  \frac{ kplog_2(kp)}{n + plog_2(p)}$

$A - \frac{nA}{n + plog_2(p)}= \frac{ kplog_2(kp)}{n + plog_2(p)}$

$A[ 1 - \frac{n}{n + plog_2(p)}]= \frac{ kplog_2(kp)}{n + plog_2(p)}$

$A[\frac{n + plog_2(p)}{n + plog_2(p)} - \frac{n}{n + plog_2(p)}]= \frac{ kplog_2(kp)}{n + plog_2(p)}$

$A\frac{plog_2(p)}{n + plog_2(p)} = \frac{ kplog_2(kp)}{n + plog_2(p)}$

$Aplog_2(p) = kplog_2(kp)$

$A = \frac{kplog_2(kp)}{plog_2(p)}$

$A = \frac{klog_2(kp)}{log_2(p)}$

## Quantas vezes nós devemos aumentar $n$ se nós dobrarmos o número de cores de 8 para 16

$A(k,p) =\frac{klog_2(kp)}{log_2(p)}$
se $k = 2$ e $p=8$ então:

$A = \frac{2log_{2}(16)}{log_{2}(8)}$

$A = \frac{2(4)}{3}$

$A = \frac{8}{3}$


## O programa paralelo é escalável ?

Dado a definição do autor sim. Para o autor escalável é quando a eficiência de um programa paralelo se mantém constante, ou seja, se existe uma taxa que relaciona o
crescimento do tamanho do problema com o crescimento do número de threads/processos, então o programa paralelo escala.