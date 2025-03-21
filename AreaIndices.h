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
 		int consultar(int claveBuscar);
 		void actualizarTabla(vector<Indice> newIndices);
};

#endif // AREA_INDICES_H