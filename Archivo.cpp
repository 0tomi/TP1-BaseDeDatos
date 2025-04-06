#ifndef ARCHIVO_CPP
#define ARCHIVO_CPP
#include "Archivo.h"
#include <cstdlib>

template<typename T>
Archivo<T>::Archivo(int n, int PMAX, int OMAX): tablaDatos(n, PMAX, OMAX), tablaIndices(PMAX/n)
{

}

template<typename T>
T* Archivo<T>::consulta(int clave)
{
    auto direccion = this->tablaIndices.consultar(clave);
    return this->tablaDatos.consultar(direccion, clave);
}

template<typename T>
std::string Archivo<T>::insertar(int clave, T dato)
{
    auto direccion = this->tablaIndices.consultar(clave);
    auto codigo = this->tablaDatos.insertar(direccion, clave, dato);
    switch (codigo){
        case AreaDatos<T>::InsercionIntermedia: 
            this->lastWarning = "";
            break;
 
        case AreaDatos<T>::OverflowLleno:
            this->lastWarning = "Overflow lleno, por favor reorganizar..";
            break;

        case AreaDatos<T>::AreaPrimariaLlena:
            this->lastWarning = "Ultimo bloque del area primaria colocado, sin espacio para mas bloques.";
            break;
        // Se creo un nuevo bloque / Se cambio de lugar el primer registro de un bloque.
        default:

            this->tablaIndices.actualizarTabla( this->tablaDatos.obtenerTablaIndices() );
            this->lastWarning = "";
    }
    return this->lastWarning;
}

template<typename T>
AreaDatos<T>* Archivo<T>::verTablaDatos()
{
    return &(this->tablaDatos);
}

template<typename T>
AreaIndices* Archivo<T>::verTablaIndices()
{
    return &(this->tablaIndices);
}

template<typename T>
std::string Archivo<T>::warning()
{
    return this->lastWarning;
}

#endif // !ARCHIVO_CPP

