/*
 * TAD fila
 * Autor:
 *    Joao Vitor de Oliveira Souza
 *
 * Versao 1.0 29/12/22
 */
#include <stdlib.h>
#include <stdio.h>
#include "libfila.h"

/* Cria uma fila vazia e a retorna, se falhar retorna NULL. */
fila_t *cria_fila (){
	fila_t *f;

	if ( !(f = malloc(sizeof(fila_t))) )
		return NULL;
	
	f->ini = NULL;
	f->fim = NULL;
	f->tamanho = 0;

	return f;
}


/* Remove todos os elementos da fila, libera espaco e devolve NULL. */
fila_t *destroi_fila (fila_t *f){
	nodo_f_t *prox, *ante;
	
	prox = f->ini;

	while (prox != NULL){
		ante = prox;
		prox = prox->prox;

		free(ante);
		ante = NULL;
	}
	f->ini = NULL;
	f->fim = NULL;
	free(f);

	return NULL;
}

/* Retorna 1 se a fila esta vazia e 0 caso contrario. */
int vazia_fila (fila_t *f){
	if ( f->tamanho == 0 )
		return 1;
	return 0;
}


/* Retorna o numero de elementos da fila. */
int tamanho_fila (fila_t *f){
	return (f->tamanho);
}


/* Insere um elemento na estrutura seguindo a politica FIFO.
   1 se bem-sucedido e 0 se falhar. */
int insere_fila (fila_t *f, int elemento){
	nodo_f_t *n;
		
	if ( !(n = malloc(sizeof(nodo_f_t))) )
		return 0;

	n->elem = elemento;
	n->prox = NULL;

	if ( vazia_fila(f) ){
		f->ini = n;
		f->fim = n;
		f->tamanho = 1;
		
		return 1;
	}

	f->fim->prox = n;
	f->fim = n;
	f->tamanho++;
	
	return 1;
}


/* Remove elemento da fila seguindo a politica FIFO. 
   Retorna o elemento no parametro "elemento". Retorna 1
   se bem-sucedido e 0 se falhar. */
int retira_fila (fila_t *f, int *elemento){
	nodo_f_t *aux;
	
	if ( vazia_fila(f) )
		return 0;
		
	*elemento = f->ini->elem ;
	aux = f->ini->prox ;

	f->ini->prox = NULL;
	free(f->ini);
	
	f->ini = aux;
	f->tamanho--;
	
	return 1;
}


/* Imprime os elementos da fila, apenas para depuracao. */
void imprime_fila (fila_t *f){
	nodo_f_t *prox;
	
	prox = f->ini;

	while (prox != NULL){
		printf("%d ", prox->elem);
		prox = prox->prox;
	}
	printf("\n");
}
