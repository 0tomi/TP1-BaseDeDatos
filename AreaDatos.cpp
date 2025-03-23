#ifndef AREA_DATOS_CPP
#define AREA_DATOS_CPP

#include "AreaDatos.h"

ostream& operator<< (ostream& os, AreaDatos& areaDatos){
    int nroBloque = 0, x;
    os << "+------------------------------------------+" << endl;
    os << "| BLOQUE " << nroBloque << setw(34) << "|" << endl;
    os << "|------------------------------------------|" << endl;
    for(x = 0; x < areaDatos.OMAX; x++ ){

        if(x % areaDatos.ELM_POR_BLOQ == 0 && x != 0) {
            nroBloque++;
            os << "| BLOQUE " << nroBloque << endl;
            os << "|------------------------------------------|" << endl;
        }
        os << "| " << setw(5) << AreaDatos.registros[x].clave << " | " << setw(5) << AreaDatos.registros[x].dato << " |" << setw(5) << AreaDatos.registros[x].dir << endl;
        os << "|------------------------------------------|" << endl;
    }
    os << "+------------------------------------------+" << endl;
    return os;
}

AreaDatos::AreaDatos(int ELM_POR_BLOQ, int OMAX, int PMAX){
    this->ELM_POR_BLOQ = ELM_POR_BLOQ;
    this->OMAX = OMAX;
    this->PMAX = PMAX;
    
    this->registros.reserve(OMAX);
}

AreaDatos::Estado AreaDatos::insertar(int pos, int clave, string dato)
{
    /*  Casos posibles:
        0: No hay bloques.  [HECHO]
            0.1: Se procede a crear un nuevo bloque e insertar el elemento.
                Retorna: AreaDatos::NuevoBloqueCreado

        [Insercion en bloque anterior al ultimo]
        1: Insercion en bloque con espacio. [HECHO]
            1.1: Se inserta el elemento. 
                Retorna: AreaDatos::InsercionIntermedia

        2: Insercion en bloque lleno.
            2.a: Hay espacio en el overflow.    [HECHO]
                2.a.1: Se busca la clave menor mas cercana a la clave que vamos a insertar.
                2.a.2: Se va a la direccion que contiene la clave menor. 
                2.a.2.a: No apunta a ningun lado:   [HECHO]
                    2.a.2.a.1: Se coloca el dato y la clave donde haya espacio en el overflow.
                    2.a.2.a.2: Se coloca la direccion del dato en la direccion del overflow de la clave menor. 
                        Retorna: AreaDatos::InsercionIntermedia
                2.a.2.b: La clave menor contiene una direccion. [HECHO]
                    2.a.2.b.1: Nos movemos mediante las direcciones que contengan las claves posteriores al overflow. 
                    2.a.2.b.2: Se ubica una clave que no apunte a ningun lado.
                    2.a.2.b.3: Se ubica el registro en el overflow. 
                    2.a.2.b.4: Se coloca la direccion de ese registro en la clave previamente encontrada (dentro del overflow).
                        Retorna: AreaDatos::InsercionIntermedia.
            2.b: Queda UN espacio en el overflow.   [HECHO]
                2.b.1: Se inserta la clave. 
                2.b.2: Se retorna Area::OverflowLleno
            2.c: No hay espacio en el overflow.     [HECHO]
                2.c.1: Se retorna el estado de Overflow lleno.
        
        [3: Insercion en el ultimo bloque.]   [FALTA]
            3.a: Contiene una cantidad de registros menor a n/2.  [FALTA]
                3.a.1: Se inserta el elemento.
                    Retorna: AreaDatos::InsercionIntermedia
            3.b: Contiene cantidad de registros mayor o igual a n/2.  [FALTA]
                3.b.1: Se crea nuevo bloque. 
                    3.b.a: El bloque creado es el ultimo que se puede poner en el area primaria.  [FALTA]
                        3.b.a.1: Se inserta la clave. 
                        3.b.a.2: Se retorna AreaDatos::AreaPrimariaLlena
                    3.b.b: Hay espacio para mas bloques despues del insertado.    [FALTA]
                        3.b.b.1: Se inserta el elemento.
                            Retorna: AreaDatos::NuevoBloqueCreado.

        Codigos: 
        1: Insercion usando nuevo bloque
        2: Insercion con overflow lleno
        3: Insercion con area primaria llena
    */

   if (!CANTIDAD_BLOQUES) {
        this->crearBloque(0);
        this->registros[0] = {clave, dato, 0}; // Por default 0 indica que no apunta al overflow.
        return AreaDatos::NuevoBloqueCreado;
    }

    // Caso3: ultimo bloque
    if (this->isLastBlock(pos)) {

    }

    // Caso bloque intermedio
    // Caso 1
    if (!this->isBlockFull(pos)) {
        auto posNuevoRegistro = this->buscarDirRegistroVacio(pos);
        this->registros[posNuevoRegistro] = {clave, dato, 0};
        this->ordenarBloque(pos);
        return AreaDatos::InsercionIntermedia;
    }

    // Caso 2.c
    if (isOverflowFull())
        return AreaDatos::OverflowLleno;

    // Caso 2.a y 2.b
    auto registroSinOverflow = this->buscarDirRegistroSinDireccion(pos, clave);
    this->ultimoRegistroInsertadoOverflow++;
    this->registros[this->ultimoRegistroInsertadoOverflow] = {clave, dato, 0};
    this->registros[registroSinOverflow].dir = this->ultimoRegistroInsertadoOverflow;
    
    if (isOverflowFull())
        return AreaDatos::OverflowLleno;
    else return AreaDatos::InsercionIntermedia;
}

void AreaDatos::ordenarBloque(int posInit)
{
    auto inicioBloque = registros.begin() + posInit;
    auto finalBloque = registros.begin() + (posInit + this->ELM_POR_BLOQ);
    sort(inicioBloque, finalBloque, [](Registro &a, Registro &b) { return a.clave < b.clave; });
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

bool AreaDatos::isOverflowFull()
{
    return (this->ultimoRegistroInsertadoOverflow == this->OMAX - 1);
}


int AreaDatos::buscarDirRegistroSinDireccion(int pos, int clave){
    auto dirClaveCercana = this->buscarDirClaveCercana(pos, clave);
    while (registros[dirClaveCercana].dir != 0)
        dirClaveCercana = registros[dirClaveCercana].dir;

    return dirClaveCercana;
}

int AreaDatos::buscarDirClaveCercana(int pos, int clave)
{
    int dirClaveAnterior = pos;
    for(int x = pos; (x < pos+this->ELM_POR_BLOQ) && (registros[x].clave < clave); x++)
        dirClaveAnterior = x;

    return dirClaveAnterior;    
}

int AreaDatos::buscarDirRegistroVacio(int bloque)
{
    for (int x = bloque; x < bloque+this->ELM_POR_BLOQ; x++)
        if (registros[x].clave == 0) return x;
    return -1;
}

/// Esta funcion ya no se utiliza pero la dejo aca como recuerdo
//  PD: Capaz luego la modificamos y asi modularizamos un poco mas la insercion 
int AreaDatos::insertarNuevoRegistroEnOverflow(int posBloque, int clave, string dato){
    int posRegistroCercano = buscarDirClaveCercana(posBloque, clave); //3.a.1: Se busca la clave menor mas cercana a la clave que vamos a insertar.
    int direccion = this->registros[posRegistroCercano].dir; //3.a.2: Se va a la direccion que contiene la clave menor. 
    if (direccion == 0){ //3.a.2.a: No apunta a ningun lado:
        /*
                    3.a.2.a.1: Se coloca el dato y la clave donde haya espacio en el overflow.
                    3.a.2.a.2: Se coloca la direccion del dato en la direccion del overflow de la clave menor. 
                     */
        int overPosLibre = this->ultimoRegistroInsertadoOverflow + 1;
        this->registros[posRegistroCercano].dir = overPosLibre;
        this->registros[overPosLibre] = {clave, dato, 0};

    }else{
                    /*3.a.2.b: La clave menor contiene una direccion. 
                    3.a.2.b.1: Nos movemos mediante las direcciones que contengan las claves posteriores al overflow. 
                    3.a.2.b.2: Se ubica una clave que no apunte a ningun lado.
                    3.a.2.b.3: Se ubica el registro en el overflow. 
                    3.a.2.b.4: Se coloca la direccion de ese registro en la clave previamente encontrada (dentro del overflow).*/
        int direccionActual = this->registros[posRegistroCercano].dir;

        while (this->registros[direccionActual].dir != 0){
            direccionActual = this->registros[direccionActual].dir;
            int overPosLibre = this->ultimoRegistroInsertadoOverflow + 1;
            this->registros[posRegistroCercano].dir = overPosLibre;
            this->registros[overPosLibre] = {clave, dato, 0};
                    
        }
                        
    }
        
}
#endif // AREA_DATOS_CPP