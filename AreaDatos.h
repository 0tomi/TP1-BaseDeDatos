#ifndef AREA_DATOS_H
#define AREA_DATOS_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
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

        template<class U>
        friend ostream& operator<< (ostream& os, AreaDatos<U>& areaDatos);

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

template<typename T>
ostream& operator<< (ostream& os, AreaDatos<T>& areaDatos){
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

#endif // !AREA_DATOS_H