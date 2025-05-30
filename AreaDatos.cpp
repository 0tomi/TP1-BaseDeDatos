#ifndef AREA_DATOS_CPP
#define AREA_DATOS_CPP

#include "AreaDatos.h"
#include "Indice.h"

template<typename T>
AreaDatos<T>::AreaDatos(int _ELM_POR_BLOQ, int _PMAX, int _OMAX): PMAX(_PMAX){
    this->ELM_POR_BLOQ = _ELM_POR_BLOQ;
    this->OMAX = _OMAX;
    this->PMAX = _PMAX;
    this->ultimoRegistroInsertadoOverflow = this->PMAX; // Porque antes de la insercion incrementamos la pos en 1, sino seria PMAX+1
    
    // Se podria implementar aca un check de seguridad mediante modularidad
    // Para que la cantidad maxima de bloques siempre sea redonda.

    this->CantidadMaximaBloques = this->PMAX / ELM_POR_BLOQ;

    this->registros = new Registro<T>[OMAX];
}

template<typename T>
typename AreaDatos<T>::Estado AreaDatos<T>::insertar(int block, int clave, T& dato)
{
    if (!CANTIDAD_BLOQUES) 
        return this->insercionCreandoUnBloque(clave, dato);
    
    auto porcentajeOcupacion = this->getOccupationRate(block);
    
    if (porcentajeOcupacion < 50)
        return this->insercionComun(block, clave, dato);
    if (porcentajeOcupacion == 100)
        return this->insercionBloqueLleno(block, clave, dato);
    
    return this->insercionBloqueMedioLleno(block, clave, dato);
}

template<class T>
typename AreaDatos<T>::Estado AreaDatos<T>::insercionCreandoUnBloque(int clave, T& dato)
{
    auto dirInsercion = this->crearBloque();
    this->registros[dirInsercion] = {clave, dato, 0}; // Apunta al overflow.
    return AreaDatos::NuevoBloqueCreado;
}


template<typename T>
// Posibles salidas: InsercionIntermedia, PrimerRegistroCambiado.
typename AreaDatos<T>::Estado AreaDatos<T>::insercionComun(int block, int clave, T&dato)
{
    auto posNuevoRegistro = this->buscarDirRegistroVacio(block);
    this->registros[posNuevoRegistro] = {clave, dato, 0};

    bool primerRegistroCambiado = this->ordenarBloque(block);
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
template<typename T>
typename AreaDatos<T>::Estado AreaDatos<T>::insercionBloqueMedioLleno(int block, int clave, T &dato)
{
    auto clavesMinMax = this->buscarDirClaveMinYMax(block);
    
    if (clave > clavesMinMax.first && clave < clavesMinMax.second)
        return this->insercionComunEnBloque(block, clave, dato);
    if (this->isAreaPrimariaLlena())
        return this->AreaPrimariaLlena;   // Estado
    
    return this->insercionCreandoUnBloque(clave, dato);
}

template<typename T>
pair<int,int> AreaDatos<T>::buscarDirClaveMinYMax(int bloque)
{
    auto claveMin = registros[bloque].clave;
    int claveMax = registros[this->buscarDirUltimoRegistro(bloque)].clave;

    return {claveMin, claveMax};
}
template<typename T>
typename AreaDatos<T>::Estado AreaDatos<T>::insercionComunEnBloque(int block, int clave, T &dato)
{
    auto posNuevaClave = this->buscarDirRegistroVacio(block);
    this->registros[posNuevaClave] = {clave, dato, 0};
    this->ordenarBloque(block);
    return InsercionIntermedia;
}

/*
    Posibles salidas: InsercionIntermedia, OverflowLleno, NuevoBloqueCreado
    Caso bloque lleno: 
    A: La clave a insertar esta entre los valores minimos y maximos del bloque.
        A.A. Hay espacio en el overflow.
            A.A.1- Se inserta el registro en el overflow.
            A.A.2- Se graba la direccion del overflow en el ultimo registro del bloque.
            A.A.2- Se retorna InsercionIntermedia.
        A.B. No hay espacio en el overflow.
            A.B.1- Se retorna OverflowLleno.
    B: La clave a insertar escapa de los valores minimos y maximos del bloque.
        B.1- Se crea nuevo bloque y se inserta la clave allí.
        B.2- Se retorna NuevoBloqueCreado.

*/
template<typename T>
typename AreaDatos<T>::Estado AreaDatos<T>::insercionBloqueLleno(int block, int clave, T &dato)
{
    auto clavesMinMax = this->buscarDirClaveMinYMax(block);
    if (clave > clavesMinMax.first && clave < clavesMinMax.second){
        if(!isOverflowFull()){
            auto posRegistroVacio = ultimoRegistroInsertadoOverflow+1;
            this->ultimoRegistroInsertadoOverflow++;
            this->registros[posRegistroVacio] = {clave, dato, 0};
            this->registros[buscarDirUltimoRegistro(block)].dir = this->PMAX+1;
            return AreaDatos::InsercionIntermedia;
        }
        // Si esta lleno:
        return AreaDatos::OverflowLleno;
    }

    if (this->isAreaPrimariaLlena())
        return this->AreaPrimariaLlena;
    
    return this->insercionCreandoUnBloque(clave, dato);
}

template<typename T>
T* AreaDatos<T>::consultar(int pos, int clave){
    int posRegistroBuscado = this->buscarDirClave(pos, clave);

    if (posRegistroBuscado == -1) {
        if (this->registros[buscarDirUltimoRegistro(pos)].dir == 0)
            return nullptr;

        posRegistroBuscado = this->registros[this->buscarDirUltimoRegistro(pos)].dir;

        while (posRegistroBuscado < this->OMAX && this->registros[posRegistroBuscado].clave != clave)
            posRegistroBuscado++;

        if (posRegistroBuscado == this->OMAX)
            return nullptr;
    }

    return &(registros[posRegistroBuscado].datos);
}

template<typename T>
vector<Indice> AreaDatos<T>::obtenerTablaIndices(){
    vector<Indice>indices;
    auto bloques = this->CANTIDAD_BLOQUES * this->ELM_POR_BLOQ;
    for(int i = 0; i < bloques; i += ELM_POR_BLOQ) 
        indices.push_back({registros[i].clave,i});
    return indices;
}

template<typename T>
bool AreaDatos<T>::ordenarBloque(int posInit)
{
    int clavePrimerRegistroAntesDeOrdenar = registros[posInit].clave;
    auto inicioBloque = registros + posInit;        
    auto finalBloque = registros + (posInit + this->ELM_POR_BLOQ);
    
    sort(inicioBloque, finalBloque, [](Registro<T> &a, Registro<T> &b){ 
        if (a.clave == 0) return false;
        if (b.clave == 0) return true;
        return a.clave < b.clave; 
    });
    
    return (clavePrimerRegistroAntesDeOrdenar != registros[posInit].clave);
}

template<typename T>
int AreaDatos<T>::crearBloque()
{
    this->CANTIDAD_BLOQUES++;

    if (CANTIDAD_BLOQUES-1 == 0)
        return this->dirUltimoBloqueInsertado;

    dirUltimoBloqueInsertado = dirUltimoBloqueInsertado + this->ELM_POR_BLOQ;
    return dirUltimoBloqueInsertado;
}

template<typename T>
int AreaDatos<T>::getOccupationRate(int block)
{
    int occupation = 0;
    for (int x = block; x < block+this->ELM_POR_BLOQ; x++)
        if (this->registros[x].clave != 0) occupation++;

    float ocu = occupation, n = this->ELM_POR_BLOQ; 
    float porcentage = (ocu / n) * 100;    
    return porcentage;
}

template<typename T>
bool AreaDatos<T>::isOverflowFull()
{
    return (this->ultimoRegistroInsertadoOverflow == this->OMAX - 1);
}

template<typename T>
bool AreaDatos<T>::isAreaPrimariaLlena()
{
    return (this->CANTIDAD_BLOQUES == CantidadMaximaBloques);
}

template<typename T>
int AreaDatos<T>::buscarDirClaveCercana(int pos, int clave)
{
    int dirClaveAnterior = pos;
    for(int x = pos; (x < pos+this->ELM_POR_BLOQ) && (registros[x].clave < clave); x++)
        dirClaveAnterior = x;

    return dirClaveAnterior;    
}

template<typename T>
int AreaDatos<T>::buscarDirUltimoRegistro(int bloque)
{
    int ultimoRegistro = bloque;
    for (int x = bloque; (x < this->ELM_POR_BLOQ+bloque) && (registros[x].clave != 0); x++)
        ultimoRegistro = x;
    return ultimoRegistro;
}

template<typename T>
int AreaDatos<T>::buscarDirRegistroVacio(int bloque)
{
    for (int x = bloque; x < bloque+this->ELM_POR_BLOQ; x++)
        if (registros[x].clave == 0) return x;
    return -1;
}

template<typename T>
int AreaDatos<T>::getCantidadRegistros(int bloque)
{
    int cantidad_Registros = 0;
    for (int x = bloque; x < bloque+ this->ELM_POR_BLOQ; x++)
        if (this->registros[x].clave) cantidad_Registros++;
    
    return cantidad_Registros;
}

template<typename T>
int AreaDatos<T>::buscarDirClave(int bloque, int clave)
{
    for (int x = bloque; x < bloque + this->ELM_POR_BLOQ; x++)
        if (registros[x].clave == clave) return x;
    return -1;    
}

#endif // AREA_DATOS_CPP