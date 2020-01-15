#pragma once
#include "alocar_free.h"

char** separa_por_campo(char* linha, int n_campos, char *separadores);
void lerficheiros(ptHIPERMERCADO h);
void guardahistorico(ptHIPERMERCADO h, ptSIMULACAO s);
void guardar_dados(ptHIPERMERCADO h, ptSIMULACAO s);
