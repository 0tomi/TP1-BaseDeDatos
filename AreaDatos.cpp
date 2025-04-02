#ifndef AREA_DATOS_CPP
#define AREA_DATOS_CPP

#include "AreaDatos.h"
#include <iomanip>
#include "Indice.h"

AreaDatos::AreaDatos(int ELM_POR_BLOQ, int PMAX, int OMAX){
    this->ELM_POR_BLOQ = ELM_POR_BLOQ;
    this->OMAX = OMAX;
    this->PMAX = PMAX;
    this->ultimoRegistroInsertadoOverflow = this->PMAX;
    
    // Se podria implementar aca un check de seguridad mediante modularidad
    // Para que la cantidad maxima de bloques siempre sea redonda.

    this->CantidadMaximaBloques = this->PMAX / ELM_POR_BLOQ;

    this->registros = new Registro[OMAX];
}

ostream& operator<< (ostream& os, AreaDatos& areaDatos){
    if (areaDatos.CANTIDAD_BLOQUES == 0){
        os << "[ Area de datos vacia (Sin bloques) ]";
        return os;
    }
    
    int ultimaPosOcupada = areaDatos.CANTIDAD_BLOQUES * areaDatos.ELM_POR_BLOQ;
    // no se si se empieza a contar desde el bloque 0 o el 1, dejo 1 ante la duda
    int nroBloque = 1;

    os << "+|--[Clave]--|--[Indice]--|--[Direccion]--|+" << endl;
    os << "|------------------------------------------|" << endl;
    for (int i = 0; i < ultimaPosOcupada; i += areaDatos.ELM_POR_BLOQ) {
        os << "| BLOQUE " << nroBloque << setw(34) << "|" << endl;
        os << "|------------------------------------------|" << endl;
        for (int j = i; j < i + areaDatos.ELM_POR_BLOQ; j++)
            if (areaDatos.registros[j].clave != 0)
                os << "| " << setw(5) << areaDatos.registros[j].clave << " | " << setw(5) << areaDatos.registros[j].datos << " |" << setw(5) << areaDatos.registros[j].dir << endl
                << "|------------------------------------------|" << endl;

        nroBloque++;
    }

    if (areaDatos.ultimoRegistroInsertadoOverflow != areaDatos.PMAX) {
        os << "| ============= [ OVERFLOW ] ============= |" << endl
           << "| ---------------------------------------- |" << endl;
        int aux = areaDatos.PMAX + 1;
        while (areaDatos.registros[aux].clave != 0){
            os << "| " << setw(5) << areaDatos.registros[aux].clave << " | " << setw(5) << areaDatos.registros[aux].datos << " |" << setw(5) << areaDatos.registros[aux].dir << endl
               << "|------------------------------------------|" << endl;
            aux++;
        }
    }

    return os;
}

AreaDatos::Estado AreaDatos::insertar(int pos, int clave, string& dato)
{
    if (!CANTIDAD_BLOQUES) 
        return this->insercionCreandoUnBloque(clave, dato);
    
    auto porcentajeOcupacion = this->getOccupationRate(pos);
    
    if (porcentajeOcupacion < 50)
        return this->insercionComun(pos, clave, dato);
    if (porcentajeOcupacion == 100)
        return this->insercionBloqueLleno(clave, dato);
    
    return this->insercionBloqueMedioLleno(pos, clave, dato);
}

AreaDatos::Estado AreaDatos::insercionCreandoUnBloque(int clave, string& dato)
{
    auto dirInsercion = this->crearBloque();
    this->registros[dirInsercion] = {clave, dato, this->PMAX+1}; // Apunta al overflow.
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
    auto clavesMinMax = this->buscarDirClaveMinYMax(block);
    
    if (clave > clavesMinMax.first && clave < clavesMinMax.second)
        return this->insercionComunEnBloque(block, clave, dato);
    if (this->isAreaPrimariaLlena())
        return this->AreaPrimariaLlena;   // Estado
    
    return this->insercionCreandoUnBloque(clave, dato);
}

pair<int,int> AreaDatos::buscarDirClaveMinYMax(int bloque)
{
    auto claveMin = registros[bloque].clave;
    int claveMax = registros[this->buscarDirUltimoRegistro(bloque)].clave;

    return {claveMin, claveMax};
}

AreaDatos::Estado AreaDatos::insercionComunEnBloque(int block, int clave, string &dato)
{
    auto posNuevaClave = this->buscarDirRegistroVacio(block);
    this->registros[posNuevaClave] = {clave, dato, 0};
    this->ordenarBloque(block);
    return InsercionIntermedia;
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
AreaDatos::Estado AreaDatos::insercionBloqueLleno(int clave, string &dato)
{
    if(!isOverflowFull()){
        cout << "realizando inser en " << ultimoRegistroInsertadoOverflow;
        auto posRegistroVacio = ultimoRegistroInsertadoOverflow+1;
        this->ultimoRegistroInsertadoOverflow++;
        this->registros[posRegistroVacio] = {clave, dato, 0};
        return AreaDatos::InsercionIntermedia;
    }else{
        return AreaDatos::OverflowLleno;
    }
}

string* AreaDatos::consultar(int pos, int clave){
    int posRegistroBuscado = this->buscarDirClave(pos, clave);
    if (posRegistroBuscado == -1) {
        auto posOverflow = this->registros[this->buscarDirUltimoRegistro(pos)].dir;
        for (int x = posOverflow; (x < this->OMAX) && (registros[x].clave != clave); x++)
            posRegistroBuscado = x;

        posRegistroBuscado++;

        if (posRegistroBuscado == this->OMAX)
            return nullptr;
    }

    return &(registros[posRegistroBuscado].datos);
}

vector<Indice> AreaDatos::obtenerTablaIndices(){
    vector<Indice>indices;
    auto bloques = this->CANTIDAD_BLOQUES * this->ELM_POR_BLOQ;
    for(int i = 0; i < bloques; i += ELM_POR_BLOQ) 
        indices.push_back({registros[i].clave,i});
    return indices;
}

bool AreaDatos::ordenarBloque(int posInit)
{
    int clavePrimerRegistroAntesDeOrdenar = registros[posInit].clave;
    auto inicioBloque = registros + posInit;        // Por aritmetica de punteros, el compilador sabe que me quiero mover hasta donde esta el bloque.
    auto finalBloque = registros + (posInit + this->ELM_POR_BLOQ);
    
    sort(inicioBloque, finalBloque, [](Registro &a, Registro &b){ 
        if (a.clave == 0) return false;
        if (b.clave == 0) return true;
        return a.clave < b.clave; 
    });
    
    return (clavePrimerRegistroAntesDeOrdenar != registros[posInit].clave);
}

int AreaDatos::crearBloque()
{
    this->CANTIDAD_BLOQUES++;

    if (CANTIDAD_BLOQUES-1 == 0)
        return this->dirUltimoBloqueInsertado;

    dirUltimoBloqueInsertado = dirUltimoBloqueInsertado + this->ELM_POR_BLOQ;
    return dirUltimoBloqueInsertado;
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

bool AreaDatos::isOverflowFull()
{
    return (this->ultimoRegistroInsertadoOverflow == this->OMAX - 1);
}

bool AreaDatos::isAreaPrimariaLlena()
{
    return (this->CANTIDAD_BLOQUES == CantidadMaximaBloques);
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