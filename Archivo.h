#ifndef ARCHIVO_H
#define ARCHIVO_H
#include <iostream>
#include "AreaDatos.cpp"
#include "AreaIndices.cpp"

template<typename T>
class Archivo{
public:
    Archivo(int n, int PMAX, int OMAX);
    T* consulta (int clave);    // Luego implementamos T en vez de string
    string insertar(int clave, T dato); // Luego implementamos T en vez de string
    AreaDatos<T>* verTablaDatos();
    AreaIndices* verTablaIndices();
    friend ostream& operator<< (ostream& os, Archivo& archivo);
    std::string warning();
    
private:
    string lastWarning;
    AreaDatos<T> tablaDatos;
    AreaIndices tablaIndices;
};

#endif // !ARCHIVO_H