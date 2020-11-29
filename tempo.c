// Autor: Thayse Marques Solis
// Data da última modificação: 26 de novembro de 2020
// Descrição resumida: Primeiro programa de simulação da disciplina Sistemas Distribuídos.

/* Nosso primeiro programa de simulação
 * Vamos simular N nodos, cada um conta o "tempo" independentemente
 * Um exemplo simples e significativo para captar o "espírito" da simulação */
 
//tempo.c

#include <stdio.h>
#include <stdlib.h>
#include "smpl.h"

// Vamos definir os EVENTOS
#define test 1
#define fault 2
#define recovery 3

// vamos definir o descritor do processo

typedef struct {
	int id;		// identificador de facility (recurso) do SMPL
	// outras variáveis locais dos processos são declaradas aqui!
} TipoProcesso;

TipoProcesso* processo;

int main(int argc, char* argv[]) {
	static int N, // número de processos
		   token, // indica o processo que está sendo executado
		   event, r, i;
	static char fa_name[5];
	
	if(argc!=2) {
		puts("Uso correto: tempo <número de processos>");
		exit(1);
	}
	
	N = atoi(argv[1]);
	
    smpl(0,"Um Exemplo de Simulação"); // Inicia a simulação
	reset();
	stream(1); // Uma única thread de simulação
	
	// inicializar processos
	
	processo = (TipoProcesso*) malloc(sizeof(TipoProcesso)*N);
	
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
	
	for (i = 0; i < N;i++) { // para todos os processos, escalona test para daqui a 30.0 unidades de tempo
		schedule (test, 30.0, i);
	}
	
	schedule(fault, 31.0, 1); // escalona uma falha no tempo 31 para o processo 1
	schedule(recovery, 61.0, 1); // escalona uma recuperação no tempo 61 para o processo 1
	
	//agora vem o loop principal do simulador
	
	while(time()<150.0) {
		cause(&event, &token);
		switch(event) {
			case test:
				if (status(processo[token].id) != 0) break; // se o processo está falho, não testa!
				printf("O processo %d vai testar no tempo %4.1f\n", token, time());
				schedule(test, 30.0, token);
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