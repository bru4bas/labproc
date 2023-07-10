Exemplo de um mini sistema executivo cooperativo.
Nos arquivos:
* kernel.s -- vetor de interrupções, ponto de entrada do kernel (interrupção de software), despachador (context switcher);
* sched.c -- escalador, funções para chamar o kernel (getpid, yield);
* main.c -- ponto(s) de entrada para dois processos (threads).
