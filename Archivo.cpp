#ifndef ARCHIVO_CPP
#define ARCHIVO_CPP
#include "Archivo.h"
#include <cstdlib>

Archivo::Archivo(int n, int PMAX, int OMAX): tablaDatos(n, PMAX, OMAX), tablaIndices(PMAX/n)
{
    // stuff to do
}

std::string* Archivo::consulta(int clave)
{
    auto direccion = this->tablaIndices.consultar(clave);
    return this->tablaDatos.consultar(direccion, clave);
}

std::string Archivo::insertar(int clave, std::string dato)
{
    auto direccion = this->tablaIndices.consultar(clave);
    auto codigo = this->tablaDatos.insertar(direccion, clave, dato);
    switch (codigo){
        case AreaDatos::InsercionIntermedia: 
            this->lastWarning = "";
            break;
 
        case AreaDatos::OverflowLleno:
            this->lastWarning = "Insercion realizada al final del overflow, overflow lleno, porfavor reorganizar..";
            break;

        case AreaDatos::AreaPrimariaLlena:
            this->lastWarning = "Ultimo bloque del area primaria colocado, sin espacio para mas bloques.";
            break;
        // Se creo un nuevo bloque / Se cambio de lugar el primer registro de un bloque.
        default:

            this->tablaIndices.actualizarTabla( this->tablaDatos.obtenerTablaIndices() );
            this->lastWarning = "";
    }
    return this->lastWarning;
}

AreaDatos* Archivo::verTablaDatos()
{
    return &(this->tablaDatos);
}

AreaIndices* Archivo::verTablaIndices()
{
    return &(this->tablaIndices);
}

ostream& operator<< (ostream& os, Archivo& archivo)
{
    os << "\n+---------------[Tabla de Indices]--------------+" << endl;
    os << archivo.tablaIndices;
    os << "\n+---------------[Tabla de datos]--------------+" << endl;
    os << archivo.tablaDatos;
    return os;
}

std::string Archivo::warning()
{
    return this->lastWarning;
}

#endif // !ARCHIVO_CPP

