#ifndef ARCHIVO_H
#define ARCHIVO_H
#include <iostream>
#include "AreaDatos.cpp"
#include "AreaIndices.cpp"

template<class T>
class Archivo{
public:
    Archivo(int n, int PMAX, int OMAX);
    T* consulta (int clave);    // Luego implementamos T en vez de string
    string insertar(int clave, T dato); // Luego implementamos T en vez de string
    AreaDatos<T>* verTablaDatos();
    AreaIndices* verTablaIndices();
    std::string warning();
    template <class U>
    friend ostream& operator<< (ostream& os, Archivo<U>& archivo);
    
private:
    string lastWarning;
    AreaDatos<T> tablaDatos;
    AreaIndices tablaIndices;
};

template<typename T>
ostream& operator<< (ostream& os, Archivo<T>& archivo)
{
    os << "\n+---------------[Tabla de Indices]--------------+" << endl;
    os << archivo.tablaIndices;
    os << "\n+---------------[Tabla de datos]--------------+" << endl;
    os << archivo.tablaDatos;
    return os;
}

#endif // !ARCHIVO_H