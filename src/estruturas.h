#pragma once

typedef struct noeventos {
	int tipo;
	double tempo;
	struct noeventos *prox;
}*ptNOEVENTO;
typedef struct simulacao {
	double t_total, t_avan;
	int  n_caixas, n_pessoas_fila, n_medio_fila, fechar_caixa_n_pessoas, n_pessoas_supermercado, gerar_ao_mesmo_tempo, m_entre_entradas;
	double t_max_espera;
	ptNOEVENTO eventos;
}*ptSIMULACAO;
typedef struct cliente {
	char *ID, *nome;
	double t_entrada, t_entrada_fila, t_espera;
	int n_produtos;
	double preco, produto_preco_alto, t_saida, t_caixa;
}*ptCLIENTE;
typedef struct funcionario {
	char *numero, *nome;
	double t_de_caixa;
	unsigned int caixa_atual, c_atendidos;
}*ptFUNCIONARIO;
typedef struct produto {
	char *codigo, *nome;
	double preco, t_compra, t_caixa;
}*ptPRODUTO;
typedef struct nocliente {
	struct nocliente *prox;
	ptCLIENTE inf;
}*ptNOCLIENTE;
typedef struct nofuncionario {
	struct nofuncionario *prox;
	ptFUNCIONARIO inf;
}*ptNOFUNCIONARIO;
typedef struct noproduto {
	struct noproduto *prox;
	ptPRODUTO inf;
}*ptNOPRODUTO;
typedef struct caixa {
	unsigned int  p_vendidos, estado, x_abriu, p_oferecidos;
	double t_espera, valor, valor_oferecido;
	ptNOCLIENTE espera_cabeca, espera_cauda, atendidos;
	ptNOFUNCIONARIO funcionario;
}CAIXA;
typedef struct hipermercado {
	ptNOCLIENTE c_o_inicio, c_i_inicio;
	ptNOFUNCIONARIO f_inicio;
	ptNOPRODUTO p_inicio;
	CAIXA *caixas;
	double t_espera, clientes_por_fila;
	unsigned int c_lidos, f_lidos, p_lidos;
}*ptHIPERMERCADO;
