#ifndef AREA_INDICES_CPP
#define AREA_INDICES_CPP
#include "AreaIndices.h"
void AreaIndices::organizarIndices(){
	sort(this->tablaIndices.begin(),this->tablaIndices.end(),[](const Indice& a,const Indice& b){
		return a.clave < b.clave;
	});
}

int AreaIndices::consultar(int claveBuscar){
	int primerIndice = 0;
	int ultimoIndice = this->tablaIndices.size() - 1;
	int bloque_cercano = -1; 									//Si no se encuentra
	while(primerIndice <= ultimoIndice){
		int mitad = (primerIndice + ultimoIndice) / 2;

		if(this->tablaIndices[mitad].clave == claveBuscar){
			return this->tablaIndices[mitad].direccion;
		}

		if(this->tablaIndices[mitad].clave < claveBuscar){
			bloque_cercano = this->tablaIndices[mitad].direccion;
			primerIndice = mitad + 1;
		}else{
			ultimoIndice = mitad - 1;
		}
	}
	return bloque_cercano;
}

void AreaIndices::actualizarTabla(vector<Indice> newIndices){
	this->tablaIndices = move(newIndices);
	this->organizarIndices();
}

#endif // AREA_INDICES_CPP





