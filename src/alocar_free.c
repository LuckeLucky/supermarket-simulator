#include "alocar_free.h"
#include "estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

ptHIPERMERCADO alocar_hipermercado() {
	/*ALOCA HIPERMERCADO*/
	ptHIPERMERCADO h = (ptHIPERMERCADO)malloc(sizeof(struct hipermercado));
	if (!h)
		return NULL;
	h->c_o_inicio = NULL;
	h->c_i_inicio = NULL;
	h->f_inicio = NULL;
	h->p_inicio = NULL;
	h->caixas = NULL;
	h->c_lidos = 0;
	h->f_lidos = 0;
	h->p_lidos = 0;
	h->t_espera = 0;
	h->clientes_por_fila = 0;
	return h;
}
ptNOCLIENTE alocar_nocliente() {
	/*ALOCA NOCLIENTE*/
	ptNOCLIENTE c = (ptNOCLIENTE)malloc(sizeof(struct nocliente));
	if (!c)
		return NULL;
	c->prox = NULL;
	c->inf = (ptCLIENTE)malloc(sizeof(struct cliente));
	if (!c->inf) {
		free(c);
		return NULL;
	}
	c->inf->ID = NULL;
	c->inf->nome = NULL;
	c->inf->n_produtos = 0;
	c->inf->preco = 0;
	c->inf->produto_preco_alto = 0;
	c->inf->t_entrada = 0;
	c->inf->t_entrada_fila = 0;
	c->inf->t_espera = 0;
	c->inf->t_saida = 0;
	c->inf->t_caixa = 0;
	return c;
}
ptNOFUNCIONARIO alocar_nofuncionario() {
	/*ALOCA FUNCIONARIO*/
	ptNOFUNCIONARIO f = (ptNOFUNCIONARIO)malloc(sizeof(struct nofuncionario));
	if (!f)
		return NULL;
	f->prox = NULL;
	f->inf = (ptFUNCIONARIO)malloc(sizeof(struct funcionario));
	if (!f->inf) {
		free(f);
		return NULL;
	}
	f->inf->numero = NULL;
	f->inf->nome = NULL;
	f->inf->caixa_atual = 0;
	f->inf->t_de_caixa = 0;
	f->inf->c_atendidos = 0;
	return f;
}
ptNOPRODUTO alocar_noproduto() {
	/*ALOCA NO PRODUTO*/
	ptNOPRODUTO p = (ptNOPRODUTO)malloc(sizeof(struct noproduto));
	if (!p)
		return NULL;
	p->prox = NULL;
	p->inf = (ptPRODUTO)malloc(sizeof(struct produto));
	if (!p->inf) {
		free(p);
		return NULL;
	}
	p->inf->codigo = NULL;
	p->inf->nome = NULL;
	p->inf->preco = 0;
	p->inf->t_caixa = 0;
	p->inf->t_compra = 0;
	return p;
}
CAIXA* alocar_caixas(int n) {
	/*ALOCA CAIXAS*/
	CAIXA *c = (CAIXA*)malloc(sizeof(struct caixa)*n);
	if (!c)
		return NULL;
	for (int i = 0; i < n; i++) {
		c[i].p_vendidos = 0;
		c[i].valor = 0;
		c[i].estado = 0;
		c[i].x_abriu = 0;
		c[i].t_espera = 0;
		c[i].p_oferecidos = 0;
		c[i].valor_oferecido = 0;
		c[i].espera_cabeca = NULL;
		c[i].espera_cauda = NULL;
		c[i].atendidos = NULL;
		c[i].funcionario = NULL;
	}
	return c;
}
ptSIMULACAO alocar_simulacao() {
	/*ALOCA A SIMULACAO*/
	ptSIMULACAO s = (ptSIMULACAO)malloc(sizeof(struct simulacao));
	if (!s)
		return NULL;
	s->eventos = NULL;
	return s;
}
ptNOEVENTO alocar_noevento(int evento, double tempo) {
	/*ALOCA 1 EVENTO*/
	ptNOEVENTO e = (ptNOEVENTO)malloc(sizeof(struct noeventos));
	if (!e)
		return NULL;
	e->prox = NULL;
	e->tempo = tempo;
	e->tipo = evento;
	return e;
}
void free_nocliente(ptNOCLIENTE c) {
	/*LIBERTA O NOCLINTE*/
	if (!c)
		return;
	if (c->inf) {
		if (c->inf->ID)
			free(c->inf->ID);
		if (c->inf->nome)
			free(c->inf->nome);
		free(c->inf);
	}
	free(c);

}
void free_nofuncionario(ptNOFUNCIONARIO f) {
	/*LIBERTA O NOFUNIONARIO*/
	if (!f)
		return;
	if (f->inf) {
		if (f->inf->nome)
			free(f->inf->nome);
		if (f->inf->numero)
			free(f->inf->numero);
		free(f->inf);
	}
	free(f);
}
void free_noproduto(ptNOPRODUTO p) {
	/*LIBERTA NOPRODUTO*/
	if (!p)
		return;
	if (p->inf) {
		if (p->inf->codigo)
			free(p->inf->codigo);
		if (p->inf->nome)
			free(p->inf->nome);
		free(p->inf);
	}
	free(p);
}
void libertar_mem_total(ptHIPERMERCADO h, ptSIMULACAO s) {
	/*LIBERTA A MEMORIA DE TODA A SIMULACAO*/
	if (!h || !s)
		return;
	ptNOCLIENTE ant, act;
	ant = NULL;
	act = h->c_o_inicio;
	while (act) {
		ant = act;
		act = act->prox;
		free_nocliente(ant);

	}
	act = h->c_i_inicio;
	while (act) {
		ant = act;
		act = act->prox;
		free_nocliente(ant);

	}
	for (int i = 0; i < s->n_caixas; i++) {
		act = h->caixas[i].espera_cabeca;
		while (act) {
			ant = act;
			act = act->prox;
			free_nocliente(ant);

		}
		act = h->caixas[i].atendidos;
		while (act) {
			ant = act;
			act = act->prox;
			free_nocliente(ant);

		}
	}
	free(h->caixas);
	ptNOPRODUTO p_act, p_ant;
	p_act = h->p_inicio;
	while (p_act) {
		p_ant = p_act;
		p_act = p_act->prox;
		free_noproduto(p_ant);

	}
	ptNOFUNCIONARIO f_act, f_ant;
	f_act = h->f_inicio;
	while (f_act) {
		f_ant = f_act;
		f_act = f_act->prox;
		free_nofuncionario(f_ant);

	}
	free(h);
	ptNOEVENTO e_act, e_ant;
	e_act = s->eventos;
	while (e_act) {
		e_ant = e_act;
		e_act = e_act->prox;
		free(e_ant);
	}
	free(s);
}