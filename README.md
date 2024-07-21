# vCube

## Introdução
Este trabalho tem como objetivo implementar o detector de falhas vCube (versão 2) no ambiente de simulação SMPL, e avaliar o seu desempenho considerando diversos valores de N (número de processos) e diferentes eventos de mudança de estado dos processos.

## Desenvolvimento
Cada processo no sistema manterá um vetor `STATE[0..N-1]` de contadores de eventos, inicializado em -1 para representar o estado `UNKNOWN`. Inicialmente, o processo em si tem sua entrada no vetor `STATE[]` setada para zero. O vCube divide os processos em clusters, e cada processo testador executa testes em todos os clusters a que pertence em cada intervalo de teste. Quando um processo testador descobre um novo evento em um processo testado, ele incrementa a entrada correspondente no seu vetor `STATE[]`. Para a transferência de informações de estado, o testador lê o vetor `STATE[]` do processo testado e compara as entradas correspondentes para detectar novidades. Se o valor da entrada for maior no vetor `STATE[]` do processo testado, o testador copia essa informação.

## Como executar

```bash
cd src
```

Para compilar o programa:

```bash
make
```

Para executar o programa:

```bash
./vcube < numero de processos >
```

## Testes
Os testes foram realizados em três cenários distintos, cujos resultados estão detalhados no diretório `test`.
1. Testa com 8 processos.
2. Testa com 16 processos.
3. Testa novamente com 8 processos, onde os processos 3 e 5 eram falhos.