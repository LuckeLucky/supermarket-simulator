#pragma once
#include "estruturas.h"

ptHIPERMERCADO alocar_hipermercado();
ptNOCLIENTE alocar_nocliente();
ptNOFUNCIONARIO alocar_nofuncionario();
ptNOPRODUTO alocar_noproduto();
CAIXA* alocar_caixas(int n);
ptSIMULACAO alocar_simulacao();
ptNOEVENTO alocar_noevento(int evento, double tempo);
void free_nocliente(ptNOCLIENTE c);
void free_nofuncionario(ptNOFUNCIONARIO f);
void free_noproduto(ptNOPRODUTO p);
void libertar_mem_total(ptHIPERMERCADO h, ptSIMULACAO s);