#pragma once
#include "alocar_free.h"
ptNOCLIENTE remover_cliente_fila_espera(CAIXA *c);
void atender_cliente(CAIXA *c, ptSIMULACAO s);
int contafila(ptNOCLIENTE inicio);
void inserir_cliente_fila_espera(CAIXA *c, ptNOCLIENTE inserir);
void cliente_vai_para_fila(ptHIPERMERCADO h, ptSIMULACAO s);
void cliente_sai_hipermercado(ptHIPERMERCADO h, ptSIMULACAO s);
ptNOCLIENTE retirar_cliente_cauda(CAIXA *c);
void mudar_cliente_caixa(ptHIPERMERCADO h, ptSIMULACAO s, int caixa);