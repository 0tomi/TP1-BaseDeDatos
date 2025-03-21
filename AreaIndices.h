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
		friend ostream& operator<< (ostream& os, AreaIndices& areaIndices);
	public:
 		AreaIndices(int tam);
 		int consultar(int claveBuscar);
 		void actualizarTabla(vector<Indice> newIndices);
};

#endif // AREA_INDICES_H