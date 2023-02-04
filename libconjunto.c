/*
 * TAD conjunto
 * Autor:
 *		Joao Vitor de Oliveira Souza
 *
 *	Versao 1.0   26/11/22
 *  Versao 1.0.1 27/11/22
 *  Versao 1.1   29/12/22
 */
#include <stdlib.h>
#include <stdio.h>
#include "libconjunto.h"


/*
 * Cria um conjunto vazio e retorna um ponteiro para o 
 * conjunto, em caso de falha, retorna NULL.
 */
conjunto_t *cria_cjt (int max){
	conjunto_t *c;
	int *v;

	if ( !(c = malloc(sizeof(conjunto_t))) )
		return NULL;
	
	if ( !(v = malloc(sizeof(int)*max+1)) )
		return NULL;


	c->max  = max;
	c->card =   0;
	c->ptr  =   0;
	c->v    =   v;

	return c;
}


/*
 * Libera o conjunto (e seu vetor), retorna NULL.
 */
conjunto_t *destroi_cjt (conjunto_t *c ){
	free(c->v);
	c->v = NULL;
	
	free(c);

	return NULL;
}


/*
 * Verifica se o conjunto esta vazio, vazio = 1 & !vazio = 0.
 */
int vazio_cjt (conjunto_t *c){
	if ( c->card == 0 )
		return 1;
	return 0;
}


/*
 * Retorna o num de elementos no conjunto.
 */
int cardinalidade_cjt (conjunto_t *c){
	return c->card;
}

/*
 * Busca o elemento no vetor, retorna o indice do elemento.
 * retorna -1 se nao encontrar.
 */
int busca (int *v, int ini, int fim, int elemento){
	int meio;

	if (ini > fim)
		return -1;
		
	meio = (ini + fim) / 2;
	
	if (elemento < v[meio])
		return busca(v, ini, meio - 1, elemento);

	if (elemento > v[meio])
		return busca(v, meio + 1, fim, elemento); 
	
	return meio;
}


/* 
 * Essa funcao troca a posicao de 2 elementos de um vetor.
 */
int troca (int *v, int x, int y){
	int aux;

	aux = v[x];
	v[x] = v[y];
	v[y] = aux;

	return 1;
}


/*
 * Insere um elemento no conjunto se ainda nao estiver contido.
 * Retorna 1 se bem-sucedido, ou 0, se falhar.
 */
int insere_cjt (conjunto_t *c, int elemento){
	int i;

	if ( !pertence_cjt(c, elemento) && (c->card < c->max) ){		
		c->v[c->card] = elemento;
		
		if (vazio_cjt(c)){
			c->v[0] = elemento;
			c->card++;
			return 1;
		}

		for (i=c->card-1 ; i>=0 && c->v[i] > c->v[i+1]; i--){
			troca(c->v, i, i+1);
		}
		c->card++;

		return 1;
	}
	else if (c->card < c->max)
		return 1;

	return 0;
}


/*
 * Remove o elemento do conjunto caso exista e retorna 1,
 * caso nao exista retorna 0.
 */
int retira_cjt (conjunto_t *c, int elemento){
	int idc, i;

	if (pertence_cjt(c, elemento)){
		idc = busca(c->v, 0, c->card -1, elemento);

		for (i=idc;i < c->card-1; i++)
			troca(c->v, i, i+1);

		c->card--;

		return 1;
	}
	return 0;
}


/*
 * Verifica se o elemento pertence ao conjunto.
 * Retorna 1 se pertence e 0 caso nao.
 */
int pertence_cjt (conjunto_t *c, int elemento){
	if ( busca(c->v, 0, c->card-1, elemento) != -1 )
		return 1;
	return 0;
}


/*
 * Verifica se o c1 está contido no c2, isto eh, se os elementos
 * de c1 estao presentes em c2, retorna 1 se esta contido e 0 caso contrario.
 */
int contido_cjt (conjunto_t *c1, conjunto_t *c2){
	int i;

	if (c1->card > c2->card)
		return 0;

	for (i=0; i < c1->card ;i++){
		if ( !pertence_cjt(c2, c1->v[i]) )	
			return 0;
	}
	return 1;
}


/*
 * Verifica se os dois conjuntos sao iguais, se c1 = c2 retorna 1 e 0 se c1 != c2.
 */
int	sao_iguais_cjt (conjunto_t *c1, conjunto_t *c2){
	int i;

	if (c1->card != c2->card)
		return 0;

	for (i=0; i < c1->card ;i++)
		if (c1->v[i] != c2->v[i])
			return 0;
	
	return 1;
} 


/*
 * Cria um conjunto com a diferenca entre c1 e c2, nesta ordem.
 * Retorna NULL se falhar.
 */
conjunto_t *diferenca_cjt (conjunto_t *c1, conjunto_t *c2){
	conjunto_t *diff;
	int i, elemento;
	
	if ( !(diff = cria_cjt( c1->card )) )
		return NULL;
	
	for (i=0; i < c1->card ;i++){
		elemento = c1->v[i];

		if (!pertence_cjt(c2, elemento))
			insere_cjt(diff, elemento);
	}

	return diff;
}


/*
 * Cria um conjunto com a intersecao dos conjuntos c1 e c2, e retorna um ponteiro.
 * Retorna NULL se falhar.
 */
conjunto_t *interseccao_cjt (conjunto_t *c1, conjunto_t *c2){
	conjunto_t *inter;
	int i, elemento;
	
	if ( !(inter = cria_cjt( c1->card )) )
		return NULL;
	
	for (i=0; i < c1->card ;i++){
		elemento = c1->v[i];

		if ( pertence_cjt(c2, elemento) )
			insere_cjt(inter, elemento);
	}

	return inter;
}


/*
 * Cria um conjunto uniao dos elementos de c1 e c2, e o ponteiro para o mesmo.
 * Retorna NULL caso falhe.
 */
conjunto_t *uniao_cjt (conjunto_t *c1, conjunto_t *c2){
	conjunto_t *uniao;
	int i, maior;

	if (c1->card > c2->card)
		maior = c1->card;
	else
		maior = c2->card;
	
	if ( !(uniao = cria_cjt(c1->card + c2->card)) )
		return NULL;
	
	for (i=0; i < maior ;i++){
		if (i < c1->card)
			insere_cjt(uniao, c1->v[i]);
		
		if (i < c2->card)
			insere_cjt(uniao, c2->v[i]);
	}
	
	return uniao;
}


/*
 * Cria uma copia do conjunto e retorna um ponteiro para a copia.
 * Em caso de falha, retorna NULL.
 */
conjunto_t *copia_cjt (conjunto_t *c){
	conjunto_t *copia;
	int i;

	if ( (copia = cria_cjt(c->card)) ){
		for (i=0; i < c->card ; i++){
			copia->v[i] = c->v[i];
			copia->card++;
		}
		
		return copia;
	}
	return NULL;
}


/*
 * Cria um subconjunto com elementos aleatorios de c, e retorna um ponteiro para o
 * mesmo, em caso de falha retorna NULL.
 */
conjunto_t *cria_subcjt_cjt (conjunto_t *c, int n){
	conjunto_t *subcjt;
	
	if ( (n >= c->card) || (c->card==0) ) 
		return copia_cjt(c);
	
	if( !(subcjt = cria_cjt(n)) )
		return NULL;

	while (subcjt->card < subcjt->max)
		insere_cjt(subcjt, c->v[rand()%(c->card)]);
		
	return subcjt;
}


/*
 * Imprime de forma crescente todos os elementos de c.
 */
void imprime_cjt (conjunto_t *c){
	int i;

	for (i=0; i < c->card ;i++){
		printf("%d", c->v[i]);
		if (i != c->card-1)
			printf(" ");
	}
	if (vazio_cjt(c))
		printf("conjunto vazio");
	printf("\n");
}


/*
 * Inicia iterador do conjunto.
 */
void inicia_iterador_cjt (conjunto_t *c){
	c->ptr = -1;
}	


/*
 * Essa funcao devolve no parametro *ret_iterador o elemento apontado por ptr e
 * incrementa o iterador. Retorna 0 se o iterador ultrapassar o ultimo elemento,
 * ou retornara 1 se o iterador apontar para um elemento valido.
 */
int incrementa_iterador_cjt (conjunto_t *c, int *ret_iterador){
	if (c->ptr < c->max){
		c->ptr++;
		*ret_iterador = c->v[c->ptr];
		return 1;
	}

	c->ptr = 0;
	return 0;
}


/*
 * Retorna um elemento qualquer do conjunto, o mesmo eh removido do conjunto.
 */
int retira_um_elemento_cjt (conjunto_t *c){
	int elemento;

	elemento = c->v[ c->card-1 ];
	retira_cjt(c, c->v[c->card-1]);

	return elemento;
}
