#include "alocar_free.h"
#include "cli.h"
#include "filas.h"
#include "list.h"
#include "sim.h"
#include "texto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
void configurarsimulacao(ptSIMULACAO sim) {
	/*CONFIGURACOES DA SIMULACAO*/
	printf("CONFIGURAR SIMULACAO");
	printf("\nDuracao da simulacao:");
	int horas, minutos;
	do {
		do {
			printf("\nHoras(<=24):");
			scanf("%d", &horas);
		} while (horas > 24 && horas<0);
		do {
			printf("\nMinutos(<=60):");
			scanf("%d", &minutos);
		} while (minutos > 60 && minutos < 0);

	} while (horas * 60 * 60 + minutos * 60 > 86400);
	sim->t_total = (double)(horas * 60 * 60 + minutos * 60);
	sim->t_avan = 0.0;
	printf("\nNumero de Caixas:");
	scanf("%d", &(sim->n_caixas));
	printf("\nTempo de espera maximo do cliente(minutos):");
	scanf("%lf", &(sim->t_max_espera));
	sim->t_max_espera = sim->t_max_espera * 60;
	printf("\nNumero Maximo de pessoas por fila(caixa):");
	scanf("%d", &(sim->n_pessoas_fila));;
	printf("\nAbrir|Fechar caixa quando o numero medio de pessoas por caixa for inferior ou superior a:");
	scanf("%d", &(sim->n_medio_fila));;
	printf("\nAbrir Caixa quando o numero de pessoas no hipermercado for maior que:");
	scanf("%d", &(sim->n_pessoas_supermercado));
	printf("\nPARA GERAR CLIENTES");
	printf("\nPodem entrar ao mesmo tempo quantos clientes?:");
	scanf("%d", &(sim->gerar_ao_mesmo_tempo));
	printf("\nTempo medio entre cada entrada(segundos)");
	scanf("%d", &(sim->m_entre_entradas));
	system("cls");

}
int num_aleatorio(int max) {
	/*GERA UM NUMERO ALEATORIO ENTRE 0 E MAX-1*/
	return rand() % max;
}
int menusim() {
	/*MENU QUE E USADO ENQUANTO CORRE A SIMULACAO*/
	int op;
	do {
		printf("\n1-Abrir Caixa");
		printf("\n2-Fechar Caixa e Clientes sao atendidos");
		printf("\n3-Cliente Muda de Caixa");
		printf("\n4-Caixa Fecha e Clientes sao distribuidos");
		printf("\n5-Pesquisar");
		printf("\n6-Correr 1evento");
		printf("\n7-Correr 10eventos");
		printf("\n8-Correr 100eventos");
		printf("\n9-Correr 200eventos");
		printf("\n10-Correr todos os eventos e mostrar");
		printf("\n11-Correr todos os eventos sem mostrar");
		printf("\n0-Acabar Simulacao");
		printf("\nOPCAO:");
		fseek(stdin, 0, SEEK_END);
		scanf("%d", &op);
		fseek(stdin, 0, SEEK_END);
	} while (op<1 && op>11);
	return op;
}
void mostrar(ptHIPERMERCADO h, ptSIMULACAO s) {
	/*MOSTRA AS CAIXAS E OS CLIENTES NAS SUAS CAIXAS*/
	if (!h)
		return;
	ptNOCLIENTE cab;
	system("cls");
	printf("%lf\t t_espera=%lf\tClientes Hipermercado:%d", s->t_avan, h->t_espera, contafila(h->c_i_inicio));
	for (int i = 0; i < s->n_caixas; i++) {
		printf("\n[(%d)caixa:%d\tTempo Medio de espera=%lf]\n", h->caixas[i].estado, i, h->caixas[i].t_espera);
		cab = h->caixas[i].espera_cabeca;
		while (cab) {
			printf("(%s) ", cab->inf->ID);
			cab = cab->prox;
		}
		printf("\n");
	}
}
void sim_utilizador(ptHIPERMERCADO h, ptSIMULACAO s) {
	/*SIMULACAO UTILIZA O MENUSIM PARA INTERAGIR COM O UTILIZADOR*/
	if (!h || !s)
		return;
	int op;
	int caixa;
	do {
		op = menusim();
		switch (op) {
		case 1: {
			abrir_caixa(h, s); printf("Caixa Aberta"); mostrar(h, s);
		}break;
		case 2: {
			do {
				printf("\nQual a Caixa que deseja Fechar?:");
				scanf("%d", &caixa);
			} while (caixa<0 || caixa >= s->n_caixas || h->caixas[caixa].estado == 0);
			h->caixas[caixa].estado = 0;
			mostrar(h, s);
		}break;
		case 3: {
			if (verificar_caixa_menor_fila < 0)
				break;
			do {
				printf("\nQual a Caixa do Cliente?:");
				scanf("%d", &caixa);
			} while (caixa<0 || caixa >= s->n_caixas || h->caixas[caixa].estado == 0);
			mudar_cliente_caixa(h, s, caixa);
			mostrar(h, s);
		}break;
		case 4: {
			if (verificar_caixa_menor_fila < 0)
				break;
			do {
				printf("\nQual a Caixa a Fechar?:");
				scanf("%d", &caixa);
			} while (caixa<0 || caixa >= s->n_caixas || h->caixas[caixa].estado == 0);
			int tam_fila = contafila(h->caixas[caixa].espera_cabeca);
			h->caixas[caixa].estado = 0;
			for (int i = 0; i < tam_fila; i++)
				mudar_cliente_caixa(h, s, caixa);
			mostrar(h, s);
		}break;
		case 5:pesquisar(h, s); break;
		case 6:correr_simulacao(h, s, 1, 1); break;
		case 7:correr_simulacao(h, s, 10, 1); break;
		case 8:correr_simulacao(h, s, 100, 1); break;
		case 9:correr_simulacao(h, s, 200, 1); break;
		case 10:correr_simulacao(h, s, INT_MAX, 1); return;
		case 11:correr_simulacao(h, s, INT_MAX, 0); return;
		case 0:return;
		}
	} while (op > 0 && op<12);
}
void Listar_clientes(ptHIPERMERCADO h, ptSIMULACAO s) {
	/*LISTA OS CLIENTES ATENDIDOS DE UMA CERTA CAIXA*/
	if (!h || !s) {
		return;
	}
	int caixa, stop = 1;
	while (stop) {
		do {
			printf("\nVer Clientes Atendidos da Caixa?:");
			scanf("%d", &caixa);
		} while ((caixa<0 || caixa >= s->n_caixas));
		ptNOCLIENTE x = h->caixas[caixa].atendidos;
		while (x) {
			printf("\n%s\t%s\t%d\t%lf\t%lf\t%lf", x->inf->ID, x->inf->nome, x->inf->n_produtos, x->inf->t_entrada_fila, x->inf->t_espera, x->inf->t_saida);
			x = x->prox;
		}
		printf("Deseja Ver clientes atendidos de outra caixa? 0(nao) 1(sim):");
		scanf("%d", &stop);
	}

}
void desempenho_sistema(ptHIPERMERCADO h, ptSIMULACAO s) {

	if (!h || !s)
		return;
	int caixa_mais = 0;
	for (int i = 0; i < s->n_caixas; i++) {
		if (contafila(h->caixas[i].atendidos) > contafila(h->caixas[caixa_mais].atendidos))
			caixa_mais = i;
	}
	printf("\nA Caixa que atendeu mais clientes foi a %d com %d clientes", caixa_mais, contafila(h->caixas[caixa_mais].atendidos));
	caixa_mais = 0;
	for (int i = 0; i < s->n_caixas; i++) {
		if (h->caixas[i].p_vendidos > h->caixas[caixa_mais].p_vendidos)
			caixa_mais = i;
	}
	printf("\nA Caixa que vendeu mais produtos foi a %d com %d produtos", caixa_mais, h->caixas[caixa_mais].p_vendidos);
	caixa_mais = 0;
	for (int i = 0; i < s->n_caixas; i++) {
		if (h->caixas[i].valor > h->caixas[caixa_mais].valor)
			caixa_mais = i;
	}
	printf("\nA Caixa que fez mais dinheiro foi a %d com %lf produtos", caixa_mais, h->caixas[caixa_mais].valor);
	caixa_mais = 0;
	for (int i = 0; i < s->n_caixas; i++) {
		if (h->caixas[i].p_oferecidos > h->caixas[caixa_mais].p_oferecidos)
			caixa_mais = i;
	}
	printf("\nA Caixa que ofereceu mais produtos foi a %d com %d produtos", caixa_mais, h->caixas[caixa_mais].p_oferecidos);
	caixa_mais = 0;
	for (int i = 0; i < s->n_caixas; i++) {
		if (contafila(h->caixas[i].atendidos) < contafila(h->caixas[caixa_mais].atendidos))
			caixa_mais = i;
	}
	printf("\nA Caixa que atendeu menos clientes foi a %d com %d clientes", caixa_mais, contafila(h->caixas[caixa_mais].atendidos));
	int p_oferecidos = 0;
	double valor = 0;
	for (int i = 0; i < s->n_caixas; i++) {
		p_oferecidos += h->caixas[i].p_oferecidos;
		valor += h->caixas[i].valor_oferecido;
	}
	printf("\nNo Total ofereceram-se %d produtos com o custo de %lf", p_oferecidos, valor);
	ptNOFUNCIONARIO f = h->f_inicio, guarda_f = h->f_inicio;
	while (f) {
		if (f->inf->c_atendidos < guarda_f->inf->c_atendidos)
			guarda_f = f;
		f = f->prox;
	}
	printf("\nFuniconario(a):%s atendeu apenas %d clientes", guarda_f->inf->nome, guarda_f->inf->c_atendidos);
	f = h->f_inicio, guarda_f = h->f_inicio;
	while (f) {
		if (f->inf->c_atendidos > guarda_f->inf->c_atendidos)
			guarda_f = f;
		f = f->prox;
	}
	printf("\nFuniconario(a):%s atendeu  %d clientes!!", guarda_f->inf->nome, guarda_f->inf->c_atendidos);
}