/* TAD Lista de Eventos Futuros (LEF)
 * 
 * Autor: Joao Vitor de Oliveira Souza
 * 
 * Versao 1.0  29/12/22
 * Versao 1.1  20/01/22
 */
#include <stdlib.h>
#include <stdio.h>
#include "liblef.h"

/* Cria uma LEF vazia e retorna um ponteiro, retorna NULL se falhar. */
lef_t *cria_lef (){
	lef_t *l;

	if ( !(l = malloc(sizeof(lef_t))) )
		return NULL;
	
	l->Primeiro = NULL;
	
	return l;
}


/* Destroi a LEF e retorna NULL. */
lef_t *destroi_lef (lef_t *l){
	nodo_lef_t *prox, *ante;

	prox = l->Primeiro;

	while (prox != NULL){
		ante = prox;
		prox = prox->prox;
		
		free(ante->evento);
		ante->evento = NULL;

		free(ante);
		ante = NULL;
	}

	l->Primeiro = NULL;
	free(l);

	return NULL;
}

/* Copia o evento 2 no evento 1. */
void copia_evento (evento_t *e1, evento_t *e2){
	e1->tempo = e2->tempo;
	e1->tipo = e2->tipo;
	e1->dado1 = e2->dado1;
	e1->dado2 = e2->dado2;
	e1->dado3 = e2->dado3;
}

/* Retorna 1 se LEF vazia e 0 caso contrario. */
int vazia_lef(lef_t *l){
	if (l->Primeiro == NULL)
		return 1;
	return 0;
}


/* Adiciona um evento na primeira posicao da LEF. 
   Retorna 1 se bem-sucedido, 0 caso contrario. */
int adiciona_inicio_lef (lef_t *l, evento_t *evento){
	nodo_lef_t *n, *aux;
	evento_t *e;

	if ( !(e = malloc(sizeof(evento_t))) )
		return 0;
	if ( !(n = malloc(sizeof(nodo_lef_t))) )
		return 0;

	copia_evento(e, evento);
	n->evento = e;
	n->prox = NULL;
	
	aux = l->Primeiro; 
	l->Primeiro = n;
	n->prox = aux;

	return 1;
}

	
/* Adiciona um evento de acordo com o evento->tempo na LEF.
   Retorna 1 se bem-sucedido e 0 caso contrario. */
int adiciona_ordem_lef (lef_t *l, evento_t *evento){
	nodo_lef_t *prox, *ante, *n;
	evento_t *e;

	prox = l->Primeiro;
	ante = NULL;

	if (vazia_lef(l)){
		if ( !adiciona_inicio_lef(l, evento) )
			return 0;
		return 1;
	} 
	
	else {
		while (prox != NULL && ((prox->evento->tempo) < (evento->tempo)) ){
			ante = prox;
			prox = prox->prox;
		}

		if (ante == NULL){
			if ( !adiciona_inicio_lef(l, evento) )
            	return 0;
        	return 1;
		} 

		else {
			/* aloca um novo nodo para ser inserido. */
			
			if ( !(e = malloc(sizeof(evento_t))) )
				return 0;

        	if ( !(n = malloc(sizeof(nodo_lef_t))) )
            	return 0;
        	copia_evento(e, evento);
				
			n->evento = e;
			n->prox = prox;
			ante->prox = n;

			return 1;
		}
	}
	return 0;
}


/* Retorna e retira o primeiro evento da LEF */
evento_t *obtem_primeiro_lef (lef_t *l){
	nodo_lef_t *aux;
	evento_t *e;
	
	if (vazia_lef(l))
		return NULL;

	if ( !(e = malloc(sizeof(evento_t)))  )
		return NULL;
	
	copia_evento(e, l->Primeiro->evento);
	aux = l->Primeiro;
	l->Primeiro = l->Primeiro->prox;
	
	free(aux->evento);
	free(aux);

	return e;
}
