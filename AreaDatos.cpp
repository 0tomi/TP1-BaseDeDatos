#ifndef AREA_DATOS_CPP
#define AREA_DATOS_CPP

#include "AreaDatos.h"

AreaDatos::AreaDatos(int ELM_POR_BLOQ, int OMAX, int PMAX){
    this->ELM_POR_BLOQ = ELM_POR_BLOQ;
    this->OMAX = OMAX;
    this->PMAX = PMAX;
    
    this->registros.reserve(OMAX);
}

int AreaDatos::insertarRegistro(int pos, int clave, string dato)
{
    /*  Casos posibles:
        0: No hay bloques.
            0.1: Se procede a crear un nuevo bloque e insertar el elemento.
                Retorna: AreaDatos::NuevoBloqueCreado

        2: Insercion en bloque con espacio.
            2.a: Insercion en algun bloque anterior al ultimo. 
                2.a.1: Se inserta el elemento. 
                    Retorna: AreaDatos::InsercionIntermedia
            2.b: Insercion en el ultimo bloque.
                2.b.a: Contiene una cantidad de registros menor a n/2. 
                    2.b.a.1: Se inserta el elemento.
                        Retorna: AreaDatos::InsercionIntermedia
                2.b.b: Contiene cantidad de registros mayor o igual a n/2.
                    2.b.b.1: Se crea nuevo bloque. 
                        2.b.b.a: El bloque creado es el ultimo que se puede poner en el area primaria.
                            2.b.b.a.1: Se inserta la clave. 
                            2.b.b.a.2: Se retorna AreaDatos::AreaPrimariaLlena
                        2.b.b.b: Hay espacio para mas bloques despues del insertado.
                            2.b.b.b.1: Se inserta el elemento.
                                Retorna: AreaDatos::NuevoBloqueCreado.

        3: Insercion en bloque lleno. 
            3.a: Hay espacio en el overflow.
                3.a.1: Se busca la clave menor mas cercana a la clave que vamos a insertar.
                3.a.2: Se va a la direccion que contiene la clave menor. 
                3.a.2.a: No apunta a ningun lado:
                    3.a.2.a.1: Se coloca el dato y la clave donde haya espacio en el overflow.
                    3.a.2.a.2: Se coloca la direccion del dato en la direccion del overflow de la clave menor. 
                        Retorna: AreaDatos::InsercionIntermedia
                3.a.2.b: La clave menor contiene una direccion. 
                    3.a.2.b.1: Nos movemos mediante las direcciones que contengan las claves posteriores al overflow. 
                    3.a.2.b.2: Se ubica una clave que no apunte a ningun lado.
                    3.a.2.b.3: Se ubica el registro en el overflow. 
                    3.a.2.b.4: Se coloca la direccion de ese registro en la clave previamente encontrada (dentro del overflow).
                        Retorna: AreaDatos::InsercionIntermedia.
            3.b: Queda UN espacio en el overflow.
                3.b.1: Se inserta la clave. 
                3.b.2: Se retorna Area::OverflowLleno

        Codigos: 
        1: Insercion usando nuevo bloque
        2: Insercion con overflow lleno
        3: Insercion con area primaria llena
    */
   int lugarInsercion = 0;

   if (!CANTIDAD_BLOQUES) {
        this->crearBloque(0);
        this->registros[0] = {clave, dato, 0}; // Por default 0 indica que no apunta al overflow.
        return 1;
    }

    if (!this->isLastBlock(pos)){
        if (!this->isBlockFull(pos)) {

        } // caso bloque lleno


    } 

    // caso ultimo bloque
    // falta manejar el caso de sin espacio para insertar nuevos bloques   



}

void AreaDatos::crearBloque(int pos)
{
    this->CANTIDAD_BLOQUES++;
    this->ultimoBloqueInsertado = pos;
}

bool AreaDatos::isBlockFull(int pos)
{
    for (int x = pos; x < pos+this->ELM_POR_BLOQ; x++)
        if (this->registros[x].clave == 0) return false;
    return true;
}

bool AreaDatos::isLastBlock(int pos)
{
    return pos == this->ultimoBloqueInsertado;
}

#endif // AREA_DATOS_CPP