/* Autor: Elias P. Duarte Jr.
   Data da Ãšltima ModificaÃ§Ã£o: 11/abril/2024
   DescriÃ§Ã£o: Nosso primeiro programa de simulaÃ§Ã£o da disciplina Sistemas DistribuÃ­dos.
     Vamos simular N processos, cada um conta o â€œtempoâ€ independentemente
    Um exemplo simples e significativo para captar o â€œespÃ­ritoâ€ da simulaÃ§Ã£o */

#include <stdio.h>
#include <stdlib.h>
#include "smpl.h"

// Vamos definir os EVENTOS 
#define test 1
#define fault 2
#define recovery 3

// Vamos definir o descritor do processo

typedef struct{ 
  int id;            // identificador de facility do SMPL
  int *state;
  // outras variaveis locais dos processos sao declaradas aqui!
} TipoProcesso;

TipoProcesso *processo;

int main (int argc, char *argv[]) {
   
  static int N,   // nºmero de processos
            token,  // indica o processo que esta executando
            event, r, i, k,
            MaxTempoSimulac = 150;

  int seguinte = 0;
  int flag;

  static char fa_name[5];

  if (argc != 2) {
    puts("Uso correto: tempo <numero de processos>");
    exit(1);
  }

  N = atoi(argv[1]);

  smpl(0, "Um Exemplo de Simulacao");
  reset();
  stream(1);

  // inicializar processos

  processo = (TipoProcesso *) malloc(sizeof(TipoProcesso)*N);

  for (i=0; i<N; i++) {
    memset(fa_name, '\0', 5);
    sprintf(fa_name, "%d", i);
    processo[i].id = facility(fa_name,1);
    processo[i].state = (int*) malloc(N*sizeof(int)); // aloca espaco para o vetor state de cada processo
    for (k = 0; k < N; k++)  // inizializando para todos os demais processos como "unknown" == -1
      processo[i].state[k] = -1;
    processo[i].state[i] = 0; // inizializa em 0 para o proprio processo
  }
  // vamos fazer o escalonamento inicial de eventos

  // nossos processos vÃ£o executar testes em intervalos de testes
  // o intervalo de testes vai ser de 30 unidades de tempo
  // a simulaÃ§Ã£o comeÃ§a no tempo 0 (zero) e vamos escalonar o primeiro teste de todos os    
  //       processos para o tempo 30.0

  for (i=0; i<N; i++) {
    schedule(test, 30.0, i); 
  }
  schedule(fault, 31.0, 1);
  schedule(recovery, 61.0, 1);

  // agora vem o loop principal do simulador

  puts("===============================================================");
  puts("           Sistemas DistribuÃ­dos Prof. Elias");
  puts("          LOG do Trabalho PrÃ¡tico 0, Tarefa 1");
  puts("      Digitar, compilar e executar o programa tempo.c");
  printf("   Este programa foi executado para: N=%d processos.\n", N); 
  printf("           Tempo Total de SimulaÃ§Ã£o = %d\n", MaxTempoSimulac);
  puts("===============================================================");

  while(time() < 150.0) {
    cause(&event, &token);
      switch(event) {
        case test: 
          if (status(processo[token].id) !=0) break; // se o processo estÃ¡ falho, nÃ£o testa!

          flag = 1;
          seguinte = (token + 1) % N;
     
          while ((seguinte != token) && flag) {
            if (status(processo[seguinte].id) == 0) {
              printf("O processo %d testou o processo %d correto no tempo %4.1f\n", token, seguinte, time());
              processo[token].state[seguinte] = 0;
              flag = 0;

              for (k = (seguinte+1) % N; k != token; k = (k+1) % N)
                processo[token].state[k] = processo[seguinte].state[k];
            } else {
              printf("O processo %d testou o processo %d falho no tempo %4.1f\n", token, seguinte, time());
              processo[token].state[seguinte] = 1;
            }
            seguinte = (seguinte + 1) % N;
          }

          printf("Vetor state do processo %d: [", token);
          for (int k = 0; k < N; k++) {
            printf (" %d", processo[token].state[k]);
          }
          printf (" ]");
          puts ("");
          
          schedule(test, 30.0, token);
          break;
        case fault:
          r = request(processo[token].id, token, 0);
          printf("Socooorro!!! Sou o processo %d  e estou falhando no tempo %4.1f\n", token, time());
          break;
        case recovery:
          release(processo[token].id, token);
          printf("Viva!!! Sou o processo %d e acabo de recuperar no tempo %4.1f\n", token, time());
          schedule(test, 1.0, token);
          break;
      } // switch
  } // while
} // tempo.c