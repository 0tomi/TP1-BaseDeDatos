#ifndef MENU_CPP
#define MENU_CPP

#include <iostream>
#include "Archivo.h"
using namespace std;

Archivo definir_archivo(){
	int n;
	cout << "Tamaño de registros por bloque: " << endl;
	cin >> n;
	int PMAX;
	cout << "Tamaño del arreglo Area primaria de datos: " << endl;
	cin >> PMAX;
	int OMAX;
	cout << "Tamaño total del arreglo: "; << endl;
	cin >> OMAX;

	Archivo archive(n, PMAX, OMAX);
	return archive;

}

void menu_insercion(Archivo& archive){
	int clave;
	string dato;

	cout << "Clave a insertar: " << endl;
	cin >> clave;
	cout << "Dato a insertar" << endl;
	cin >> dato;

	string estadoInsercion = archive.insertar(clave, dato);
	cout << estadoInsercion;
}

void menu_consulta(Archivo& archive){
	int clave;

	cout << "Clave a consultar: " << endl;
	cin >> clave;

	string* resultado = archive.consulta(clave);
	if(resultado != nullptr){
		cout << "DATO ES:" << *resultado << endl;
	}else{
		cout << "no se encontro" << endl;
	}
}

void menu_mostrarAIyAD(Archivo& archive){
	cout << archive << endl;
}

void menu_global(Archivo& archive){
	int option;
	do{
		cout << "1. Insertar en Archivo " << endl;
		cout << "2. Consultar en Archivo" << endl;
		cout << "3. Mostrar Area de indices y Area de datos" << endl;
		cout << "4. Salir" << endl;
		cout << "seleccion opcion: " << endl;
		cin >> option;

		switch(option){
		case 1:
			menu_insercion(archive);
			break;
		case 2:
			menu_consulta(archive);
			break;
		case 3:
			menu_mostrarAIyAD(archive);
			break;
		case 4:
			break;
		default:
			cout << "No valido" << endl;
			break;
			}
		} while(option != 4);

	
}

int main(int argc, char *argv[]) {

	Archivo archive = definir_archivo();
	menu_global(archive);
	
	return 0;
}






#endif // !MENU_CPP