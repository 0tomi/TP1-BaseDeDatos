#ifndef AREA_DATOS_H
#define AREA_DATOS_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "Registro.h"
#include "Indice.h"
using namespace std;

template<typename T>
class AreaDatos{
    public:
        enum Estado {
            InsercionIntermedia,
            NuevoBloqueCreado,
            OverflowLleno,
            AreaPrimariaLlena,
            PrimerRegistroCambiado
        };

        AreaDatos(int ELM_POR_BLOQ, int PMAX, int OMAX);
        ~AreaDatos() { delete registros; }
        Estado insertar(int pos, int clave, T& dato);
        T* consultar(int pos, int clave); 
        vector<Indice> obtenerTablaIndices();
        friend ostream& operator<< (ostream& os, AreaDatos<T>& areaDatos);

    private:
        Registro<T>* registros;
        int CANTIDAD_BLOQUES = 0, ELM_POR_BLOQ, OMAX, PMAX;    // ELM_POR_BLOQ = n; OMAX = tamanio maximo array; PMAX = tamanio maximo zona de datos
        int dirUltimoBloqueInsertado = 0;
        int ultimoRegistroInsertadoOverflow = PMAX+1;
        int CantidadMaximaBloques;

        Estado insercionCreandoUnBloque(int clave, T& dato);
        Estado insercionComun(int block, int clave, T &dato);

        Estado insercionBloqueMedioLleno(int block, int clave, T &dato);
        Estado insercionComunEnBloque(int block, int clave, T &dato);

        Estado insercionBloqueLleno(int clave, T &dato);

        bool ordenarBloque(int posInit);    // True: Primer registro cambiado
        int crearBloque();
        int getOccupationRate(int block);
        bool isOverflowFull();
        bool isAreaPrimariaLlena();

        int buscarDirClaveCercana(int bloque, int clave);
        int buscarDirUltimoRegistro(int bloque);
        int buscarDirRegistroVacio(int bloque);
        int getCantidadRegistros(int bloque);
        int buscarDirClave(int bloque, int clave);
        pair<int,int> buscarDirClaveMinYMax(int bloque);
};

#endif // !AREA_DATOS_H