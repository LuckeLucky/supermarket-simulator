#include "alocar_free.h"
#include "cli.h"
#include "estruturas.h"
#include "filas.h"
#include "list.h"
#include "sim.h"
#include "texto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
ptPRODUTO encontra_produto(ptHIPERMERCADO h, int i) {
	/*RECEBE UMA POSICAO DO PRODUTO E DEVOLVE O PONTEIRO PARA ESSE PRODUTO*/
	if (!h)
		return NULL;
	ptNOPRODUTO x = h->p_inicio;
	while (x && i) {
		x = x->prox;
		i--;
	}
	return x->inf;
}
void inserir_evento_ordenado(ptSIMULACAO s, ptNOEVENTO e) {
	/*INSERE EVENTO ORDENADO POR ORDEM CRESCENTE DO TEMPO*/
	if (!s || !e)
		return;
	ptNOEVENTO ant = NULL, act = s->eventos;
	while (act != NULL && act->tempo<e->tempo) {
		ant = act;
		act = act->prox;
	}
	e->prox = act;
	if (ant == NULL) {
		s->eventos = e;
	}
	else {
		ant->prox = e;
	}
}
ptNOCLIENTE retirar_cliente_inicio(ptNOCLIENTE *inicio) {
	/*RETIRA O CLIENTE NO INICIO DE UMA FILA QUE SEJA DE NOCLIENTE
	(USA-SE PONTEIRO PARA PONTEIRO)*/
	if (!inicio)
		return NULL;
	ptNOCLIENTE primeiro = *inicio;
	*inicio = (*inicio)->prox;
	primeiro->prox = NULL;
	return primeiro;
}
void entrar_cliente_hipermercado(ptHIPERMERCADO h, ptSIMULACAO s) {
	/*RETIRA O PRIMEIRO CLIENTE DE C_O_INICIO ONDE ESTAO ORDENADOS POR ORDEM DE ENTRADA
	INSERE ORDENADO EM C_INICIO ONDE OS CLIENTES ESTAO ORDENADOS POR ORDEM CRESCENTE DE TEMPO DE ENTRADA FILA */
	if (!h || !s)
		return;
	ptNOCLIENTE act = retirar_cliente_inicio(&(h->c_o_inicio));
	inserir_cliente_ordenado(h, act);
}
void inserir_cliente_ordenado(ptHIPERMERCADO h, ptNOCLIENTE c) {
	/*INSERE ORDENADO PELO TEMPO DE ENTRADA FILA
	SENDO ASSIM O COM O TEMPO MAIS BAIXO DE ENTRADA FILA FICA NO INICIO E APONTA PARA OS RESTANTES*/
	if (!h || !c)
		return;
	ptNOCLIENTE ant = NULL, act = h->c_i_inicio;
	while (act != NULL && act->inf->t_entrada_fila<c->inf->t_entrada_fila) {
		ant = act;
		act = act->prox;
	}
	c->prox = act;
	if (ant == NULL) {
		h->c_i_inicio = c;
	}
	else {
		ant->prox = c;
	}
}
void inserir_cliente_fim(ptNOCLIENTE *inicio, ptNOCLIENTE inserir) {
	/*INSERE O CLIENTE NO FIM DE UMA FILA (USA-SE PONTEIRO PARA PONTEIRO)*/
	if (!inserir)
		return;
	if (!*inicio)
		*inicio = inserir;
	else {
		ptNOCLIENTE act = *inicio;
		while (act->prox)
			act = act->prox;
		act->prox = inserir;
	}
}
void remover_primeiro_evento(ptSIMULACAO s) {
	/**/
	if (!s || !s->eventos)
		return;
	ptNOEVENTO primeiro = s->eventos;
	s->eventos = s->eventos->prox;
	free(primeiro);
}
ptNOFUNCIONARIO funcinario_para_caixa(ptHIPERMERCADO h) {
	/*RETIRAR O FUNCIONARIO DO INICIO JA QUE A LISTRA ESTA ORDENADA POR ORDEM CRESCENTE DE TEMPO DE CAIXA
	O QUE TIVER MENOS TEMPO DE CAIXA E QUE VAI PARA A NOVA CAIXA*/
	if (!h)
		return NULL;
	ptNOFUNCIONARIO f = h->f_inicio;
	h->f_inicio = f->prox;
	f->prox = NULL;
	return f;
}
void inserir_funcionario_ordenado(ptHIPERMERCADO h, ptNOFUNCIONARIO f) {
	/*INSERE FUNCIONARIO POR ORDEM CRESCENTE DE TEMPO DE CAIXA*/
	if (!h || !f)
		return;
	ptNOFUNCIONARIO ant = NULL, act = h->f_inicio;
	while (act != NULL && act->inf->t_de_caixa<f->inf->t_de_caixa) {
		ant = act;
		act = act->prox;
	}
	f->prox = act;
	if (ant == NULL) {
		h->f_inicio = f;
	}
	else {
		ant->prox = f;
	}
}
void pesquisar(ptHIPERMERCADO h, ptSIMULACAO s) {
	/*PESQUISA PELO ID DO CLIENTE EM C_I_INICIO,C_O_INICIO,CAIXAS E ATENDIDOS*/
	if (!h)
		return;
	char ID[7];
	int  tipo = 0;
	fseek(stdin, 0, SEEK_END);
	printf("\nInsira o ID a pesquisar:");
	fgets(ID, 7, stdin);
	ptNOCLIENTE cliente;
	cliente = encontra(h->c_o_inicio, ID);
	if (!cliente)
		cliente = encontra(h->c_i_inicio, ID);
	if (!cliente) {
		for (int i = 0; i < s->n_caixas; i++) {
			cliente = encontra(h->caixas[i].espera_cabeca, ID);
			if (cliente)
				break;
			cliente = encontra(h->caixas[i].atendidos, ID);
			if (cliente)
				break;
		}
	}
	if (cliente)
		printf("\n%s\t%s", cliente->inf->ID, cliente->inf->nome);
	else
		printf("CLIENTE NÃO ENCONTRADO");

}
ptNOCLIENTE encontra(ptNOCLIENTE inicio, char *ID) {
	/*COMPARA A FILA COM O ID SE ENCONTRAR DEVOLVE O C CORRETP*/
	if (!inicio)
		return NULL;
	ptNOCLIENTE c = inicio;
	while (c) {
		if (!strcmp(c->inf->ID, ID))
			break;
		c = c->prox;
	}
	return c;
}
