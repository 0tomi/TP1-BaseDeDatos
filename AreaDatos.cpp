#ifndef AREA_DATOS_CPP
#define AREA_DATOS_CPP

#include "AreaDatos.h"

AreaDatos::AreaDatos(int ELM_POR_BLOQ, int OMAX, int PMAX){
    this->ELM_POR_BLOQ = ELM_POR_BLOQ;
    this->OMAX = OMAX;
    this->PMAX = PMAX;
    
    this->registros.reserve(OMAX);
}

#endif // AREA_DATOS_CPP