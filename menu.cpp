#ifndef MENU_CPP
#define MENU_CPP

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include "Archivo.cpp"
using namespace std;

Archivo<string> definir_archivo(){
	int n;
	cout << "Tamaño de registros por bloque: " << endl;
	cin >> n;
	int PMAX;
	cout << "Tamaño del arreglo Area primaria de datos: " << endl;
	cin >> PMAX;
	int OMAX;
	cout << "Tamaño total del arreglo: " << endl;
	cin >> OMAX;

	Archivo<string> archive(n, PMAX, OMAX);
	return archive;

}

void menu_insercion(Archivo<string>& archive){
	int clave;
	string dato;

	cout << "Clave a insertar: " << endl;
	cin >> clave;
	cout << "Dato a insertar" << endl;
	cin >> dato;

	string estadoInsercion = archive.insertar(clave, dato);
	cout << estadoInsercion
		<< "\nTermino insercion.";
}

void menu_consulta(Archivo<string>& archive){
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

void menu_mostrarAIyAD(Archivo<string>& archive){
	cout << archive << endl;
}

void menu_global(Archivo<string>& archive){
	char option = 0;
	do {
		cout << "\n===============[ MENU ]===============\n";
		cout << "1. Insertar en Archivo " << endl;
		cout << "2. Consultar en Archivo" << endl;
		cout << "3. Mostrar Area de indices y Area de datos" << endl;
		cout << "4. Salir";
		cout << "\n===============[ MENU ]===============\n";
		option = getch();
			switch(option){
			case '1':
				menu_insercion(archive);
				break;
			case '2':
				menu_consulta(archive);
				break;
			case '3':
				menu_mostrarAIyAD(archive);
				break;
			} 

	}while(option != '4');
}

int main(int argc, char *argv[]) {

	//Archivo archive = definir_archivo();
	Archivo<string> arch(4, 16, 24);
	arch.insertar(3, "test1");
	arch.insertar(8, "test2");
	arch.insertar(5, "test3");
	arch.insertar(10, "test4");
	arch.insertar(12, "test5");
	arch.insertar(14, "test6");
	arch.insertar(6, "test7");
	arch.insertar(7, "test8");
	arch.insertar(4, "test9");



	menu_global(arch);
	
	return 0;
}






#endif // !MENU_CPP