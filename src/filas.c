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
ptNOCLIENTE remover_cliente_fila_espera(CAIXA *c) {
	/*REMOVER CLIENTE DA FILA DE ESPERA
	SIGNIFICA TIRAR O CLIENTE QUE ESTA NA CABECA DA FILA SENDO ASSIM O CLIENTE ANTES PASSA A SER A CABECA*/
	if (!c)
		return NULL;
	ptNOCLIENTE cabeca = c->espera_cabeca;
	if (cabeca) {
		if (c->espera_cabeca == c->espera_cauda)
			c->espera_cabeca = c->espera_cauda = NULL;
		else
			c->espera_cabeca = c->espera_cabeca->prox;
	}
	return cabeca;
}
void atender_cliente(CAIXA *c, ptSIMULACAO s) {
	/*CLIENTE E TIRADO DA FILA DE ESPERA DA CAIXA
	O SEU TEMPO DE ESPERA E COMPARADO COM O TEMPO MAX DE ESPERA DA SIMULACAO DE FORMA A SABERMOS SE E NECESSARIO OU NAO OFERECER UM PRODUTO
	APOS ISSO O CLIENTE E INSERIDO NO FIM DA LISTA DE ATENDIDOS DA CAIXA*/
	if (!c)
		return;
	ptNOCLIENTE cliente = remover_cliente_fila_espera(c);
	if (cliente) {
		cliente->prox = NULL;
		if ((cliente->inf->t_espera - cliente->inf->t_caixa) >= s->t_max_espera) {
			cliente->inf->preco = cliente->inf->preco - cliente->inf->produto_preco_alto;
			c->p_oferecidos++;
			c->valor_oferecido += cliente->inf->produto_preco_alto;
			c->p_vendidos += (cliente->inf->n_produtos - 1);
		}
		else
			c->p_vendidos += cliente->inf->n_produtos;
		c->valor += cliente->inf->preco;
		c->funcionario->inf->c_atendidos++;
		inserir_cliente_fim(&(c->atendidos), cliente);
	}
	else
		return;
}
int contafila(ptNOCLIENTE inicio) {
	/*DEVOLVE O COMPRIMENTO DE UMA FILA*/
	int i = 0;
	ptNOCLIENTE conta = inicio;
	while (conta) {
		i++;
		conta = conta->prox;
	}
	return i;
}
void inserir_cliente_fila_espera(CAIXA *c, ptNOCLIENTE inserir) {
	/*INSERIR CLIENTE NA FILA DE ESPERA
	INSERIR CLIENTE NA CAUDA*/
	if (!c || !inserir)
		return;
	if (c->espera_cabeca == NULL)
		c->espera_cabeca = c->espera_cauda = inserir;
	else {
		c->espera_cauda->prox = inserir;
		c->espera_cauda = inserir;
	}

}
void cliente_vai_para_fila(ptHIPERMERCADO h, ptSIMULACAO s) {
	/*CLIENTE É TIRADO DO INICIO DE H->C_I_INICIO E INSERIDO EM UMA DAS CAIXAS COM MENOR FILA*/
	if (!h || !s)
		return;
	int caixa;
	ptNOEVENTO evento;
	caixa = verificar_caixa_menor_fila(h, s);
	if (caixa < 0) {
		abrir_caixa(h, s);
		caixa = verificar_caixa_menor_fila(h, s);
		if (caixa == -1) {
			caixa = num_aleatorio(s->n_caixas);
		}
	}
	ptNOCLIENTE cliente_espera = retirar_cliente_inicio(&(h->c_i_inicio));

	if (cliente_espera->inf->n_produtos > 0) {
		inserir_cliente_fila_espera(&(h->caixas[caixa]), cliente_espera);
		if (h->caixas[caixa].espera_cabeca == cliente_espera) {
			cliente_espera->inf->t_saida = (cliente_espera->inf->t_entrada_fila + cliente_espera->inf->t_caixa);
			evento = alocar_noevento(SAIDA_CLIENTE, cliente_espera->inf->t_saida);
			inserir_evento_ordenado(s, evento);
		}
	}
	else
		free_nocliente(cliente_espera);
}
void cliente_sai_hipermercado(ptHIPERMERCADO h, ptSIMULACAO s) {
		/*CLIENTE SAI DA CAIXA(CLIENTE É TIRADO DA CABECA
		APOS ISSO VERIFICA-SE SE HÁ MAIS CLIENTES NA FILA OU NÃO
		CASO HAJA E ATRIBUIDO O TEMPO DE SAIDA A QUEM ESTA NA CABECA
		CASO CONTRARIO A CAIXA E FECHADA*/
		if (!h || !s)
			return;
		int caixa;
		caixa = encontrar_caixa(h, s);
		if (caixa < 0 || caixa == s->n_caixas)
			return;
		atender_cliente(&(h->caixas[caixa]), s);
		if (h->caixas[caixa].espera_cabeca == NULL)
			fechar_caixa(h, caixa);
		else {
			h->caixas[caixa].espera_cabeca->inf->t_saida = (s->t_avan + h->caixas[caixa].espera_cabeca->inf->t_caixa);
			ptNOEVENTO evento = alocar_noevento(SAIDA_CLIENTE, h->caixas[caixa].espera_cabeca->inf->t_saida);
			inserir_evento_ordenado(s, evento);
		}
}

ptNOCLIENTE retirar_cliente_cauda(CAIXA *c) {
	/*RETIRA O CLIENTE DA CAUDA PARA PERMITIR QUE ELE VA PARA OUTRA FILA*/
	if (!c)
		return NULL;
	ptNOCLIENTE ant, act;
	ant = NULL;
	act = c->espera_cabeca;
	while (act->prox) {

		ant = act;
		act = act->prox;
	}
	if (act == c->espera_cabeca) {
		c->espera_cabeca = c->espera_cauda = NULL;
	}
	if (ant) {
		ant->prox = NULL;
		c->espera_cauda = ant;
	}
	act->prox = NULL;
	return act;
}
void mudar_cliente_caixa(ptHIPERMERCADO h, ptSIMULACAO s, int caixa) {
	/*USA O RETIRAR CLIENTE DA CAUDA E APOS ISSO É ESCOLHIDA UMA NOVA CAIXA PARA O CLIENTE*/
	if (!h || !s)
		return;
	ptNOCLIENTE cliente;
	ptNOEVENTO evento;
	int caixa2;
	cliente = retirar_cliente_cauda(&(h->caixas[caixa]));
	if (cliente) {
		cliente->inf->t_entrada_fila = s->t_avan;
		cliente->inf->t_espera = 0;
		cliente->inf->t_saida = 0;
		if (h->caixas[caixa].espera_cabeca == NULL)
			fechar_caixa(h, caixa);
		caixa2 = verificar_caixa_menor_fila(h, s);
		if (caixa2 == caixa) {
			abrir_caixa(h, s);
			caixa2 = verificar_caixa_menor_fila(h, s);
		}
		evento = alocar_noevento(ENTRADA_CLIENTE_FILA, cliente->inf->t_entrada_fila);
		if (caixa2 < 0) {
			abrir_caixa(h, s);
			caixa2 = verificar_caixa_menor_fila(h, s);
			if (caixa2 == -1) {
				caixa2 = num_aleatorio(s->n_caixas);
			}
		}
		inserir_cliente_fila_espera(&(h->caixas[caixa2]), cliente);
		if (h->caixas[caixa2].espera_cabeca == cliente) {
			cliente->inf->t_saida = (cliente->inf->t_entrada_fila + cliente->inf->t_caixa);
			evento = alocar_noevento(SAIDA_CLIENTE, cliente->inf->t_saida);
			inserir_evento_ordenado(s, evento);
		}
	}

}