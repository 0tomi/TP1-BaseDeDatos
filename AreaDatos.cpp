#ifndef AREA_DATOS_CPP
#define AREA_DATOS_CPP

#include "AreaDatos.h"

AreaDatos::AreaDatos(int ELM_POR_BLOQ, int OMAX, int PMAX){
    this->ELM_POR_BLOQ = ELM_POR_BLOQ;
    this->OMAX = OMAX;
    this->PMAX = PMAX;
    
    this->registros.reserve(OMAX);
}

int AreaDatos::insertarRegistro(int clave, string dato)
{
    /*  Casos posibles:
        1: No bloques, creamos un bloque e insertamos el elemento.
        2: Insercion en un bloque creado.
        2.b: Inserccion en un bloque creado lleno.
        3: Insercion en el ultimo bloque estando n/2 lleno
        4: Insercion al final del overflow (sin espacio)
        5: Insercion genera el ultimo bloque disponible a crear en el area primaria.
    */
   if (!CANTIDAD_BLOQUES) {
        this->crearBloque(0);
        this->registros[0] = {clave, dato, 0}; // Por default 0 indica que no apunta al overflow.
        return 1;
    } 

}

void AreaDatos::crearBloque(int pos)
{
    this->CANTIDAD_BLOQUES++;
}

#endif // AREA_DATOS_CPP