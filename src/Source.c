
#include "texto.h"
#include "sim.h"
#include "cli.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
int main() {
	/*MAIN*/
	srand((unsigned int)time(NULL));
	ptHIPERMERCADO hipermercado = alocar_hipermercado();
	lerficheiros(hipermercado);
	ptSIMULACAO simulacao = alocar_simulacao();
	configurarsimulacao(simulacao);
	gerar_clientes(simulacao, hipermercado);
	hipermercado->caixas = alocar_caixas(simulacao->n_caixas);
	sim_utilizador(hipermercado, simulacao);
	desempenho_sistema(hipermercado, simulacao);
	Listar_clientes(hipermercado, simulacao);
	calculo_tempos_finais(hipermercado, simulacao);
	guardar_dados(hipermercado, simulacao);
	libertar_mem_total(hipermercado, simulacao);
}
