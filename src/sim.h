#pragma once
#include "alocar_free.h"
void gerar_cliente(ptNOCLIENTE nc, ptHIPERMERCADO h);
void gerar_clientes(ptSIMULACAO s, ptHIPERMERCADO h);
void abrir_caixa(ptHIPERMERCADO h, ptSIMULACAO s);
void fechar_caixa(ptHIPERMERCADO h, int caixa);
int verificar_caixa_menor_fila(ptHIPERMERCADO h, ptSIMULACAO s);
int encontrar_caixa(ptHIPERMERCADO h, ptSIMULACAO s);
void correr_simulacao(ptHIPERMERCADO h, ptSIMULACAO  s, int iteracoes, int show);
void actualiza_t_espera(ptHIPERMERCADO h, ptSIMULACAO s, double tempo);
void verificar(ptHIPERMERCADO h, ptSIMULACAO s);
void calculo_tempos_finais(ptHIPERMERCADO h, ptSIMULACAO s);