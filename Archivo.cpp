#ifndef ARCHIVO_CPP
#define ARCHIVO_CPP
#include "Archivo.h"

Archivo::Archivo(int n, int PMAX, int OMAX): tablaDatos(n, PMAX, OMAX), tablaIndices(PMAX/n)
{
    // stuff to do
}

std::string* Archivo::consulta(int clave)
{
    auto direccion = this->tablaIndices.consultar(clave);
    return this->tablaDatos.consultar(direccion);
}

std::string Archivo::insertar(int clave, std::string dato)
{
    auto direccion = this->tablaIndices.consultar(clave);
    auto codigo = this->tablaDatos.insertar(clave, dato);
    switch (codigo){
        // 1: Insercion mediante nuevo bloque.
        case 1: 
            this->tablaIndices.actualizarTabla( this->tablaDatos.obtenerTablaIndices() );
            this->lastWarning = "";
            break;
        // 2: Overflow lleno.
        case 2:
            this->lastWarning = "Insercion realizada al final del overflow, overflow lleno, porfavor reorganizar..";
            break;
        // 3: Area primaria llena.
        case 3:
            this->lastWarning = "Ultimo bloque del area primaria colocado, sin espacio para mas bloques.";
            break;
        // 0: Insercion directa.
        default: this->lastWarning = "";
    }

    return this->lastWarning;
}

int Archivo::cantidadRegistros()
{
    return this->tablaDatos.getCantidadRegistros();
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
    os << archivo.tablaDatos << endl << archivo.tablaIndices;
    return os;
}

std::string Archivo::warning()
{
    return this->lastWarning;
}

#endif // !ARCHIVO_CPP

