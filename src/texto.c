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

#define DELIMITADOR_PALAVRAS "\t\r\n"
char** separa_por_campo(char* linha, int n_campos, char *separadores) {
	/*FUNCAO QUE RECEBE UMA LINHA E SEPARA ,O N_CAMPOS E O NUMERO DE CAMPOS ESPERADOS APOS SEPARACAO*/
	char** v = (char**)malloc(sizeof(char*)*n_campos);
	if (!v)
		return NULL;
	char *campo;
	int i = 0;
	campo = strtok(linha, separadores);
	while (campo) {
		if (campo) {
			v[i] = (char*)malloc((strlen(campo) + 1) * sizeof(char));
			if (v[i])
				strcpy(v[i], campo);
		}
		campo = strtok(NULL, separadores);
		i++;
	}
	if (i != n_campos) {
		for (int j = 0; j < i; j++) {
			if (v[j])
				free(v[j]);
		}
		free(v);
		return NULL;
	}
	return v;
}

void lerficheiros(ptHIPERMERCADO h) {
	/*FUNCAO RESPONSAVEL PELA LEITURA DOS FICHEIROS*/
	FILE *f_clientes = fopen("clientes.txt", "r");
	if (!f_clientes)
		return;
	char linha[400];
	while (!feof(f_clientes)) {
		ptNOCLIENTE nocliente = alocar_nocliente();
		fgets(linha, 400, f_clientes);
		char **v = separa_por_campo(linha, 2, DELIMITADOR_PALAVRAS);
		if (v) {
			nocliente->inf->ID = v[0];
			nocliente->inf->nome = v[1];
			inserir_cliente_fim(&(h->c_o_inicio), nocliente);
			h->c_lidos++;
			free(v);
		}
		else
			free_nocliente(nocliente);
	}
	fclose(f_clientes);
	FILE *f_funcionarios = fopen("funcionarios.txt", "r");
	if (!f_funcionarios)
		return;
	while (!feof(f_funcionarios)) {
		ptNOFUNCIONARIO nofuncionario = alocar_nofuncionario();
		fgets(linha, 400, f_funcionarios);
		char **v = separa_por_campo(linha, 2, DELIMITADOR_PALAVRAS);
		if (v) {
			nofuncionario->inf->numero = v[0];
			nofuncionario->inf->nome = v[1];
			h->f_lidos++;
			free(v);
			inserir_funcionario_ordenado(h, nofuncionario);
		}
		else
			free_nofuncionario(nofuncionario);
	}
	fclose(f_funcionarios);
	FILE *f_produtos = fopen("produtos.txt", "r");
	if (!f_produtos)
		return;
	while (!feof(f_produtos)) {
		ptNOPRODUTO noproduto = alocar_noproduto();
		fgets(linha, 400, f_produtos);
		char **v = separa_por_campo(linha, 5, DELIMITADOR_PALAVRAS);
		if (v) {
			noproduto->inf->codigo = v[0];
			noproduto->inf->nome = v[1];
			noproduto->inf->preco = atof(v[2]);
			noproduto->inf->t_compra = atof(v[3]);
			noproduto->inf->t_caixa = atof(v[4]);
			if (noproduto->inf->t_compra == 0.0 || noproduto->inf->t_caixa == 0.0)
				free_noproduto(noproduto);
			else {
				if (h->p_inicio == NULL)
					h->p_inicio = noproduto;
				else {
					noproduto->prox = h->p_inicio;
					h->p_inicio = noproduto;
				}
				h->p_lidos++;
			}
			free(v);
		}
		else
			free_noproduto(noproduto);
	}
	fclose(f_produtos);
}
void guardahistorico(ptHIPERMERCADO h, ptSIMULACAO s) {
	/*GUARDA O HISTORICO TOTAL DA SIMULACAO ATUAL*/
	if (!h || !s)
		return;
	FILE *f_historico_sim_act = fopen("historico.csv", "a");
	if (!f_historico_sim_act)
		return;
	int atendidos = 0, p_vendidos = 0;
	for (int i = 0; i < s->n_caixas; i++) {
		atendidos = atendidos + contafila(h->caixas[i].atendidos);
		p_vendidos += h->caixas[i].p_vendidos;
	}
	fprintf(f_historico_sim_act, "%lf\t%d\t%d\t%lf\t%d\n", s->t_avan, atendidos, p_vendidos, h->t_espera, contafila(h->c_i_inicio));
	fclose(f_historico_sim_act);
}
void guardar_dados(ptHIPERMERCADO h, ptSIMULACAO s) {
	/*GUARDA DADOS DESTA SIMULACAO EM TXT*/
	if (!h || !s)
		return;
	FILE *f_sims = fopen("historico.txt", "a");
	if (!f_sims)
		return;
	fprintf(f_sims, "\nCONFIGURAÇOES DA SIMULACAO:");
	fprintf(f_sims, "\nTEMPO=%lf\tNUMERO DE CAIXAS:%d\tNUMERO MAXIMO DE PESSOAS FILA:%d\tABRIR CAIXA QUANDO NUMERO DE PESSOAS POR FILA>%d\tABRIR CAIXA QUANDO NUMERO DE PESSOAS NO HIPERMERCADO>%d\tTEMPO MAXIMO DE ESPERA:%lf", s->t_total, s->n_caixas, s->n_pessoas_fila, s->n_medio_fila, s->n_pessoas_supermercado, s->t_max_espera);
	for (int i = 0; i < s->n_caixas; i++) {
		fprintf(f_sims, "\nCAIXA:%d\tVALOR PRODUTOS VENDIDOS:%lf\tPRODUTOS VENDIDOS:%d\tVALOR OFERECIDO:%lf\tPRODUTOS OFERECIDOS:%d\tTEMPO MEDIO DE ESPERA:%lf\tCLIENTES ATENDIDOS:%d", i, h->caixas[i].valor, h->caixas[i].p_vendidos, h->caixas[i].valor_oferecido, h->caixas[i].p_oferecidos, h->caixas[i].t_espera, contafila(h->caixas[i].atendidos));
	}
	fprintf(f_sims, "\nTEMPO DE ESPERA MEDIO:%lf", h->t_espera);
	fprintf(f_sims, "\n------------------------------------------------------------------------------------------");
	fclose(f_sims);
}