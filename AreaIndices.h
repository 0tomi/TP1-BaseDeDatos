#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct Indice{
	int clave;
	int direccion;
};
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
