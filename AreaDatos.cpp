#ifndef AREA_DATOS_CPP
#define AREA_DATOS_CPP

#include "AreaDatos.h"
#include <iomanip>
#include "Indice.h"

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
        os << "| " << setw(5) << areaDatos.registros[x].clave << " | " << setw(5) << areaDatos.registros[x].datos << " |" << setw(5) << areaDatos.registros[x].dir << endl;
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
    if (!CANTIDAD_BLOQUES) 
        return this->insercionSinBloques(clave, dato);
    
    if (this->isLastBlock(pos)) 
        return this->insercionUltimoBloque(clave, dato, pos);
    
    return this->insercionBloqueIntermedio(clave, dato, pos);
}

AreaDatos::Estado AreaDatos::insercionSinBloques(int clave, string& dato)
{
    this->crearBloque(0);
    this->registros[0] = {clave, dato, 0}; // Por default 0 indica que no apunta al overflow.
    return AreaDatos::NuevoBloqueCreado;
}

AreaDatos::Estado AreaDatos::insercionUltimoBloque(int clave, string& dato, int pos)
{
    auto cantidadRegistros = this->getCantidadRegistros(pos);
    int limiteMitad = ELM_POR_BLOQ/2;

    if(cantidadRegistros < limiteMitad){ 
        auto posNuevoRegistro = this->buscarDirRegistroVacio(pos);
        this->registros[posNuevoRegistro] = {clave, dato, 0};
        return AreaDatos::InsercionIntermedia;
    } 

    if (ultimoBloqueInsertado + ELM_POR_BLOQ == PMAX)
        return AreaDatos::AreaPrimariaLlena;

    auto nuevaPos = this->crearBloque(ultimoBloqueInsertado + ELM_POR_BLOQ);
    this->registros[nuevaPos] = {clave, dato, 0};

    if(this->ultimoBloqueInsertado+ELM_POR_BLOQ == PMAX)
        return AreaDatos::AreaPrimariaLlena;

    return AreaDatos::NuevoBloqueCreado;
}

AreaDatos::Estado AreaDatos::insercionBloqueIntermedio(int clave, string& dato, int pos)
{
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
    return AreaDatos::InsercionIntermedia;
}

vector<Indice> AreaDatos::obtenerTablaIndices(){
    vector<Indice>indices;
    auto bloques = this->CANTIDAD_BLOQUES;
    for(int i = 0; i <= this->CANTIDAD_BLOQUES*ELM_POR_BLOQ; i += ELM_POR_BLOQ) 
        indices.push_back({registros[i].clave,i});
    return indices;
}

void AreaDatos::ordenarBloque(int posInit)
{
    auto inicioBloque = registros.begin() + posInit;
    auto finalBloque = registros.begin() + (posInit + this->ELM_POR_BLOQ);
    sort(inicioBloque, finalBloque, [](Registro &a, Registro &b) { return a.clave < b.clave; });
}

int AreaDatos::crearBloque(int pos)
{
    this->CANTIDAD_BLOQUES++;
    this->ultimoBloqueInsertado = pos;
    return ultimoBloqueInsertado;
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

  string* AreaDatos::consultar(int pos, int clave){
    int dirClaveAnterior = this->buscarDirClaveCercana(int pos, int clave);
    if(dirClaveAnterior+1 == pos+this->ELM_POR_BLOQ || registros[dirClaveAnterior+1].clave != clave){
       int dirOver=registros[dirClaveAnterior].dir;
        while (dirOver != 0)
        {
            if(registros[dirOver].clave == clave){
                  return &(registros[dirOver].datos);
            }

            dirOver = registros[dirOver].dir;
        }
    
        return nullptr;

    }
    return &(registros[dirClaveAnterior+1].datos);

  }

int AreaDatos::buscarDirRegistroVacio(int bloque)
{
    for (int x = bloque; x < bloque+this->ELM_POR_BLOQ; x++)
        if (registros[x].clave == 0) return x;
    return -1;
}

int AreaDatos::getCantidadRegistros(int bloque)
{
    int cantidad_Registros = 0;
    for (int x = bloque; x < bloque+ this->ELM_POR_BLOQ; x++)
        if (this->registros[x].clave) cantidad_Registros++;
    
    return cantidad_Registros;
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