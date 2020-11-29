// Autor: Thayse Marques Solis
// Data da última modificação: 29 de novembro de 2020
// Descrição resumida: Terceira tarefa de simulação da disciplina Sistemas Distribuídos.

#include <stdio.h>
#include <stdlib.h>
#include "smpl.h"

// Vamos definir os EVENTOS
#define test 1
#define fault 2
#define recovery 3

#define correto 0
#define falho 1
#define desconhecido -1

// vamos definir o descritor do processo

typedef struct {
	int id;		// identificador de facility (recurso) do SMPL
	int* State;
	// outras variáveis locais dos processos são declaradas aqui!
} TipoProcesso;

TipoProcesso* processo;

int main(int argc, char* argv[]) {
	static int N, // número de processos
		   token, // indica o processo que está sendo executado
		   event, r, i;
	static char fa_name[5];
	
	if(argc!=2) {
		puts("Uso correto: tempo3 <número de processos>");
		exit(1);
	}
	
	N = atoi(argv[1]);
	
	
    smpl(0,"Um Exemplo de Simulação"); // Inicia a simulação
	reset();
	stream(1); // Uma única thread de simulação
	
	// inicializar processos
	
	processo = (TipoProcesso*) malloc(sizeof(TipoProcesso)*N);
	
	for (i=0;i<N;i++) {
		processo[i].State = (int*) malloc(sizeof(int)*N); //aloca espaço para o estado de cada processo que é guardado no vetor State
		for (int j=0; j < N; j++)
		{
			processo[i].State[j]=desconhecido; // inicializa o estado de cada processo como "desconhecido" ou -1
		}
	}
	
	for (i=0;i<N;i++) {
		memset(fa_name, '\0', 5); // for (j=0;j<5;j++) fa_name[j] = '\0';
		sprintf(fa_name, "%d", i);
		processo[i].id = facility(fa_name, 1);
	}
	
	//vamos fazer o escalonamento inicial de eventos
	
	// nossos processos vão executar testes em intervalos de testes
	// o intervalo de teste vai ser de 30 unidades de tempo
	// a simulação começa no tempo 0 (zero) e vamos escalonar o primeiro teste de todos os processos 
	//	para o tempo 30.0
	
	for (i = 0; i < N;i++) { // para todos os processos, escalona test para daqui a 30.0 unidades de tempo (i eh o token)
		schedule (test, 30.0, i);
	}
	
	schedule(fault, 31.0, 1); // escalona uma falha no tempo 31 para o processo 1
	schedule(recovery, 61.0, 1); // escalona uma recuperação no tempo 61 para o processo 1
	
	//agora vem o loop principal do simulador
	
	while(time()<150.0) {
		cause(&event, &token);
		int num_processo_atual = token;
		int num_proximo_processo = (token+1)%N;
		switch(event) {
			case test:
				// Se o processo atual estiver falho, ele não vai testar o processo seguinte
				if (status(processo[num_processo_atual].id) != 0) { 
					break; 
				}
				
				// Agora o processo atual vai testar os processos seguintes até encontrar um correto.
				while (status(processo[num_proximo_processo].id) != 0) {
					printf("No tempo %4.1f, o processo %d testou o processo %d que está falho.\n", time(), num_processo_atual, num_proximo_processo);
					processo[num_processo_atual].State[num_proximo_processo] = falho;
					num_proximo_processo = (num_proximo_processo+1)%N;
				}
				printf("No tempo %4.1f, o processo %d testou o processo %d que está correto.\n", time(), num_processo_atual, num_proximo_processo);
				processo[num_processo_atual].State[num_proximo_processo] = correto;
				
				// Agora vamos agendar o próximo teste
				schedule(test, 30.0, token);
				
				//Mostra o vetor de estados (State) do processo atual
				printf("O vetor State do processo %d eh: ", num_processo_atual);
				for (i = 0; i < N; i++)
				{
					printf("%d ", processo[num_processo_atual].State[i]);
				}
				printf("\n");
				break;
			case fault:
				r = request(processo[token].id, token, 0); // request reserva a facility e faz status != 0
				if(r!=0) {
					puts("Não foi possível falhar o processo.");
					exit(1);
				}
				
				
				printf("O processo %d falhou no tempo %4.1f\n", token, time());
				break;
			case recovery:
				release(processo[token].id, token);
				printf("O processo %d recuperou no tempo %4.1f\n", token, time());
				schedule(test, 30.0, token);
				break;
		} // switch
	} // while
	return 0;
}

