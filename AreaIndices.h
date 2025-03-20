#include <iostream>
#include <vector>
#include <algorithm>
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
