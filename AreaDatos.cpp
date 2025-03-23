#ifndef AREA_DATOS_CPP
#define AREA_DATOS_CPP

#include "AreaDatos.h"

AreaDatos::AreaDatos(int ELM_POR_BLOQ, int OMAX, int PMAX){
    this->ELM_POR_BLOQ = ELM_POR_BLOQ;
    this->OMAX = OMAX;
    this->PMAX = PMAX;
    
    this->registros.reserve(OMAX);
}

AreaDatos::Estado AreaDatos::insertar(int pos, int clave, string dato)
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
        return AreaDatos::NuevoBloqueCreado;
    }

    if (!this->isLastBlock(pos)){
        if (!this->isBlockFull(pos)) {

        }else{
            // caso bloque lleno
            //3.a: Hay espacio en el overflow.
                if (!isOverFull()){
                    int ultimoRegistro = this->OMAX-1;
                    /*
                     3.b: Queda UN espacio en el overflow.
                     3.b.1: Se inserta la clave. 
                     3.b.2: Se retorna Area::OverflowLleno
                    */
                    if(this->registros[ultimoRegistro].clave != 0;){
                        insertarNuevoRegistroEnOverflow(pos, clave, dato);
                        return AreaDatos::OverflowLleno;
                    }else{
                        insertarNuevoRegistroEnOverflow(pos, clave, dato);
                        return AreaDatos::InsercionIntermedia //Retorna: AreaDatos::InsercionIntermedia
                    }
                }
                
                

            
            

                               
            }
          
        


        } 



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

bool AreaDatos::isOverFull(){
    for(int x = this->PMAX; x < this->OMAX; x++){
        if (this->registros[x].clave == 0) return false;
    return true;
}

int AreaDatos::overPosLibre(){
    int noPosLibre = -1;
    for(int x = this->PMAX; x < this->OMAX; x++){
        
        if (this->registros[x].clave == 0) {
            return x;    
        }

   }
    return noPosLibre;
}

int AreaDatos::buscarRegistroCercano(int bloque, int clave){
    int claveCercana = -1;
    for(int x = bloque; x < bloque+this->ELM_POR_BLOQ; x++){
        if (this->registros[x].clave < clave && this->registros[x].clave > claveCercana){
            claveCercana = this->registros[x].clave;
            posicionCercana = x;
        }
    }
    return posicionCercana;
}


void AreaDatos::insertarNuevoRegistroEnOverflow(int posBloque, int clave, string dato){
    int posRegistroCercano = buscarRegistroCercano(posBloque, clave); //3.a.1: Se busca la clave menor mas cercana a la clave que vamos a insertar.
    int direccion = this->registros[posRegistroCercano].dir; //3.a.2: Se va a la direccion que contiene la clave menor. 
    if (direccion == 0){ //3.a.2.a: No apunta a ningun lado:
        /*
                    3.a.2.a.1: Se coloca el dato y la clave donde haya espacio en el overflow.
                    3.a.2.a.2: Se coloca la direccion del dato en la direccion del overflow de la clave menor. 
                     */
        int overPosLibre = overPosLibre();
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
            int overPosLibre = overPosLibre();
            this->registros[posRegistroCercano].dir = overPosLibre;
            this->registros[overPosLibre] = {clave, dato, 0};
                    
        }
                        
    }
        
}
#endif // AREA_DATOS_CPP