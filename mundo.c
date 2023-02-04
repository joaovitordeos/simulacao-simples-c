/*
 * Trabalho 1 - Programacao 1 
 * Autor: Joao Vitor de Oliveira Souza
 * 
 * Versao 1.0   27/01/23
 * Versao 1.1   31/01/23 
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "liblef.h"
#include "libfila.h"
#include "libconjunto.h"

/* Constantes relacionadas ao mundo. */
const int FIM_MUNDO = 34944;
const int INICIO_MUNDO = 0;
const int TAM_MAX = 20000;
const int N_HABILIDADES = 10;
const int N_MISSOES = FIM_MUNDO/100;

/* Constantes relacionadas a herois. */
const int PAC_MAX = 100;
const int PAC_MIN = 0;
const int IDD_MIN = 18;
const int IDD_MAX = 100;
const int HAB_MIN = 2;
const int HAB_MAX = 5;
const int HP_INICIAL = 100;
const int HP_MIN = 0;

/* Constantes relacionadas aos locais */
const int CAP_MAX = 30;
const int CAP_MIN = 5;

/* Constantes relacionadas a hospitais  */
const int LEITO_MAX = N_HABILIDADES/2;
const int LEITO_MIN = 1;
const int HP_TAX_MAX = 10;
const int HP_TAX_MIN = 5;

/* Constantes do evento CHEGADA. */
const int PCHG_MIN = 0;
const int PCHG_MAX = 96*7;
const int MIN_PERM = 1;

/* Constantes do evento SAIDA */
const int VEL_MIN = 0;
const int VEL_BASE = 100;
const int VEL_PERDA = 40;

/* Constantes do evento MISSAO */
const int REQ_MAX = 6;
const int REQ_MIN = 3;
const int DAN_MAX = 10;
const int DAN_MIN = 0;
const int XP_MIN = 1;
const int XP_MAX = 5;

/* ID de alguns poderes */
const int CURA = 0;
const int ESCUDO = 1;
const int FORCA = 2;
const int INTELIGENCIA = 3;

/* CURA */
const int CURA_MAX = 5;
const int CURA_MIN = 2;

/* ID de cada evento */
#define CHEGADA 1
#define SAIDA 2
#define MISSAO 3
#define HOSPITAL 4
#define FIM 5

/* ID de cada entidade */
const int HEROI = 1;
const int VILAO = 2;
const int CIDADAO = 3;

typedef struct _vilao {
    int ID;
    int idade;
    int hpMax;
    int hp;
    int crimes;
    conjunto_t *hab;
} vilao;

typedef struct _heroi {
	int ID;
	int pac;
	int idade;
	int exp;
    int hpMax;
    float hp;
	conjunto_t *hab;
} heroi;

typedef struct _hospital {
    int ID;
    int taxCura;
	int pos[2];
    conjunto_t *leitos;
    fila_t *fila;
} hospital;

typedef struct _local {
	int ID;
	int pos[2];
	conjunto_t *herois;
	fila_t *fila;
} local;

typedef struct _mundo {
	int TAMANHO;
	int tempo;
	int nHerois;
	int nLocais;
    int nMeds;
	conjunto_t *todasHab;
	heroi *todosH;
	local *todosL;
    hospital *todosMed;
} mundo;

/* Retorna o maxim
    return NULL;o entre 'a' e 'b'. */
int max(int a, int b){
    if (a < b)
        return b;
    return a;
}

/* Retorna um inteiro representando a velocidade do heroi. */
int velocidade(int idade){
	return VEL_BASE - max(VEL_MIN, idade-VEL_PERDA);
}

/* Retorna a distancia cartesiana entre doi pontos, p1 e p2. */
int distancia_cart(int *p1, int *p2){
	return sqrt(pow(p2[0] - p1[0], 2) + pow(p2[1] - p1[1], 2));
}

/* Retorna um valor inteiro positivo entre min e max. min < max. */
int aleat(int min, int max){
	max = max - min + 1;
	return rand()%max+min;
}

/* Retorna um ponteiro para n entidades 'local' com seus atributos iniciados. 
   Gera locais de 0 a nLocais. Retorna NULL se falhar. */
local *gera_locais(local *l, int nLocais, int tamMax){
	int i;
	
	for (i=0;i<nLocais;i++){
		l[i].herois = cria_cjt(aleat(CAP_MIN, CAP_MAX));
		if ( !l[i].herois )
			return NULL;
		
		l[i].ID = i;
		l[i].pos[0] = aleat(0, tamMax-1);
		l[i].pos[1] = aleat(0, tamMax-1);
		l[i].fila = cria_fila();
	}

	return l;
}

/* Retorna um ponteiro para n entidades 'heroi' iniciando o ID em 0 ate nHerois.
   Retorna NULL se falhar. */
heroi *gera_herois(heroi *h, conjunto_t *habM, int nHerois){
	int i;
	
	for (i=0;i<nHerois;i++){
		h[i].hab = cria_subcjt_cjt(habM, aleat(HAB_MIN, HAB_MAX));
		if ( !h[i].hab )
			return NULL;

		h[i].ID = i;
		h[i].pac = aleat(PAC_MIN, PAC_MAX);
		h[i].idade = aleat(IDD_MIN, IDD_MAX);
		h[i].exp = 0;
        h[i].hpMax = HP_INICIAL;
        h[i].hp = HP_INICIAL;
	}
	return h;
}

/* Gera nMeds hospitais no mundo. */
hospital *gera_hosp(hospital *med, int nMeds, int tamMax){
    int i;
    for (i=0;i<nMeds;i++){
        med[i].leitos = cria_cjt(aleat(LEITO_MIN, LEITO_MAX));
        if ( !med[i].leitos )
            return NULL;

		med[i].pos[0] = aleat(0, tamMax-1);
		med[i].pos[1] = aleat(0, tamMax-1);
		
        med[i].ID = i;
        med[i].taxCura = aleat(HP_TAX_MIN, HP_TAX_MAX);
        med[i].fila = cria_fila();
    }

    return med;
}

/* Preenche um conjunto com valores de 0 a capacidade maxima do conjunto,
   representando as habilidades. */
void preenche_conjunto_hab(conjunto_t *c){
    int i;

    for (i=0; i<(c->max) ;i++)
        insere_cjt(c, i);
}

/* Retorna um ponteiro para a entidade 'mundo' juntamente com seus atributos.
   Retorna NULL se falhar. */
mundo *cria_mundo(){
	mundo *m;
	
	if ( !(m = malloc(sizeof(mundo))) )
		return NULL;

	m->tempo = INICIO_MUNDO;
	m->TAMANHO = TAM_MAX;

	m->nHerois = N_HABILIDADES * 5;
	m->nLocais = m->nHerois/6;
    m->nMeds = m->nLocais/3;

	/* Verificacao da alocacao de memoria. */
	if ( !(m->todosH = malloc(sizeof(heroi)*m->nHerois)) )  return NULL;
	if ( !(m->todosL = malloc(sizeof(local)*m->nLocais)) )  return NULL;
    if ( !(m->todosMed = malloc(sizeof(hospital)*m->nMeds)) )   return NULL;
	if ( !(m->todasHab = cria_cjt(N_HABILIDADES)) ) return NULL;
	
	preenche_conjunto_hab(m->todasHab);
	m->todosH = gera_herois(m->todosH, m->todasHab, m->nHerois);
	m->todosL = gera_locais(m->todosL, m->nLocais, m->TAMANHO);
    m->todosMed = gera_hosp(m->todosMed, m->nMeds, m->TAMANHO);

	/* verificacao da geracao das entidades heroi e local. */
	if ( !m->todosH )
		return NULL;
	if ( !m->todosL )
		return NULL;

	return m;
}

/* Desaloca nMeds em med juntamente com seus atributos e retorna NULL. */
hospital *destroi_hospital(hospital *med, int nMeds){
    int i;
    
    for (i=0;i<nMeds;i++){
        med[i].fila = destroi_fila(med[i].fila);
        med[i].leitos = destroi_cjt(med[i].leitos);
    }

    free(med);

    return NULL;
}

/* Desaloca nHerois em 'h' juntamente com seus atributos e retorna NULL. */
heroi *destroi_herois(heroi *h, int nHerois){
	int i;

	for (i=0;i<nHerois;i++)
		h[i].hab = destroi_cjt(h[i].hab);

	free(h);

	return NULL;
}

/* Desaloca nLocais em 'l' juntamente com seus atributos e retorna NULL. */
local *destroi_locais(local *l, int nLocais){
	int i;

	for (i=0;i<nLocais;i++){
		l[i].herois = destroi_cjt(l[i].herois);
		l[i].fila = destroi_fila(l[i].fila);
	}
	
	free(l);

	return NULL;
}

/* Desaloca a entidade 'mundo' e todos os seus atributos e retorna NULL. */
mundo *destroi_mundo(mundo *m){
	m->todasHab = destroi_cjt(m->todasHab);

	m->todosH = destroi_herois(m->todosH, m->nHerois);
	m->todosL = destroi_locais(m->todosL, m->nLocais);
    m->todosMed = destroi_hospital(m->todosMed, m->nMeds);
	
	free(m);

	return NULL;
}

/* Insere o evento FIM na LEF no tempo FIM_MUNDO. Retorna 1, se falhar retorna 0. */
int adiciona_fim(lef_t *l){
	evento_t fim;

	fim.tempo = FIM_MUNDO;
	fim.tipo = FIM;
	fim.dado1 = 0;
	fim.dado2 = 0;

	return adiciona_ordem_lef(l, &fim);
}

/* Insere um evento MISSAO na LEF. Retorna 1, se falhar retorna 0. */
int gera_missao(lef_t *l, mundo *m, int idMissao, int tempMissao){
	evento_t e;

	e.tempo = tempMissao;
	e.tipo = MISSAO;
	e.dado1 = idMissao;
	e.dado2 = -1;
	
	return adiciona_ordem_lef(l, &e);
}

/* Insere todos os eventos MISSAO do mundo na LEF. */
void gera_todas_missoes(lef_t *l, mundo *m){
    int i;

    for (i=0; i<N_MISSOES ; i++){
        gera_missao(l, m, i, aleat(INICIO_MUNDO + 1, FIM_MUNDO - 1));
	}
}

/* Insere um evento SAIDA na LEF. Retorna 1, se falhar retorna 0. */
int gera_saida(lef_t *l, int tempSaida, int idHeroi, int idLocal){
	evento_t e;
	
	e.tipo = SAIDA;
	e.tempo = tempSaida;
	e.dado1 = idHeroi;
	e.dado2 = idLocal;

	return adiciona_ordem_lef(l, &e);
}

/* Insere um evento CHEGADA de um heroi na LEF. Retorna 1, se falhar retorna 0. */
int gera_chegada(lef_t *l, mundo *m, int tempChegada, int idHeroi, int idLocal){
	evento_t e;
	
	e.tipo = CHEGADA;
	e.tempo = tempChegada;
	e.dado1 = idHeroi;
	e.dado2 = idLocal;
	
	if (tempChegada == m->tempo)
		return adiciona_inicio_lef(l, &e);
	else 
		return adiciona_ordem_lef(l, &e);
}

/* Insere um evento de ida a um hospital na LEF. Retorna 1, se falhar retorna 0. */
int gera_internacao(lef_t *e, int tempIntern, int idHeroi, int idMed){
	return 1;
}

/* Insere os primeiros eventos CHEGADA na LEF. */
void adiciona_chegada_inicial(lef_t *l, mundo *m){
    int i;

    for (i=0; i<(m->nHerois) ; i++)
        gera_chegada(l, m, aleat(PCHG_MIN, PCHG_MAX), i, aleat(0, m->nLocais-1));
    
}

void remove_hp(mundo *m, int ID, int perda, int entidade){
    if ( entidade == HEROI) m->todosH[ID].hp -= perda; else
    if ( entidade == VILAO) NULL; else 
    if ( entidade == CIDADAO) NULL;
}

void adiciona_hp(mundo *m, int ID, int ganho, int entidade){
    if ( entidade == HEROI) 
		if (m->todosH[ID].hpMax >= (m->todosH[ID].hp+ganho))
			m->todosH[ID].hp += ganho;
		else 
			m->todosH[ID].hp = m->todosH[ID].hpMax;
	else if ( entidade == VILAO) 
		NULL; 
	else if ( entidade == CIDADAO) NULL;
}

void adiciona_xp(mundo *m, int ID, int ganho){
	m->todosH[ID].exp += ganho;
}

/* Retorna um ponteiro para um conjunto de todas as habilidades em um local */
conjunto_t *uniao_habs(heroi *todosH, local *localAtual){
	int i, j, numHabs, numHerois, idHeroi, habAtual;
	conjunto_t *uniaoHab;
	heroi *heroiAtual;

	if ( !(uniaoHab = cria_cjt(N_HABILIDADES)) ) return NULL;

	numHerois = cardinalidade_cjt(localAtual->herois);

	inicia_iterador_cjt(localAtual->herois);
	for (i=0; i<numHerois ;i++){
		if (incrementa_iterador_cjt(localAtual->herois, &idHeroi)){
            heroiAtual = &todosH[idHeroi];
			numHabs = cardinalidade_cjt(heroiAtual->hab);
			
			inicia_iterador_cjt(heroiAtual->hab);
			for (j=0; j<numHabs ;j++){
				if (incrementa_iterador_cjt(heroiAtual->hab, &habAtual)){
					insere_cjt(uniaoHab, habAtual);
				}
			}
		}
	}
	return uniaoHab;
}

/* Essa procedure processa todas as informacoes pos missao, 
   atualizando os atributos dos herois. */
void resultado_missao(lef_t *e, mundo *m, local *localAtual){
	int i, numHerois, idHeroi, possuiCura, possuiEscudo, curaTotal, ganhoXP, dano;
	conjunto_t *uniaoHab;

	if ( !(uniaoHab = uniao_habs(m->todosH, localAtual)) ) return NULL;
	
	/* Verifica se essas habilidades estao no grupo de heroi desse local. */
	possuiCura = pertence_cjt(uniaoHab, CURA);
	possuiEscudo = pertence_cjt(uniaoHab, 	ESCUDO);

	numHerois = cardinalidade_cjt(localAtual->herois);

	inicia_iterador_cjt(localAtual->herois);
	for (i=0;i<numHerois;i++){
		if (incrementa_iterador_cjt(localAtual->herois, &idHeroi)){
			ganhoXP = aleat(XP_MIN, XP_MAX	);
			dano =  aleat(DAN_MIN, DAN_MAX);
			curaTotal = aleat(CURA_MIN, CURA_MAX);

			if (m->todosH[idHeroi].hp < HP_MIN)
				gera_internacao(e, 1, 1, 1);
			
			/* Se possui cura na equipe, o heroi recebe curaTotal de HP. */
			if (possuiCura ){
				adiciona_hp(m, idHeroi, curaTotal, HEROI);
				printf("%6d:CURA  HEROI %2d:+%d HP\n", m->tempo, idHeroi, curaTotal);
			} 

			/* Se possui escudo na equipe, o heroi recebe apenas metade do dano. */
			if( possuiEscudo ) {
				remove_hp(m, idHeroi, dano/2, HEROI);
				printf("%6d:PROTE HEROI %2d:DANO EVITADO %d\n", m->tempo, idHeroi, dano/2);
				printf("%6d:DANO  HEROI %2d:DANO RECEBIDO -%d HP\n", m->tempo, idHeroi, dano/2);
			} else {
				remove_hp(m, idHeroi, dano, HEROI);
				printf("%6d:DANO  HEROI %2d:-%d HP\n", m->tempo, idHeroi, dano);
			}

			/* Se possuir o poder INTELIGENCIA, o heroi recebe o dobro do ganho de XP. */
			if(pertence_cjt(m->todosH[idHeroi].hab, INTELIGENCIA)) {
				m->todosH[idHeroi].exp += ganhoXP*2;
				printf("%6d:XP x2 HEROI %2d:+%d XP\n", m->tempo, idHeroi, ganhoXP*2);
			}  else {
				m->todosH[idHeroi].exp += ganhoXP*2;
				printf("%6d:XP    HEROI %2d:+%d XP\n", m->tempo, idHeroi, ganhoXP);
			}

		}
	}

	uniaoHab = destroi_cjt(uniaoHab);
}

/* Essa procedure eh responsavel por tratar do evento CHEGADA. */
void trata_chegada(lef_t *EVENTOS, mundo *MUNDO, heroi *Heroi, local *Local){
	int pacHeroi, cap_max, cap_atual, tpl, idHeroi, idLocal;
	
	idHeroi = Heroi->ID;
	idLocal = Local->ID;

	pacHeroi =	Heroi->pac;
	cap_max = Local->herois->max;
	cap_atual = cardinalidade_cjt(Local->herois);

	printf("%6d:CHEGA HEROI %2d ", MUNDO->tempo, idHeroi);
	printf("Local %d (%2d/%2d),", idLocal, cap_atual, cap_max);
    
	if (cap_atual < cap_max){
		printf(" ENTRA\n");

		insere_cjt(Local->herois, Heroi->ID);
        
		tpl = max(MIN_PERM, pacHeroi/10+aleat(-2, 6));
		gera_saida(EVENTOS, tpl+MUNDO->tempo, idHeroi, idLocal);
    
	} else {
		if ( (pacHeroi/4 - tamanho_fila(Local->fila)) > 0){

			insere_fila(Local->fila, idHeroi);

			printf(" FILA %2d\n", tamanho_fila(Local->fila));
		} else {
			gera_saida(EVENTOS, MUNDO->tempo, idHeroi, idLocal);

			printf(" DESISTE\n");
		}
	}
}

/* Essa procedure eh responsavel por tratar do evento SAIDA. */
void trata_saida(lef_t *EVENTOS, mundo *MUNDO, heroi *Heroi, local *Local){
	int proxHeroi, tdl, novoIdLocal, novoTempo;
	
	int idHeroi = Heroi->ID;
	int idLocal = Local->ID;
	int cap_max = Local->herois->max;
	int cap_atual = Local->herois->card;
	
	printf("%6d:SAIDA HEROI %2d ", MUNDO->tempo, idHeroi);
	printf("Local %d (%2d/%2d)", idLocal, cap_atual, cap_max);
	
	novoIdLocal = aleat(0, MUNDO->nLocais - 1);
	
	tdl = distancia_cart(Local->pos, MUNDO->todosL[novoIdLocal].pos);
	tdl = tdl/velocidade(Heroi->idade);
	
	novoTempo = MUNDO->tempo + tdl/15;
	
	gera_chegada(EVENTOS, MUNDO, novoTempo, idHeroi, novoIdLocal);

	if ( pertence_cjt(Local->herois, idHeroi) ){

        retira_cjt(Local->herois, idHeroi);
        if ( !vazia_fila(Local->fila) ){
            retira_fila(Local->fila, &proxHeroi);
            printf(", REMOVE FILA HEROI %d", proxHeroi);

            gera_chegada(EVENTOS, MUNDO, MUNDO->tempo, proxHeroi, idLocal);
        }
    }
	printf("\n");
}

/* Essa procedure eh responsavel por tratar do evento SAIDA. */
void _trata_saida(lef_t *EVENTOS, mundo *MUNDO, evento_t saida){
 
}

/* Essa procedure eh responsavel por tratar do evento MISSAO. */
void verifica_possibilidade(mundo *MUNDO, evento_t *missao, conjunto_t *habMissao, local *localAtual){
	int c1, c2;
	conjunto_t *uniaoHab;

	uniaoHab = uniao_habs(MUNDO->todosH, localAtual);

	printf("%6d:MISSAO %3d HAB_EQL %d:", MUNDO->tempo, missao->dado1, localAtual->ID);
	imprime_cjt(uniaoHab);

	if (contido_cjt(habMissao, uniaoHab)){
		if (missao->dado2 < 0)
			missao->dado2 = localAtual->ID;
		else {
			c1 = cardinalidade_cjt(MUNDO->todosL[missao->dado2].herois);
			c2 = cardinalidade_cjt(localAtual->herois);
			if ( c1 > c2)
				missao->dado2 = localAtual->ID;
		}
	}

	uniaoHab = destroi_cjt(uniaoHab);
}

void trata_missao(lef_t *EVENTOS, mundo *MUNDO, evento_t *missao){
    conjunto_t *habMissao;
    int i;

    habMissao = cria_subcjt_cjt(MUNDO->todasHab, aleat(REQ_MIN, REQ_MAX));

	printf("%6d:MISSAO %3d HAB_REQ ", MUNDO->tempo, missao->dado1);
	imprime_cjt(habMissao);
			
	for (i=0;i<MUNDO->nLocais;i++)
		verifica_possibilidade(MUNDO, missao, habMissao, &MUNDO->todosL[i]);

	if (missao->dado2 < 0){
		printf("%6d:MISSAO %3d IMPOSSIVEL\n", MUNDO->tempo, missao->dado1);
		gera_missao(EVENTOS, MUNDO, missao->dado1, aleat(MUNDO->tempo, FIM_MUNDO));
        
		/* Todos perdem HP */
        for (i=0;i<MUNDO->nHerois;i++)
            remove_hp(MUNDO, i, MUNDO->todosH[i].hpMax*0.015, HEROI);
        for (i=0;i<MUNDO->nHerois;i++)
            printf("%6d:STATUS HEROI %2d: HP:%.0f\n", MUNDO->tempo, i, MUNDO->todosH[i].hp);
	} else {
		printf("%6d:MISSAO %3d HER_ESQ %d:", MUNDO->tempo, missao->dado1, MUNDO->todosL[missao->dado2].ID);
		imprime_cjt(MUNDO->todosL[missao->dado2].herois);
		resultado_missao(EVENTOS ,MUNDO, &MUNDO->todosL[missao->dado2]);
	} 

	habMissao = destroi_cjt(habMissao);
}


/* Essa procedure eh responsavel por tratar o evento FIM. */
lef_t *trata_fim(lef_t *EVENTOS, mundo *MUNDO){
	int i;

	printf("%6d:FIM\n", MUNDO->tempo);
	EVENTOS = destroi_lef(EVENTOS);
	 
	for (i=0; i<(MUNDO->nHerois) ;i++)
		printf("HEROI %2d EXPERIENCIA %2d\n", i, MUNDO->todosH[i].exp);

	return NULL;
}


int main(){
	local *Local;
	heroi *Heroi;
	evento_t *e;
	lef_t *EVENTOS;
	mundo *MUNDO;
	
	srand(time(0));

	EVENTOS = cria_lef();
	MUNDO = cria_mundo();
	
	adiciona_fim(EVENTOS);
	adiciona_chegada_inicial(EVENTOS, MUNDO);	
	gera_todas_missoes(EVENTOS, MUNDO);
	
	while ( (EVENTOS != NULL) && (e = obtem_primeiro_lef(EVENTOS))){
		MUNDO->tempo = e->tempo;
		switch (e->tipo){
			case CHEGADA:
				Heroi = &MUNDO->todosH[e->dado1];
				Local = &MUNDO->todosL[e->dado2];

				trata_chegada(EVENTOS, MUNDO, Heroi, Local);					
				free(e);
				break;
			case SAIDA:
				Heroi = &MUNDO->todosH[e->dado1];
				Local = &MUNDO->todosL[e->dado2];

				trata_saida(EVENTOS, MUNDO, Heroi, Local);
				free(e);
				break;
			case MISSAO:
				trata_missao(EVENTOS, MUNDO, e);
				free(e);
                break;
			case FIM:
				EVENTOS = trata_fim(EVENTOS, MUNDO);
				free(e);
				break;
		}
	}
	
	free(EVENTOS);
	MUNDO = destroi_mundo(MUNDO);
	
	return 0;
}