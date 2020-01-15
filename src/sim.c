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
#define ENTRADA_CLIENTE 1
#define ENTRADA_CLIENTE_FILA 2
#define SAIDA_CLIENTE 3

void gerar_cliente(ptNOCLIENTE nc, ptHIPERMERCADO h) {
	/*GERA OS CLIENTES QUE FORAM LIDOS,ATRIBUI T_ENTRADA,T_ENTRADA_FILA,N_PRODUTOS
	SE OS PRODUTOS FOREM IGUAL 0 É ATRIBUIDO A MESMA UM TEMPO DE ENTRADA NA FILA...*/
	if (!nc || !h) {
		exit(1);
	}
	int pos_produto;
	ptPRODUTO prod;
	nc->inf->n_produtos = num_aleatorio(30);
	for (int i = 0; i < nc->inf->n_produtos; i++) {
		pos_produto = num_aleatorio(h->p_lidos);
		prod = encontra_produto(h, pos_produto);
		nc->inf->preco += prod->preco;
		nc->inf->t_entrada_fila += prod->t_compra;
		nc->inf->t_caixa += prod->t_caixa;
		if (nc->inf->produto_preco_alto < prod->preco)
			nc->inf->produto_preco_alto = prod->preco;
	}
	if (nc->inf->n_produtos == 0)
		nc->inf->t_entrada_fila = (nc->inf->t_entrada + (double)num_aleatorio(200));
	else
		nc->inf->t_entrada_fila += nc->inf->t_entrada;
}
void gerar_clientes(ptSIMULACAO s, ptHIPERMERCADO h) {
	/*APOS A CONFIGURACAO DA SIMULACAO SAO GERADOS OS CLIENTES ATE AO TEMPO TOTAL TENDO EM CONTA
	QUANTOS PODEM ENTRAR AO MESMO TEMPO E DE QUANTO EM QUANTO TEMPO ENTRAM*/
	if (!s || !h)
		exit(1);
	double t = 0;
	int clientes_mesmo_tempo;
	ptNOCLIENTE no;
	no = h->c_o_inicio;
	do {
		clientes_mesmo_tempo = num_aleatorio(s->gerar_ao_mesmo_tempo) + 1;
		t = ((double)num_aleatorio(s->m_entre_entradas) + t);
		do {
			no->inf->t_entrada = t;
			gerar_cliente(no, h);
			ptNOEVENTO eventoe = alocar_noevento(ENTRADA_CLIENTE, no->inf->t_entrada), eventof = alocar_noevento(ENTRADA_CLIENTE_FILA, no->inf->t_entrada_fila);
			inserir_evento_ordenado(s, eventoe);
			inserir_evento_ordenado(s, eventof);
			if (no->inf->t_entrada >= s->t_total) {
				return;
			}
			no = no->prox;
			clientes_mesmo_tempo--;
		} while (clientes_mesmo_tempo && no->prox);
	} while (no->prox);
}
void abrir_caixa(ptHIPERMERCADO h, ptSIMULACAO s) {
	/*ABRE UMA CAIXA ALEATORIA DE ENTRE O TOTAL
	SE NÃO ENCONTRAR NENHUMA QUER DIZER QUE TODAS JÁ ESTÃO ABERTAS*/
	if (!h || !s)
		return;
	int i = 0, conta = 500;
	int stop = 1;
	do {
		i = num_aleatorio(s->n_caixas);
		if (h->caixas[i].estado == 0 && !h->caixas[i].espera_cabeca)
			stop = 0;
		conta--;
	} while (stop && conta);
	if (!conta)
		return;
	h->caixas[i].estado = 1;
	h->caixas[i].x_abriu++;
	ptNOFUNCIONARIO f = funcinario_para_caixa(h);
	f->inf->caixa_atual = i;
	h->caixas[i].funcionario = f;
}
void fechar_caixa(ptHIPERMERCADO h, int caixa) {
	/*FECHA A CAIXA ESTADO PASSA A 0
	E O FUNCIONARIO É RETIRADO DA CAIXA E INSERIDO NA LISTA POR ORDEM DECRESCENTE DO TEMPO DE CAIXA*/
	if (!h)
		return;
	h->caixas[caixa].estado = 0;
	ptNOFUNCIONARIO f = h->caixas[caixa].funcionario;
	h->caixas[caixa].funcionario = NULL;
	inserir_funcionario_ordenado(h, f);
}
int verificar_caixa_menor_fila(ptHIPERMERCADO h, ptSIMULACAO s) {
	/*VERIFICA A CAIXA QUE TEM A MENOR FILA
	SE NAO HOUVER CAIXAS ABERTAS COM UMA FILA MENOR QUE A INDICADA NA CONFIGURACAO DA SIMULACAO É DEVOLVIDO -1*/
	if (!h || !s)
		return -1;
	int caixa_menor_fila = -1, tamfila_act, tamfila_menor = INT_MAX;
	for (int i = 0; i < s->n_caixas; i++) {
		if (h->caixas[i].estado == 1) {
			tamfila_act = contafila(h->caixas[i].espera_cabeca);
			if (tamfila_act < tamfila_menor && tamfila_act<s->n_pessoas_fila) {
				caixa_menor_fila = i;
				tamfila_menor = tamfila_act;
			}
		}
	}
	return caixa_menor_fila;

}
int encontrar_caixa(ptHIPERMERCADO h, ptSIMULACAO s) {
	/*ENCONTRAR A CAIXA DE ONDE VAI SAIR O CLIENTE*/
	if (!h)
		return -1;
	int i = 0;
	for (; i < s->n_caixas; i++) {
		if (h->caixas[i].espera_cabeca && h->caixas[i].espera_cabeca->inf->t_saida == s->eventos->tempo)
			break;
	}
	return i;

}
void correr_simulacao(ptHIPERMERCADO h, ptSIMULACAO  s, int iteracoes, int show) {
	/*CORRER A SIMULACAO
	POR NUMERO DE ITERACOES E EVENTOS
	PODE MOSTRAR OU NAO*/
	double tempo = 0;
	while (s->eventos && iteracoes) {
		/*A SIMULACAO CORRE ENQUANTO HOUVER EVENTOS PARA MOSTRAR*/
		tempo = (s->eventos->tempo - s->t_avan);
		s->t_avan = s->eventos->tempo;
		if (show)
			mostrar(h, s);
		actualiza_t_espera(h, s, tempo);
		verificar(h, s);
		switch (s->eventos->tipo) {
		case 1:/*ENTRADA*/
		{
			entrar_cliente_hipermercado(h, s);
		}break;
		case 2:/*ENTRADA FILA*/
		{
			cliente_vai_para_fila(h, s);
		}break;
		case 3:/*SAIDA HIPERMERCADO*/
		{
			cliente_sai_hipermercado(h, s);
		}break;
		}
		remover_primeiro_evento(s);
		iteracoes--;
		guardahistorico(h, s);
	}
}
void actualiza_t_espera(ptHIPERMERCADO h, ptSIMULACAO s, double tempo) {
	/*ACTUALIZA OS TEMPOS DE ESPERA DOS CLIENTES ASSIM COMO OS TEMPO MEDIOS DE ESPERA DE CADA CAIXA E DO HIPERMERCADO*/
	if (!h)
		return;
	int k = 0;
	int j = 0, n_cliente = 0;
	double t_espera_caixa = 0;
	ptNOCLIENTE cab;
	for (int i = 0; i < s->n_caixas; i++) {
		cab = h->caixas[i].espera_cabeca;
		while (cab) {
			cab->inf->t_espera += tempo;
			t_espera_caixa += cab->inf->t_espera;
			k++;
			n_cliente++;
			cab = cab->prox;
		}
		if (h->caixas[i].espera_cabeca) {
			j++;
			h->caixas[i].t_espera = t_espera_caixa / k;
			h->t_espera += h->caixas[i].t_espera;
			h->caixas[i].funcionario->inf->t_de_caixa += tempo;
		}
		else
			h->caixas[i].t_espera = 0;
		if (!h->caixas[i].espera_cabeca && h->caixas[i].estado == 1)
			h->caixas[i].funcionario->inf->t_de_caixa += tempo;
		k = 0;
		t_espera_caixa = 0;
	}
	if (j) {
		h->t_espera = h->t_espera / j;
		h->clientes_por_fila = n_cliente / j;
	}
	else
	{
		h->t_espera = 0;
		h->clientes_por_fila = 0;
	}
}
void verificar(ptHIPERMERCADO h, ptSIMULACAO s) {
	/*VERIFICA SE E PRECISO ABRIR CAIXA OU FECHAR CAIXA*/
	if (!h || !s)
		return;
	int menor_fila = -1;
	if (h->t_espera > s->t_max_espera || h->clientes_por_fila>s->n_medio_fila || contafila(h->c_i_inicio)>s->n_pessoas_supermercado)
		abrir_caixa(h, s);
	if (h->clientes_por_fila < s->n_medio_fila) {
		for (int i = 0; i < s->n_caixas; i++) {
			if (contafila(h->caixas[i].espera_cabeca) < s->fechar_caixa_n_pessoas)
				menor_fila = i;
		}
		if (menor_fila < 0)
			return;
		else
			fechar_caixa(h, menor_fila);
	}
}
void calculo_tempos_finais(ptHIPERMERCADO h, ptSIMULACAO s) {
	/*CALCULO NO FIM DO TEMPO MEDIO DE ESPERA*/
	if (!h || !s)
		return;
	double t_espera = 0;
	int parcial = 0, total = 0;
	ptNOCLIENTE c;
	for (int i = 0; i < s->n_caixas; i++) {
		c = h->caixas[i].atendidos;
		h->caixas[i].t_espera = 0;
		while (c) {
			h->caixas[i].t_espera += c->inf->t_espera;
			t_espera += c->inf->t_espera;
			c = c->prox;
			total++;
		}
		h->caixas[i].t_espera = (h->caixas[i].t_espera / contafila(h->caixas[i].atendidos));
		parcial = 0;
	}
	h->t_espera = t_espera / total;
}