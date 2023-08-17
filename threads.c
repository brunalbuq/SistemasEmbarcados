#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
// criando um tipo para salvar as informações das tarefas 
typedef struct
{
  char name;
  int periodo;
  int deadline;
  int tempo_exe;
}
infothread;

void *
funcao_tarefa (void *arg)
{
 // criando variáveis para salvar o tempo de inicio em 'start_time', o tempo atual em 'current_time' e o tempo da próxima ativação
  struct timespec start_time, current_time, proxima_ativacao;
  while (1)
    {
    // salvando em start_time o tempo de inicio da execução
      clock_gettime (CLOCK_MONOTONIC, &start_time);
      // salvando localmente as informações da thread
      infothread *info = (infothread *) arg;
      // printando na tela que a tarefa inicializou sua execução
      printf ("A tarefa %c comeC'ou sua execuC'C#o em %ld.%ld \n", info->name,start_time.tv_sec,
	      start_time.tv_nsec);
	  // printando na tela que a tarefa está em execução
      printf ("A tarefa %c estC! em execuC'C#o\n", info->name);
    // printando na tela que a tarefa finalizou sua execução
      clock_gettime (CLOCK_MONOTONIC, &current_time);
      // simulando um tempo de execução com o comando que solicita que a tarefa durma 
      usleep (info->tempo_exe * 1000000);	// simulando o tempo de execuC'C#o em microsegundos
    // printando na tela que a tarefa finalizou sua execução
      printf ("A tarefa %c finalizou sua execuC'C#o em  %ld.%ld \n", info->name,
	      current_time.tv_sec, current_time.tv_nsec);
    // proxima ativação em segundos
      proxima_ativacao.tv_sec = start_time.tv_sec + info->periodo;
      // proxima ativaC'C#o em nanosegundos
      proxima_ativacao.tv_nsec = start_time.tv_nsec + info->periodo % 1000000000;
      if (proxima_ativacao.tv_nsec >= 1000000000)
      {
          proxima_ativacao.tv_sec += 1;
          proxima_ativacao.tv_nsec -=1000000000;
      }
      
      while (current_time.tv_sec < proxima_ativacao.tv_sec || 
      (current_time.tv_sec == proxima_ativacao.tv_sec && 
      current_time.tv_nsec< proxima_ativacao.tv_nsec))
      {
        clock_gettime(CLOCK_MONOTONIC, &current_time);
      }
    }
}

int
main ()
{
    // criando variáveis para salvar as informações das threads
  infothread T1_info = { '1', 5, 2, 1 };
  infothread T2_info = { '2', 6, 6, 2 };
  infothread T3_info = { '3', 14, 14, 4 };

    // criando as variáveis tarefa_1, tarefa_2 e tarefa_3  com o tipo pthread
  pthread_t tarefa_1, tarefa_2, tarefa_3;
  // criando as tarefas (todas executam o mesmo código)
  pthread_create (&tarefa_1, NULL, funcao_tarefa, (void *) &T1_info);
  pthread_create (&tarefa_2, NULL, funcao_tarefa, (void *) &T2_info);
  pthread_create (&tarefa_3, NULL, funcao_tarefa, (void *) &T3_info);
    // obriga a inicialização das tarefas 
  pthread_join (tarefa_1, NULL);
  pthread_join (tarefa_2, NULL);
  pthread_join (tarefa_3, NULL);

}
