#ifndef AREA_INDICES_CPP
#define AREA_INDICES_CPP
#include "AreaIndices.h"
#include <iomanip>

ostream& operator <<(ostream& os, AreaIndices& areaIndices){
	if (areaIndices.cantidad == 0) {
		os << "[ Tabla de indices vacía ] ";
		return os;
	}

	os << "|----[Clave]---|---[Indice]---|\n";
    for(int i = 0; i < areaIndices.capacidad; i++){
		if (areaIndices.tablaIndices[i].clave != 0 )
			os << "|-----------------------------|" << endl
			   << "| " << setw(11) << areaIndices.tablaIndices[i].clave << " | " << setw(13) << areaIndices.tablaIndices[i].direccion << " |" << endl
			   << "|-----------------------------|" << endl;
    }

    return os;
}

void AreaIndices::organizarIndices(){
	sort(this->tablaIndices, this->tablaIndices + this->cantidad,[](const Indice& a,const Indice& b){
		if (a.clave == 0) return false;
        if (b.clave == 0) return true;
		return a.clave < b.clave;
	});
}

AreaIndices::AreaIndices(int PMAX): capacidad(PMAX), cantidad(0)
{
	this->tablaIndices = new Indice[this->capacidad];
    // Reservamos la cantidad de memoria que tendra el area de indices
}

AreaIndices::~AreaIndices(){
	delete []tablaIndices;
}

int AreaIndices::consultar(int claveBuscar){
	int indice = 0;
	int ultimoIndice = this->cantidad - 1;
	int bloque_anterior = 0;
	while(indice <= ultimoIndice){
		if(this->tablaIndices[indice].clave <= claveBuscar) 
			bloque_anterior = this->tablaIndices[indice].direccion; 
		indice++;
	}

	return bloque_anterior;


	/*int primerIndice = 0;
	int ultimoIndice = this->cantidad - 1;
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
	*/
}

void AreaIndices::actualizarTabla(vector<Indice> newIndices){
	move(newIndices.begin(), newIndices.end(), this->tablaIndices);
	this->cantidad = newIndices.size();
	this->organizarIndices();
}

#endif // AREA_INDICES_CPP





