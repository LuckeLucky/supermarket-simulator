#pragma once
#include "alocar_free.h"
ptPRODUTO encontra_produto(ptHIPERMERCADO h, int i);
void inserir_evento_ordenado(ptSIMULACAO s, ptNOEVENTO e);
ptNOCLIENTE retirar_cliente_inicio(ptNOCLIENTE *inicio);
void entrar_cliente_hipermercado(ptHIPERMERCADO h, ptSIMULACAO s);
void inserir_cliente_ordenado(ptHIPERMERCADO h, ptNOCLIENTE c);
void inserir_cliente_fim(ptNOCLIENTE *inicio, ptNOCLIENTE inserir);
void remover_primeiro_evento(ptSIMULACAO s);
ptNOFUNCIONARIO funcinario_para_caixa(ptHIPERMERCADO h);
void inserir_funcionario_ordenado(ptHIPERMERCADO h, ptNOFUNCIONARIO f);
void pesquisar(ptHIPERMERCADO h, ptSIMULACAO s);
ptNOCLIENTE encontra(ptNOCLIENTE inicio, char *ID);