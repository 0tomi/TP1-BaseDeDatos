#ifndef AREA_DATOS_H
#define AREA_DATOS_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "Registro.h"
#include "Indice.h"
using namespace std;

class AreaDatos{
    public:
        enum Estado {
            InsercionIntermedia,
            NuevoBloqueCreado,
            OverflowLleno,
            AreaPrimariaLlena,
            PrimerRegistroCambiado
        };

        AreaDatos(int ELM_POR_BLOQ, int OMAX, int PMAX);
        Estado insertar(int pos, int clave, string& dato);
        string* consultar(int pos, int clave); // string luego se reemplaza por Template
        vector<Indice> obtenerTablaIndices();
        friend ostream& operator<< (ostream& os, AreaDatos& areaDatos);

    private:
        vector<Registro> registros;
        int CANTIDAD_BLOQUES = 0, ELM_POR_BLOQ, OMAX, PMAX;    // ELM_POR_BLOQ = n; OMAX = tamanio maximo array; PMAX = tamanio maximo zona de datos
        int dirUltimoBloqueInsertado = 0;
        int ultimoRegistroInsertadoOverflow = PMAX+1;
        int CantidadMaximaBloques;

        Estado insercionCreandoUnBloque(int clave, string & ato);
        Estado insercionComun(int block, int clave, string&dato);

        Estado insercionBloqueMedioLleno(int block, int clave, string &dato);
        Estado insercionComunEnBloque(int block, int clave, string &dato);

        Estado insercionBloqueLleno(int clave, string &dato);

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

/**
template <typename T>
int AreaDatos::insertarRegistro(int clave, T dato){
    return 0;
}

template <typename T>
T* AreaDatos::obtenerRegistro(int clave){
    return nullptr;
}

 
a) Los registros se agrupan en bloques de n registros. //! HACER
b) Los registros se encuentran ubicados en orden ascendente de su clave //! HACER 
c) Cuando la misma es cero se supone un elemento vacío del arreglo. //! HACER
d) Este arreglo de registros está dividido en dos partes, la primera, llamada el
área primaria de datos, cuyo tamaño está dado por la variable PMAX, y la de Overflow,
que inicia en PMAX+1 = OVER, y que llega hasta OMAX (dimensión del arreglo). // HACIENDO
-------------------------------------------------------------------------------------------
//! HACER
Area de datos devuelve un int tras la inserción, estos int significan distintos estados:
0: Inserción realizada exitosamente
1: Inserción realizada en un nuevo bloque
2: Inserción realizada al final del overflow
3: Inserción realizada sin espacio para nuevos bloques [n registros restantes para ubicar] 
-------------------------------------------------------------------------------------------
1)Para la función de consulta, recibe como entrada la CLAVE que desea consultar y como
salida los datos del Registro. //! HACER
2) Para la inserción, se tienen como entradas la CLAVE y los datos del Registro, y deben
tener en cuenta las siguientes consideraciones:
- Los registros dentro de cada bloque deben estar en orden ascendente por su clave.
- Si la inserción corresponde al último registro del bloque y el número de los mismos
es igual o mayor a n/2 iniciar un nuevo bloque conservando todas las
características del archivo definido. Si la inserción corresponde en un espacio
intermedio del bloque, y no hay espacio en el mismo, se supone n, ubicar el
registro en el área de Overflow y poner en el último registro del bloque la dirección
del mismo. //! HACER
- Escribir mensajes para reorganizar el archivo cuando no se tiene más lugar para
abrir un nuevo bloque (se supera PMAX) y para cuando no se tiene más lugar en el
área de Overflow (se supera OMAX)) //! HACER

//? Preguntar  
1.1 Empezamos con 0 bloques, y se van insertando en el area primaria de datos  (PMAX) hasta que se llene, si se llena se emite un aviso (bool) para reorganizar los bloques.
1.2 Empezamos con el overflow desde el principio, y cuando se llena se emite mensaje de reorganizar.
*/