#include "AreaDatos.h"
#include "Registro.h"

AreaDatos::AreaDatos(int ELM_POR_BLOQ, int OMAX, int PMAX){
    this->ELM_POR_BLOQ = ELM_POR_BLOQ;
    this->OMAX = OMAX;
    this->PMAX = PMAX;
    registros = new Registro[OMAX];
}

AreaDatos::~AreaDatos(){
    delete[] registros;
}