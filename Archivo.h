#ifndef ARCHIVO_H
#define ARCHIVO_H
#include <string>
#include "AreaDatos.cpp"
#include "AreaIndices.cpp"

class Archivo{
public:
    Archivo(int n, int PMAX, int OMAX);
    std::string* consulta (int clave);    // Luego implementamos T en vez de string
    std::string insertar(int clave, std::string dato); // Luego implementamos T en vez de string
    int cantidadRegistros();
    AreaDatos* verTablaDatos();
    AreaIndices* verTablaIndices();
    friend ostream& operator<< (ostream& os, Archivo& archivo);
    std::string warning();
    
private:
    string lastWarning;
    AreaDatos tablaDatos;
    AreaIndices tablaIndices;
};

#endif // !ARCHIVO_H