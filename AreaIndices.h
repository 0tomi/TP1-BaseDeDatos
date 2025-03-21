#ifndef AREA_INDICES_H
#define AREA_INDICES_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "Indice.h"

using namespace std;
class AreaIndices{
	private:
		vector<Indice> tablaIndices;
		int tamanio;

		void organizarIndices();
	public:
 		AreaIndices(int tam){this->tamanio = tam;}
 		int consultarBloque(int clave);
 		void actualizarTabla(vector<int> claves);
};

#endif // AREA_INDICES_H