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
    
    auto porcentajeOcupacion = this->getOccupationRate(pos);

    if (porcentajeOcupacion < 50);
        return this->insercionComun(pos, clave, dato);
    if (porcentajeOcupacion == 100)
        return this->insercionBloqueLleno(pos, clave, dato);
    
    return this->insercionBloqueMedioLleno(pos, clave, dato);
}

AreaDatos::Estado AreaDatos::insercionSinBloques(int clave, string& dato)
{
    this->crearBloque(0);
    this->registros[0] = {clave, dato, 0}; // Por default 0 indica que no apunta al overflow.
    return AreaDatos::NuevoBloqueCreado;
}

// Posibles salidas: InsercionIntermedia, PrimerRegistroCambiado.
AreaDatos::Estado AreaDatos::insercionComun(int block, int clave, string&dato)
{
    registros[this->buscarDirUltimoRegistro(block)].dir = 0;    // Limpio antes de ingresar el registro, en caso de que cambie el ultimo.

    auto posNuevoRegistro = this->buscarDirRegistroVacio(block);
    this->registros[posNuevoRegistro] = {clave, dato, 0};

    bool primerRegistroCambiado = this->ordenarBloque(block);
    registros[this->buscarDirUltimoRegistro(block)].dir = this->PMAX+1;     // Asignamos al ultimo registro el puntero al overflow

    if (primerRegistroCambiado)
        return this->PrimerRegistroCambiado;
    return this->InsercionIntermedia;
}

/*
    Posibles salidas: InsercionIntermedia, NuevoBloqueCreado, AreaPrimariaLlena
    Caso bloque medio lleno: 
    1- Nos fijamos cual es la clave maxima y minima que contiene el bloque.
    Casos posibles: 
        A - La clave a insertar se encuentra entre el rango de la maxima y la minima: 
            A.1 - Se inserta la clave de forma normal en el bloque.
            A.2 - Se reordena el bloque.
            A.3 - Se retorna insercion intermedia. (Es imposible que la primer o la ultima clave cambien.)
        B- La clave a insertar se encuentra por fuera del rango, ya se porque es menor que el minimo o mayor que el maximo.
            B.A - Hay espacio para nuevos bloques.
                B.A.1 - Se crea un nuevo bloque.
                B.A.2 - Se inserta la clave en el nuevo bloque.
                B.A.3 - Se retorna NuevoBloqueCreado. 
            B.B - No hay espacio para nuevos bloques. 
                B.A.1 - Se retorna AreaPrimariaLlena.
*/
AreaDatos::Estado AreaDatos::insercionBloqueMedioLleno(int block, int clave, string &dato)
{
    
}

/*
    Posibles salidas: InsercionIntermedia, OverflowLleno
    Caso bloque lleno: 
    A: Hay espacio en el Overflow
        A.1- Se inserta la clave en el overflow.
        A.2- Se retorna InsercionIntermedia.
    B: No hay espacio en el Overflow
        B.1- Se retorna OverflowLleno.

*/
AreaDatos::Estado AreaDatos::insercionBloqueLleno(int block, int clave, string &dato)
{
    if(getOccupationRate(PMAX+1) < 100){
        auto posRegistroVacio = this->buscarDirRegistroVacio(PMAX+1);
        this->registros[posRegistroVacio] = {clave, dato, 0};
        return AreaDatos::InsercionIntermedia;
    }else{
        return AreaDatos::OverflowLleno;
    }
}

/// ######## NO ELIMINO PORQUE SE PUEDE REUTILIZAR, PERO YA NO VA ########
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

/// ######## NO ELIMINO PORQUE SE PUEDE REUTILIZAR, PERO YA NO VA ########
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
/// ######## FIN DE FUNCIONES A ELIMINAR ########

string* AreaDatos::consultar(int pos, int clave){
    int posRegistroBuscado = this->buscarDirClave(pos, clave);
    if (posRegistroBuscado == -1) {
        auto posOverflow = this->registros[this->buscarDirUltimoRegistro(pos)].dir;
        for (int x = posOverflow; (x < this->OMAX) && (registros[x].clave != clave); x++)
            posRegistroBuscado = x;

        if (posRegistroBuscado == this->OMAX)
            posRegistroBuscado = -1;
    }

    if (posRegistroBuscado == -1)
        return nullptr;
    return &(registros[posRegistroBuscado].datos);
}

vector<Indice> AreaDatos::obtenerTablaIndices(){
    vector<Indice>indices;
    auto bloques = this->CANTIDAD_BLOQUES;
    for(int i = 0; i <= this->CANTIDAD_BLOQUES*ELM_POR_BLOQ; i += ELM_POR_BLOQ) 
        indices.push_back({registros[i].clave,i});
    return indices;
}

bool AreaDatos::ordenarBloque(int posInit)
{
    int clavePrimerRegistroAntesDeOrdenar = registros[posInit].clave;
    auto inicioBloque = registros.begin() + posInit;
    auto finalBloque = registros.begin() + (posInit + this->ELM_POR_BLOQ);
    sort(inicioBloque, finalBloque, [](Registro &a, Registro &b) { return a.clave < b.clave; });
    return (clavePrimerRegistroAntesDeOrdenar != registros[posInit].clave);
}

int AreaDatos::crearBloque(int pos)
{
    this->CANTIDAD_BLOQUES++;
    this->ultimoBloqueInsertado = pos;
    return ultimoBloqueInsertado;
}

int AreaDatos::getOccupationRate(int block)
{
    int occupation = 0;
    for (int x = block; x < block+this->ELM_POR_BLOQ; x++)
        if (this->registros[x].clave != 0) occupation++;

    float ocu = occupation, n = this->ELM_POR_BLOQ; 
    float porcentage = (ocu / n) * 100;    
    return porcentage;
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

int AreaDatos::buscarDirUltimoRegistro(int bloque)
{
    int ultimoRegistro = bloque;
    for (int x = bloque; (x < this->ELM_POR_BLOQ+bloque) && (registros[x].clave != 0); x++)
        ultimoRegistro = x;
    return ultimoRegistro;
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

int AreaDatos::buscarDirClave(int bloque, int clave)
{
    for (int x = bloque; x < bloque + this->ELM_POR_BLOQ; x++)
        if (registros[x].clave == clave) return x;
    return -1;    
}

#endif // AREA_DATOS_CPP