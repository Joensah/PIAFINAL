#include <Windows.h>
#include <CommCtrl.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include "resource.h"
#include <mmsystem.h>
#include <CommDlg.h>
#include <regex>
#include <time.h>
#include "PIAESTDAT.h"
#include "Estructuras.h"

//Constantes (Para que el "switch de guardado" y "limpiar lista" sepan cual es que)
//Estructura ( limpiara y guardara, cada una tiene su propia de cargado )
#define guardar_usuarios 1 
#define guardar_ciudadanos 2
#define guardar_vacunas 3
#define guardar_carnet 4

#define busqueda_carnet 2
#define busqueda_nombre 1

#define por_rfc 1
#define por_curp 2

using namespace std;
//LOGIN
BOOL CALLBACK Inicio_Sesion(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Registro_Usuario(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK MenuPrincipal(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam);
//ALTAS
BOOL CALLBACK Registro_Personas(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Registro_Vacunas(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Registro_Carnet(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam);
//BAJAS
BOOL CALLBACK Eliminar_Vacunas(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Eliminar_Carnet(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam);
//CAMBIOS
BOOL CALLBACK Editar_Personas(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Editar_Vacunas(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Editar_Carnet(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam);
//REPORTES
BOOL CALLBACK Reportes(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Buscar(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam);

//Estructuras de datos de las personas

struct usuario {
	char nombre_usuario[64];
	char A_Paterno[32];
	char A_Materno[32];
	char clave_usuario[32];
	char contrasena[32];

	usuario* anterior;
	usuario* siguiente;
};

struct persona {
	char paterno[100];
	char materno[100];
	char nombre_persona[100];
	char CURP[30];
	char RFC[30];
	char calle[100];
	char colonia[100];
	char ciudad[100];  //Antes municipio
	char estado[100];
	char e_civil[20];
	char telefono[12];
	char foto_c[8192];
	char sexo[20];
	char Gpo_ocupacional[30];
	char Perfil_riesgo[30];

	int codigo_postal;
	int numero;
	int edad;

	SYSTEMTIME nacimiento;

	persona* anterior;
	persona* siguiente;
};
struct vacuna {
	char numero_lote[30];
	float precio;
	int dosis_requeridas;
	int cantidad_dosis;
	char descripcion[255];
	char marca[255];
	char tipo[30];


	vacuna* anterior;
	vacuna* siguiente;
};
struct carnet {
	int No_carnet;
	int No_dosis;
	int edad;
	char z_vac[100];//tipo vacuna
	char lugar[100];
	char nombre_persona[50];
	char npaterno[40];
	char nmaterno[40];
	char CURP[35];
	char vacuna[30];
	char lote[30];
	char sexo[20];
	char foto[8196];
	SYSTEMTIME fecha_aplicacion;
	SYSTEMTIME prox_dosis;
	SYSTEMTIME fecha_nac;
	//Se guardaran demas datos pero estos no se visualizaran
	//solo se mostraran en reportes
	int numero;
	char calle[100];
	char colonia[100];
	char estado[100];
	char ciudad[100];
	char e_civil[20];
	char Gpo_ocupacional[30];
	char Perfil_riesgo[30];
	char telefono[12];
	char RFC[30];


}lista_carnet[100];
carnet lista_heapquick[100];
carnet lista_search[100];



//Estructuras 2.0
//struct USUARIO {
//	char nombre_usuario[100];
//	char contrasena[100];
//	//Datos del usuario.
//	char clave_usuario[100];
//	char Uapellido[100];
//
//	void setUsuario(string usuarioP) {
//		strcpy_s(usuario, 100, usuarioP.c_str());
//	}
//	string getUsuario() {
//		string str(usuario);
//		return str;
//	}
//	void setContra(string contraP) {
//		strcpy_s(contrasena, 100, contraP.c_str());
//	}
//	string getContra() {
//		string str(contrasena);
//		return str;
//	}
//	void setUnombre(string UnombreP) {
//		strcpy_s(clave_usuario, 100, UnombreP.c_str());
//	}
//	string getUnombre() {
//		string str(clave_usuario);
//		return str;
//	}
//	void setUapellido(string UapellidoP) {
//		strcpy_s(Uapellido, 100, UapellidoP.c_str());
//	}
//	string getUapellido() {
//		string str(Uapellido);
//		return str;
//	}
//};
//struct NODOUSUARIO {
//	USUARIO* usuario = NULL;
//	NODOUSUARIO* siguiente = NULL;
//	NODOUSUARIO* anterior = NULL;
//};
//struct LISTAUSUARIOS {
//	NODOUSUARIO* origen = NULL;
//	NODOUSUARIO* fin = NULL;
//	void agregar(USUARIO* nuevo) {
//		if (origen == NULL) {
//			origen = new NODOUSUARIO;
//			origen->usuario = nuevo;
//		}
//		else {
//
//			NODOUSUARIO* elemento = origen;
//			while (elemento->siguiente != NULL) {
//				elemento = elemento->siguiente;
//			}
//			elemento->siguiente = new NODOUSUARIO;
//			elemento->siguiente->usuario = nuevo;
//			elemento->siguiente->anterior = elemento;
//			fin = elemento->siguiente;
//		}
//	}
//}USUARIOS;
//


//punteros de inicio 
usuario* inicio_uc = nullptr;
persona* inicio_c = nullptr;
vacuna* inicio_v = nullptr;
carnet* inicio_carnet = nullptr;

carnet* primero = nullptr;
carnet* ultimo = nullptr;
carnet* Bbinaria = nullptr;
persona* lista = nullptr;
persona* visual = nullptr;
int medio = 0;

//Para validar que se encontraron datos a buscar 
int encontrar_usuario = 0, CURP = NULL, RFC = NULL, dup_vacuna = NULL, dato_limpiar = 0, dato_guardar = 0, indice_carnet = 0, ID_carnet = 0, valor_busqueda = 0;
char validar__usuario[255], validar__contra[255], validar_contexto[255], validar_contexto2[255], usuario_activo[100], foto_persona[8192];
bool encontrar = false;
int mostrar = 0;
int seleccion_busqueda = 2;
int seleccion_reporte = 2;
int seleccion_p = 1;
char aux_foto[8196];
//Expresiones regulares para validaciones 
basic_regex<char>costo("[0-9]+.[0-9]{2}"); basic_regex<char>numeros("[0-9]+"); basic_regex<char>n_tel("[0-9]{10}"); basic_regex<char>letras("[A-Za-z ]+");
//VALIDACIONES DE FECHAS NO SEAN ANTERIORES A ACTUALES
long long tiempo(SYSTEMTIME dia) {
	FILETIME tiempo;
	SystemTimeToFileTime(&dia, &tiempo);
	long long transcurrido = tiempo.dwHighDateTime;
	transcurrido = transcurrido << 32;
	transcurrido += tiempo.dwLowDateTime;
	transcurrido = transcurrido / 10000000;
	transcurrido = transcurrido * 86400;
	return transcurrido;
}
int diferenciadias(SYSTEMTIME fecha1, SYSTEMTIME fecha2) {
	return tiempo(fecha1) - tiempo(fecha2);
}
//limpiar y actualizar listas ligadas
void limpiar() {
	usuario* aux = inicio_uc;
	persona* aux_c = inicio_c;
	vacuna* aux_v = inicio_v;

	switch (dato_limpiar)
	{
	case 1:
		if (inicio_uc != NULL) {

			if (inicio_uc != NULL) {
				while (aux->siguiente != inicio_uc) {
					aux = aux->siguiente;
					delete aux->anterior;
				}

			}
			delete aux;
		}
		inicio_uc = NULL;
		break;

	case 2:
		if (inicio_c != NULL) {

			if (inicio_c != NULL) {
				while (aux_c->siguiente != inicio_c) {
					aux_c = aux_c->siguiente;

					delete aux_c->anterior;
				}
			}
			delete aux_c;
		}
		inicio_c = NULL;
		break;
	case 3:
		if (inicio_v != NULL) {

			if (inicio_v != NULL) {
				while (aux_v->siguiente != inicio_v) {
					aux_v = aux_v->siguiente;
					delete aux_v->anterior;
				}
			}
			delete aux_v;
		}
		inicio_v = NULL;
		break;


	default:
		break;
	}
	dato_limpiar = 0;

}
void anadir(persona* nuevo) {
	if (inicio_c == NULL) {
		inicio_c = nuevo;
		nuevo->anterior = nuevo;
		nuevo->siguiente = nuevo;


	}
	else {
		inicio_c->anterior->siguiente = nuevo;
		nuevo->anterior = inicio_c->anterior;
		nuevo->siguiente = inicio_c;
		inicio_c->anterior = nuevo;


	}
}
void anadir(vacuna* nuevo) {
	if (inicio_v == NULL) {
		inicio_v = nuevo;
		nuevo->anterior = nuevo;
		nuevo->siguiente = nuevo;


	}
	else {
		inicio_v->anterior->siguiente = nuevo;
		nuevo->anterior = inicio_v->anterior;
		nuevo->siguiente = inicio_v;
		inicio_v->anterior = nuevo;


	}
}
void anadir(usuario* nuevo) {
	if (inicio_uc == NULL) {
		inicio_uc = nuevo;
		nuevo->anterior = nuevo;
		nuevo->siguiente = nuevo;


	}
	else {
		inicio_uc->anterior->siguiente = nuevo;
		nuevo->anterior = inicio_uc->anterior;
		nuevo->siguiente = inicio_uc;
		inicio_uc->anterior = nuevo;


	}
}


void eliminar(persona* eliminar) {

	persona* anterior = eliminar->anterior;
	persona* siguiente = eliminar->siguiente;

	if (anterior == siguiente && anterior == eliminar) {
		delete eliminar;
		inicio_c = NULL;
		return;
	}
	if (anterior->siguiente == inicio_c) {
		inicio_c = inicio_c->siguiente;
	}
	anterior->siguiente = siguiente;
	siguiente->anterior = anterior;
	delete eliminar;

}
void eliminar(usuario* eliminar) {

	usuario* anterior = eliminar->anterior;
	usuario* siguiente = eliminar->siguiente;

	if (anterior == siguiente && anterior == eliminar) {
		delete eliminar;
		inicio_uc = NULL;
		return;
	}
	if (anterior->siguiente == inicio_uc) {
		inicio_uc = inicio_uc->siguiente;
	}
	anterior->siguiente = siguiente;
	siguiente->anterior = anterior;
	delete eliminar;

}
void eliminar(vacuna* eliminar) {

	vacuna* anterior = eliminar->anterior;
	vacuna* siguiente = eliminar->siguiente;

	if (anterior == siguiente && anterior == eliminar) {
		delete eliminar;
		inicio_v = NULL;
		return;
	}
	if (anterior->siguiente == inicio_v) {
		inicio_v = inicio_v->siguiente;
	}
	anterior->siguiente = siguiente;
	siguiente->anterior = anterior;
	delete eliminar;

}
//guardados y cargados de informaciones desde los archivos de texto 
//"C:/Users/enriq/source/repos/PIAESTDAT/Indice.bin"
void Guardar_usuarios() {
	ofstream gusuario;
	ofstream gciudadano;
	ofstream gvacuna;
	ofstream gcarnet;

	switch (dato_guardar) {
	case 1:
		//Usuarios
		gusuario.open("C:/Users/enriq/source/repos/PIAESTDAT/usuario.bin", ios::out | ios::trunc | ios::binary);
		if (gusuario.is_open()) {
			usuario* aux = inicio_uc;
			if (inicio_uc != NULL) {
				while (aux->siguiente != inicio_uc) {
					gusuario.write(reinterpret_cast<char*>(aux), sizeof(usuario));
					aux = aux->siguiente;


				}
				gusuario.write(reinterpret_cast<char*>(aux), sizeof(usuario));
			}
			gusuario.close();
		}
		break;

	case 2:
		//Personas
		gciudadano.open("C:/Users/enriq/source/repos/PIAESTDAT/ciudadanos.bin", ios::out | ios::trunc | ios::binary);
		if (gciudadano.is_open()) {
			persona* aux_c = inicio_c;
			if (inicio_c != NULL && aux_c != NULL) {
				while (aux_c->siguiente != inicio_c) {
					gciudadano.write(reinterpret_cast<char*>(aux_c), sizeof(persona));
					aux_c = aux_c->siguiente;


				}
				gciudadano.write(reinterpret_cast<char*>(aux_c), sizeof(persona));
			}
			gciudadano.close();
		}
		break;

	case 3:
		//vacunas 
		gvacuna.open("C:/Users/enriq/source/repos/PIAESTDAT/vacunas.bin", ios::out | ios::trunc | ios::binary);
		if (gvacuna.is_open()) {
			vacuna* aux_v = inicio_v;
			if (inicio_v != NULL) {
				while (aux_v->siguiente != inicio_v) {
					gvacuna.write(reinterpret_cast<char*>(aux_v), sizeof(vacuna));
					aux_v = aux_v->siguiente;


				}
				gvacuna.write(reinterpret_cast<char*>(aux_v), sizeof(vacuna));
			}
			gvacuna.close();
		}
		break;

	case 4:
		//Carnet (aun no se utliza)
		char aux_fecha[100];
		char aux_fecha2[100];
		/**/gcarnet.open("C:/Users/enriq/source/repos/PIAESTDAT/carnet.bin", ios::out | ios::trunc | ios::binary);
		if (gcarnet.is_open()) {
			int aux_save = 0;
			for (aux_save; aux_save < indice_carnet; aux_save++) {
				gcarnet.write(reinterpret_cast<char*>(&lista_carnet[aux_save]), sizeof(lista_carnet[aux_save]));
			}

			gcarnet.close();
		}
		break;
	}



	dato_guardar = 0;
}
void Cargar_usuarios() {
	ifstream archivo;

	//           C:/Users/enriq/source/repos/PIAESTDAT
	archivo.open("C:/Users/enriq/source/repos/PIAESTDAT/usuario.bin", ios::in | ios::binary);
	if (archivo.is_open())
	{

		usuario* nueva = new usuario;

		archivo.read((char*)nueva, sizeof(usuario));


		while (!archivo.eof())
		{

			anadir(nueva);
			if (archivo.eof()) {
				anadir(nueva);
				nueva = new usuario;
				archivo.read((char*)nueva, sizeof(usuario));

			}
			nueva = new usuario;
			archivo.read((char*)nueva, sizeof(usuario));

		}


		archivo.close();
	}

}

void Cargar_ciudadanos() {
	ifstream archivo;


	archivo.open("C:/Users/enriq/source/repos/PIAESTDAT/ciudadanos.bin", ios::in | ios::binary);
	if (archivo.is_open())
	{

		persona* nueva = new persona;

		archivo.read((char*)nueva, sizeof(persona));


		while (!archivo.eof())
		{

			anadir(nueva);
			if (archivo.eof()) {
				anadir(nueva);
				nueva = new persona;
				archivo.read((char*)nueva, sizeof(persona));

			}
			nueva = new persona;
			archivo.read((char*)nueva, sizeof(persona));

		}


		archivo.close();
	}

}
void Cargar_vacunas()
{
	ifstream archivo;


	archivo.open("C:/Users/enriq/source/repos/PIAESTDAT/vacunas.bin", ios::in | ios::binary);
	if (archivo.is_open())
	{

		vacuna* nueva = new vacuna;

		archivo.read((char*)nueva, sizeof(vacuna));


		while (!archivo.eof())
		{

			anadir(nueva);
			if (archivo.eof()) {
				anadir(nueva);
				nueva = new vacuna;
				archivo.read((char*)nueva, sizeof(vacuna));

			}
			nueva = new vacuna;
			archivo.read((char*)nueva, sizeof(vacuna));

		}


		archivo.close();
	}
}
void Cargar_carnet() {
	ifstream archivo("C:/Users/enriq/source/repos/PIAESTDAT/carnet.bin", ios::in | ios::binary);
	int aux_load = 0;
	char aux_char_1[50], aux_char2[50];
	char aux_fecha_load[100], aux_fecha_load1[100], aux_fecha_load2[100];
	string aux;
	if (archivo.is_open()) {

		while (!archivo.eof()) {
			archivo.read((char*)&lista_carnet[aux_load], sizeof(lista_carnet[aux_load]));
			aux_load++;
			indice_carnet++;
		}
		indice_carnet--;

		archivo.close();
	}
}

void Guardar_indicee() {
	ofstream ID;
	ID.open("C:/Users/enriq/source/repos/PIAESTDAT/Indices.bin", ios::out | ios::binary | ios::trunc);
	if (ID.is_open()) {
		ID << indice_carnet;
		ID.close();
		return;
	}
}
void Cargar_indice() {
	ifstream ID;
	ID.open("C:/Users/enriq/source/repos/PIAESTDAT/Indices.bin", ios::in | ios::binary);
	if (ID.is_open()) {
		ID >> indice_carnet;
		ID.close();
		return;
	}
}

void Guardar_reportes() {
	ofstream greportes;

	greportes.open("C:/Users/enriq/source/repos/PIAESTDAT/reportes.csv", ios::out | ios::trunc | ios::binary);
	if (greportes.is_open()) {
		greportes << ",Nombre,A.Paterno,A.Materno,Edad,Fecha de nacimiento,Sexo,RFC,CURP,Estado Civil,Gpo Ocupacional,Perfil Riesgo";
		greportes << ",Telefono,Calle,Colonia,No. Casa,Municipio,Estado,Marca,No de dosis,Ultima Dosis,Proxima Dosis" << endl;
		//lista_heapquick[i].nombre
		char auxrep_fechas[50];
		for (int i = 0; i < indice_carnet; i++) {
			greportes << lista_heapquick[i].No_carnet;
			greportes << "," << lista_heapquick[i].nombre_persona;
			greportes << "," << lista_heapquick[i].npaterno;
			greportes << "," << lista_heapquick[i].nmaterno;
			greportes << "," << lista_heapquick[i].edad;
			GetDateFormat(NULL, 0, &(lista_heapquick[i].fecha_nac), NULL, auxrep_fechas, sizeof(auxrep_fechas));
			greportes << "," << auxrep_fechas;
			greportes << "," << lista_heapquick[i].sexo;
			greportes << "," << lista_heapquick[i].RFC;
			greportes << "," << lista_heapquick[i].CURP;
			greportes << "," << lista_heapquick[i].e_civil;
			greportes << "," << lista_heapquick[i].Gpo_ocupacional;
			greportes << "," << lista_heapquick[i].Perfil_riesgo;
			greportes << "," << lista_heapquick[i].telefono;
			greportes << "," << lista_heapquick[i].calle;
			greportes << "," << lista_heapquick[i].colonia;
			greportes << "," << lista_heapquick[i].numero;
			greportes << "," << lista_heapquick[i].ciudad;
			greportes << "," << lista_heapquick[i].estado;
			greportes << "," << lista_heapquick[i].vacuna;
			greportes << "," << lista_heapquick[i].No_dosis;
			GetDateFormat(NULL, 0, &(lista_heapquick[i].fecha_aplicacion), NULL, auxrep_fechas, sizeof(auxrep_fechas));
			greportes << "," << auxrep_fechas;
			GetDateFormat(NULL, 0, &(lista_heapquick[i].prox_dosis), NULL, auxrep_fechas, sizeof(auxrep_fechas));
			greportes << "," << auxrep_fechas << endl;
		}

		greportes.close();
	}

}

//BUSQUEDAS
void busqueda_secuencial(char* nombre) {      //Buscar por nombre
	int aux_busqueda_n = 0;
	while (aux_busqueda_n <= indice_carnet - 1) {
		if (strcmp(nombre, lista_carnet[aux_busqueda_n].nombre_persona) == 0) {
			lista_search[mostrar] = lista_carnet[aux_busqueda_n];
			mostrar++;
		}
		aux_busqueda_n++;

	}
}
void busqueda_binaria(int numero) {         //Buscar por carnet
	int aux_id_car = numero;
	//no les estas mandandola variable numero xd
	if (lista_carnet[0].No_carnet != NULL) {
		aux_id_car = indice_carnet + 1;
	}

	if (aux_id_car != 0) {
		bool salir = false;
		int primero = 0;
		int ultimo = indice_carnet;
		int mitad = (primero + ultimo) / 2;
		while (primero <= ultimo || salir == false) {

			if (numero == mitad) {
				valor_busqueda = mitad;
				encontrar = true;
				break;
			}
			else {
				if (mitad < numero) {
					primero = mitad + 1;
				}
				else {
					ultimo = mitad - 1;
				}
			}

			mitad = (primero + ultimo) / 2;

			if (primero > ultimo) {

				salir = true;

			}
		}


	}

}

//METOODOS DE ORDENAMIENTO

//Proceso
void heapify(int i, int f) {
	int raiz = f;
	int l = (2 * f) + 1;
	int r = (2 * f) + 2;
	//por numeros
	if (l < i && lista_heapquick[l].No_carnet>lista_heapquick[raiz].No_carnet) {
		raiz = l;
	}

	if (r < i && lista_heapquick[r].No_carnet>lista_heapquick[raiz].No_carnet) {
		raiz = r;
	}

	if (raiz != f) {
		swap(lista_heapquick[f], lista_heapquick[raiz]);

		heapify(i, raiz);
	}
}
//Recibe parametros
void heapsort() {
	int size = indice_carnet;

	for (int i = (size / 2) - 1; i >= 0; i--) {
		swap(lista_heapquick[0], lista_heapquick[i]);


		heapify(size, i);
	}


	for (int i = (size / 2) - 1; i >= 0; i--) {
		heapify(i, 0);
	}
}


//Proceso
int partition(int bajo, int alto)
{
	int pivote = lista_heapquick[alto].No_carnet;

	int x = (bajo - 1);

	for (int y = bajo; y <= alto - 1; y++) {
		if (strcmp(lista_heapquick[y].nombre_persona, lista_heapquick[pivote].nombre_persona) < 0) {//personas
			/*if (lista_carnet[y].No_carnet < pivote) {*/
			x++;
			swap(lista_heapquick[x], lista_heapquick[y]);
		}
	}
	swap(lista_heapquick[x + 1], lista_heapquick[alto]);
	return (x + 1);

}
//Recibe parametros
void quicksort(int bajo, int alto) {
	if (bajo < alto) {
		int pi = partition(bajo, alto);

		quicksort(bajo, pi - 1);
		quicksort(pi + 1, alto);
	}

}

//Esta es para validar con intento de "recursividad"("?")  
void validar_sesionR(usuario* user, bool existe, char* user_e, char* contrasena, HWND handler) {
	if (inicio_uc != NULL) {
		if (user->siguiente != inicio_uc && existe != true) {
			if (strcmp(user->clave_usuario, user_e) == 0 && strcmp(user->contrasena, contrasena) == 0) {
				existe = true;
				strcpy_s(usuario_activo, user->clave_usuario);
				//	ShowWindow(handler, 0);
				DialogBox(NULL, MAKEINTRESOURCE(V_Bienvenida), handler, (DLGPROC)MenuPrincipal);

			}
			if (existe == false) {
				validar_sesionR(user->siguiente, false, user_e, contrasena, handler);
			}
		}
		else {
			if (strcmp(user->clave_usuario, user_e) == 0 && strcmp(user->contrasena, contrasena) == 0) {
				existe = true;
				strcpy_s(usuario_activo, user->clave_usuario);
				//ShowWindow(handler, 0);
				DialogBox(NULL, MAKEINTRESOURCE(V_Bienvenida), handler, (DLGPROC)MenuPrincipal);

			}
			else {
				MessageBox(NULL, "Inicio erroneo, verifique nuevamente", MB_OK, MB_ICONWARNING);

			}

		}
	}
	else {
		MessageBox(NULL, "No hay usuarios registrados", MB_OK, MB_ICONWARNING);
	}


}
//Si existe nombre_persona de usuario existente 
usuario* validar_dup_usuario(char* user, char* cntra) {
	usuario* aux = inicio_uc;
	int match = NULL;
	int match1 = NULL;
	encontrar_usuario = 0;

	if (inicio_uc != NULL) {
		while (aux->siguiente != inicio_uc && encontrar_usuario == 0) {
			int match = strcmp(aux->clave_usuario, user);
			int match1 = strcmp(aux->contrasena, cntra);
			if (match == 0 || match1 == 0) {
				encontrar_usuario = 1;
			}
			else {
				encontrar_usuario = 0;
				aux = aux->siguiente;
			}


		}

	}

	return aux;
}
//Si hay RFC y/o CURP existente
persona* RFC_CURP(char* RFCURP) {
	persona* aux = inicio_c;
	int match = NULL;
	RFC = 0;

	if (inicio_c != NULL) {
		while (aux->siguiente != inicio_c && RFC == 0) {

			match = strcmp(aux->RFC, RFCURP);

			if (match == 0) {
				RFC = 1;

			}
			else {
				RFC = 0;
				aux = aux->siguiente;
			}


		}
		match = strcmp(aux->RFC, RFCURP);

		if (match == 0) {
			RFC = 1;

		}
		else {
			RFC = 0;
			aux = NULL;
		}

	}
	return aux;
}
persona* CURP_RFC(char* CURPR) {
	persona* aux = inicio_c;
	int match = NULL;
	CURP = 0;



	if (inicio_c != NULL) {
		while (aux->siguiente != inicio_c && CURP == 0) {

			match = strcmp(aux->CURP, CURPR);

			if (match == 0) {
				CURP = 1;
			}
			else {

				CURP = 0;
				aux = aux->siguiente;
			}


		}
		match = strcmp(aux->CURP, CURPR);
		if (match == 0) {
			CURP = 1;

		}
		else {
			CURP = 0;
			aux = NULL;

		}

	}
	return aux;
}
//Si hay marca de vacuna existente 
vacuna* Marca(char* mvac) {
	vacuna* aux = inicio_v;
	int match = NULL;
	char Marca[100];
	dup_vacuna = 0;

	if (inicio_v != NULL) {
		while (aux->siguiente != inicio_v && dup_vacuna == 0) {
			strcpy_s(Marca, aux->marca);
			int match = strcmp(Marca, mvac);

			if (match == 0) {
				dup_vacuna = 1;
			}
			else {
				dup_vacuna = 0;
				aux = aux->siguiente;
			}


		}
		strcpy_s(Marca, aux->marca);
		int match = strcmp(Marca, mvac);

		if (match == 0) {
			dup_vacuna = 1;
		}
		else {
			dup_vacuna = 0;
			aux = aux->siguiente;
		}
	}

	return aux;
}
//REGISTRO PERSONAS
void validar_registro(HWND handler) {//<-----Registro de personas 
	char edad[5];
	char numero[5];
	char telefono[11];
	char Codigo[10];
	char v_seleccion[3];
	SYSTEMTIME fecha_nacimiento;

	//basic_regex<char>RFC("^([A-Z�\x26]{3,4}([0-9]{2})(0[1-9]|1[0-2])(0[1-9]|1[0-9]|2[0-9]|3[0-1]))([A-Z\d]{3})?$");
	//basic_regex<char>CURP("^[A-Z]{1}[AEIOU]{1}[A-Z]{2}[0-9]{2}(0[1-9]|1[0-2])(0[1-9]|1[0-9]|2[0-9]|3[0-1])[HM]{1}(AS|BC|BS|CC|CS|CH|CL|CM|DF|DG|GT|GR|HG|JC|MC|MN|MS|NT|NL|OC|PL|QT|QR|SP|SL|SR|TC|TS|TL|VZ|YN|ZS|NE)[B-DF-HJ-NP-TV-Z]{3}[0-9A-Z]{1}[0-9]{1}$");

	persona* nuevo = new persona();
	SendMessage(GetDlgItem(handler, AP_Nombre), WM_GETTEXT, sizeof(nuevo->nombre_persona) / sizeof(nuevo->nombre_persona[0]), (LPARAM)nuevo->nombre_persona);
	SendMessage(GetDlgItem(handler, AP_ApellidoP), WM_GETTEXT, sizeof(nuevo->paterno) / sizeof(nuevo->paterno[0]), (LPARAM)nuevo->paterno);
	SendMessage(GetDlgItem(handler, AP_ApellidoM), WM_GETTEXT, sizeof(nuevo->materno) / sizeof(nuevo->materno[0]), (LPARAM)nuevo->materno);

	SendMessage(GetDlgItem(handler, AP_Curp), WM_GETTEXT, sizeof(nuevo->CURP) / sizeof(nuevo->CURP[0]), (LPARAM)nuevo->CURP);
	SendMessage(GetDlgItem(handler, AP_Rfc), WM_GETTEXT, sizeof(nuevo->RFC) / sizeof(nuevo->RFC[0]), (LPARAM)nuevo->RFC);

	SendMessage(GetDlgItem(handler, AP_Edad), WM_GETTEXT, sizeof(edad) / sizeof(edad[0]), (LPARAM)edad);//int
	SendMessage(GetDlgItem(handler, AP_Sexo), WM_GETTEXT, sizeof(nuevo->sexo) / sizeof(nuevo->sexo[0]), (LPARAM)nuevo->sexo);
	SendMessage(GetDlgItem(handler, AP_Telefono), WM_GETTEXT, sizeof(nuevo->telefono) / sizeof(nuevo->telefono[0]), (LPARAM)nuevo->telefono);//int
	SendDlgItemMessage(handler, AP_FechaNacimiento, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (fecha_nacimiento));

	if (strchr(nuevo->CURP, 32) || strchr(nuevo->RFC, 32)) {
		MessageBox(handler, "RFC y CURP no debe tener espacio en blanco", "Aviso", MB_OK | MB_ICONEXCLAMATION);
		delete nuevo;
	}
	else {


		if (*nuevo->nombre_persona == NULL || *nuevo->paterno == NULL || *nuevo->materno == NULL || *nuevo->RFC == NULL || *nuevo->CURP == NULL || *nuevo->sexo == NULL
			|| *telefono == NULL || *edad == NULL || strcmp(foto_persona, "") == 0) {//Validar datos personales
			MessageBox(handler, "Faltan datos personales por llenar", "Aviso", MB_OK | MB_ICONEXCLAMATION);
			delete nuevo;
		}

		else {//datos personales
			//if (regex_match(nuevo->nombre_persona, letras) || regex_match(nuevo->paterno, letras) || regex_match(nuevo->materno, letras)) {


			SendMessage(GetDlgItem(handler, AP_Calle), WM_GETTEXT, sizeof(nuevo->calle) / sizeof(nuevo->calle[0]), (LPARAM)nuevo->calle);
			SendMessage(GetDlgItem(handler, AP_Numero), WM_GETTEXT, sizeof(numero) / sizeof(numero[0]), (LPARAM)numero);//int 
			SendMessage(GetDlgItem(handler, AP_Colonia), WM_GETTEXT, sizeof(nuevo->colonia) / sizeof(nuevo->colonia[0]), (LPARAM)nuevo->colonia);
			/*ExMunicipio*/ SendMessage(GetDlgItem(handler, AP_Ciudad), WM_GETTEXT, sizeof(nuevo->ciudad) / sizeof(nuevo->ciudad[0]), (LPARAM)nuevo->ciudad);
			SendMessage(GetDlgItem(handler, AP_Estado), WM_GETTEXT, sizeof(nuevo->estado) / sizeof(nuevo->estado[0]), (LPARAM)nuevo->estado);
			SendMessage(GetDlgItem(handler, AP_CodigoPostal), WM_GETTEXT, sizeof(Codigo) / sizeof(Codigo[0]), (LPARAM)Codigo);//int

			if (*nuevo->calle == NULL || *numero == NULL || *nuevo->colonia == NULL || *nuevo->ciudad == NULL || *nuevo->estado == NULL || *Codigo == NULL) {/*validar domicilio*/
				MessageBox(handler, "Faltan datos del domicilio por llenar", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				delete nuevo;


			}
			else {//domicilio
				strcpy_s(validar_contexto, nuevo->RFC);
				strcpy_s(validar_contexto2, nuevo->CURP);
				persona* RYC = RFC_CURP(validar_contexto2);
				persona* CYR = RFC_CURP(validar_contexto2);
				//	if (regex_match(nuevo->RFC, RFC) || regex_match(nuevo->CURP, CURP)) {
				if (CURP == 1) {
					MessageBox(handler, "CURP ya existente", "Aviso", MB_OK | MB_ICONEXCLAMATION);
					delete nuevo;
				}

				else if (RFC == 1) {
					MessageBox(handler, "RFC ya existente", "Aviso", MB_OK | MB_ICONEXCLAMATION);
					delete nuevo;
				}
				else {
					SendMessage(GetDlgItem(handler, AP_EstadoCivil), WM_GETTEXT, sizeof(nuevo->e_civil) / sizeof(nuevo->e_civil[0]), (LPARAM)nuevo->e_civil);
					SendMessage(GetDlgItem(handler, AP_GpoOcupacional), WM_GETTEXT, sizeof(nuevo->Gpo_ocupacional) / sizeof(nuevo->Gpo_ocupacional[0]), (LPARAM)nuevo->Gpo_ocupacional);
					SendMessage(GetDlgItem(handler, AP_PerfilRiesgo), WM_GETTEXT, sizeof(nuevo->Perfil_riesgo) / sizeof(nuevo->Perfil_riesgo[0]), (LPARAM)nuevo->Perfil_riesgo);
					SendMessage(GetDlgItem(handler, AP_Telefono), WM_GETTEXT, sizeof(nuevo->telefono) / sizeof(nuevo->telefono[0]), (LPARAM)nuevo->telefono);
					SendMessage(GetDlgItem(handler, AP_Sexo), WM_GETTEXT, sizeof(nuevo->sexo) / sizeof(nuevo->sexo[0]), (LPARAM)nuevo->sexo);

					SendDlgItemMessage(handler, AP_FechaNacimiento, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (nuevo->nacimiento));


					if (regex_match(nuevo->telefono, n_tel) && regex_match(edad, numeros)) {

						nuevo->edad = atoi(edad);
						nuevo->numero = atoi(numero);
						nuevo->codigo_postal = atoi(Codigo);
						strcpy_s(nuevo->foto_c, foto_persona);
						anadir(nuevo);
						dato_guardar = guardar_ciudadanos;
						dato_limpiar = guardar_ciudadanos;
						Guardar_usuarios();
						limpiar();
						Cargar_ciudadanos();
						MessageBox(handler, "Informacion registrada correctamente", "Aviso", MB_OK | MB_ICONINFORMATION);
						strcpy_s(foto_persona, "");
						EndDialog(handler, 0);
					}
					else {
						MessageBox(handler, "Telefono debe ser 10 digitos y solo numeros\nEdad debe ser solo numeros", "Aviso", MB_OK | MB_ICONEXCLAMATION);
						delete nuevo;
					}



				}
			}
		}

	}
}
void validar_vacuna(HWND handler) {
	vacuna* nuevo = new vacuna();
	char v_lote[10];
	char _vprecio[20];
	char c_dosis[9]; //Cantidad de dosis necesarias
	char r_dosis[60];  //Vacunas Recibidas
	char v_tipo[30];


	SendMessage(GetDlgItem(handler, AV_Clave), WM_GETTEXT, sizeof(v_lote) / sizeof(v_lote[0]), (LPARAM)v_lote);
	SendMessage(GetDlgItem(handler, AV_Descripcion), WM_GETTEXT, sizeof(nuevo->descripcion) / sizeof(nuevo->descripcion[0]), (LPARAM)nuevo->descripcion);
	SendMessage(GetDlgItem(handler, AV_Marca), WM_GETTEXT, sizeof(nuevo->marca) / sizeof(nuevo->marca[0]), (LPARAM)nuevo->marca);
	SendMessage(GetDlgItem(handler, AV_Precio), WM_GETTEXT, sizeof(_vprecio) / sizeof(_vprecio[0]), (LPARAM)_vprecio);
	SendMessage(GetDlgItem(handler, AV_Dosis), WM_GETTEXT, sizeof(c_dosis) / sizeof(c_dosis[0]), (LPARAM)c_dosis); //Cantidad dosis
	SendMessage(GetDlgItem(handler, AV_VacunasRecibidas), WM_GETTEXT, sizeof(r_dosis) / sizeof(r_dosis[0]), (LPARAM)r_dosis);  //Vacunas Recibidas
	SendMessage(GetDlgItem(handler, AV_Tipo), WM_GETTEXT, sizeof(nuevo->tipo) / sizeof(nuevo->tipo[0]), (LPARAM)nuevo->tipo);

	if (*v_lote == NULL || *nuevo->descripcion == NULL || *nuevo->marca == NULL || *_vprecio == NULL || *c_dosis == NULL || *r_dosis == NULL || *nuevo->tipo == NULL) {
		MessageBox(handler, "Debe llenar todos los campos", "Aviso", MB_OK | MB_ICONEXCLAMATION);
		delete nuevo;
	}
	else {
		strcpy_s(validar_contexto, nuevo->marca);
		vacuna* validar = Marca(validar_contexto);
		if (dup_vacuna != 1) {
			if (regex_match(c_dosis, numeros) && regex_match(r_dosis, numeros)) {//regex
				nuevo->cantidad_dosis = atoi(c_dosis);
				nuevo->dosis_requeridas = atoi(r_dosis);
				strcpy_s(nuevo->numero_lote, v_lote);

				if (regex_match(_vprecio, costo)) {//validar precio
					nuevo->precio = stof(_vprecio);
					if (nuevo->precio <= 0) {
						MessageBox(handler, "El precio debe ser mayor a 0.00 y solo numeros", "Aviso", MB_OK | MB_ICONEXCLAMATION);
						delete nuevo;
					}
					else {
						if (nuevo->cantidad_dosis <= 0) {//cantidad
							MessageBox(handler, "*Precio\n*Numero de dosis\n*Vacunas Recibidas\n debe ser minimo 1", "Aviso", MB_OK | MB_ICONEXCLAMATION);
							delete nuevo;
						}
						else {

							if (dup_vacuna == 1) {
								MessageBox(handler, "Marca de vacuna ya registrada", "Aviso", MB_OK | MB_ICONEXCLAMATION);
								delete nuevo;
							}
							else {
								anadir(nuevo);
								dato_guardar = 3;
								Guardar_usuarios();
								dato_limpiar = 3;
								limpiar();
								Cargar_vacunas();
								MessageBox(handler, "Vacuna registrada correctamente", "Aviso", MB_OK | MB_ICONINFORMATION);
								EndDialog(handler, 0);
							}
						}

					}//cantidad

				}//precio
				else {

					MessageBox(handler, "El precio de la vacuna deben ser solo numeros y 2 decimales\nEjemplo: 0.00", "Aviso", MB_OK | MB_ICONEXCLAMATION);
					delete nuevo;
				}
			}//regex
			else {
				MessageBox(handler, "Los siguientes campos deben tener valores numericos\n->Clave\n->Numero de dosis\n->Vaunas recibidas", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				delete nuevo;
			}
		}
		else {
			MessageBox(handler, "Marca de vacuna ya existente", "Aviso", MB_OK | MB_ICONEXCLAMATION);
		}



	}

}
//ACTUALIZAR VACUNAS
void actualizar(HWND handler, vacuna* actualizar) {
	char act_precio[10], canti_dosis[4], req_dosis[4];
	SendMessage(GetDlgItem(handler, CV_Clave), WM_GETTEXT, sizeof(actualizar->numero_lote) / sizeof(actualizar->numero_lote[0]), (LPARAM)actualizar->numero_lote);
	SendMessage(GetDlgItem(handler, CV_Descripcion), WM_GETTEXT, sizeof(actualizar->descripcion) / sizeof(actualizar->descripcion[0]), (LPARAM)actualizar->descripcion);
	SendMessage(GetDlgItem(handler, CV_Marca), WM_GETTEXT, sizeof(actualizar->marca) / sizeof(actualizar->marca[0]), (LPARAM)actualizar->marca);
	SendMessage(GetDlgItem(handler, CV_Precio), WM_GETTEXT, sizeof(act_precio) / sizeof(act_precio[0]), (LPARAM)act_precio);
	SendMessage(GetDlgItem(handler, CV_VacunasImp), WM_GETTEXT, sizeof(canti_dosis) / sizeof(canti_dosis[0]), (LPARAM)canti_dosis);
	SendMessage(GetDlgItem(handler, CV_DosisReq), WM_GETTEXT, sizeof(req_dosis) / sizeof(req_dosis[0]), (LPARAM)req_dosis);
	SendMessage(GetDlgItem(handler, CV_Tipo), WM_GETTEXT, sizeof(actualizar->tipo) / sizeof(actualizar->tipo[0]), (LPARAM)actualizar->tipo);
	if (*actualizar->numero_lote == NULL || *actualizar->descripcion == NULL || *actualizar->marca == NULL || *act_precio == NULL || *canti_dosis == NULL || *req_dosis == NULL || *actualizar->tipo == NULL)
	{
		MessageBox(handler, "Los campos deben estar llenos", "Aviso", MB_OK | MB_ICONWARNING);
		dato_limpiar = guardar_vacunas;
		limpiar();
		Cargar_vacunas();
	}
	else
	{

		actualizar->cantidad_dosis = atoi(canti_dosis);
		actualizar->dosis_requeridas = atoi(req_dosis);
		actualizar->precio = stof(act_precio);
		if (actualizar->cantidad_dosis <= 0 || actualizar->dosis_requeridas <= 0 || actualizar->precio <= 0) {
			MessageBox(handler, "Los valores deben ser mayores a 0", "Aviso", MB_OK | MB_ICONWARNING);
			dato_limpiar = guardar_vacunas;
			limpiar();
			Cargar_vacunas();
		}
		else {
			dato_guardar = 3;
		}

	}

}
//ACTUALIZAR PERSONAS
void actualizar(HWND handler, persona* actualizar) {
	char edad[4];
	char numero[5];
	char Codigo[10];
	char Fecha[30];

	SendMessage(GetDlgItem(handler, CP_Nombre), WM_GETTEXT, sizeof(actualizar->nombre_persona) / sizeof(actualizar->nombre_persona[0]), (LPARAM)actualizar->nombre_persona);
	SendMessage(GetDlgItem(handler, CP_ApellidoP), WM_GETTEXT, sizeof(actualizar->paterno) / sizeof(actualizar->paterno[0]), (LPARAM)actualizar->paterno);
	SendMessage(GetDlgItem(handler, CP_ApellidoM), WM_GETTEXT, sizeof(actualizar->materno) / sizeof(actualizar->materno[0]), (LPARAM)actualizar->materno);


	if (regex_match(actualizar->nombre_persona, letras) && regex_match(actualizar->paterno, letras) && regex_match(actualizar->materno, letras)) {

		SendMessage(GetDlgItem(handler, CP_Calle), WM_GETTEXT, sizeof(actualizar->calle) / sizeof(actualizar->calle[0]), (LPARAM)actualizar->calle);
		SendMessage(GetDlgItem(handler, CP_Colonia), WM_GETTEXT, sizeof(actualizar->colonia) / sizeof(actualizar->colonia[0]), (LPARAM)actualizar->colonia);
		SendMessage(GetDlgItem(handler, CP_Ciudad), WM_GETTEXT, sizeof(actualizar->ciudad) / sizeof(actualizar->ciudad[0]), (LPARAM)actualizar->ciudad);
		SendMessage(GetDlgItem(handler, CP_Estado), WM_GETTEXT, sizeof(actualizar->estado) / sizeof(actualizar->estado[0]), (LPARAM)actualizar->estado);

		if (*actualizar->calle == NULL || *actualizar->colonia == NULL || *actualizar->ciudad == NULL || *actualizar->estado == NULL) {
			MessageBox(handler, "Campos\n->Calle\n->Colonia\n->Ciudad\->Estado\nEstan vacios, revise nuevamente", "Aviso", MB_OK | MB_ICONEXCLAMATION);
			dato_limpiar = guardar_ciudadanos;
			limpiar();
			Cargar_ciudadanos();
		}
		else {
			SendMessage(GetDlgItem(handler, CP_Edad), WM_GETTEXT, sizeof(edad) / sizeof(edad[0]), (LPARAM)edad);//int
			actualizar->edad = atoi(edad);
			SendMessage(GetDlgItem(handler, CP_Numero), WM_GETTEXT, sizeof(numero) / sizeof(numero[0]), (LPARAM)numero);//int
			actualizar->numero = atoi(numero);
			SendMessage(GetDlgItem(handler, CP_CodigoPostal), WM_GETTEXT, sizeof(Codigo) / sizeof(Codigo[0]), (LPARAM)Codigo);//int
			actualizar->codigo_postal = atoi(Codigo);
			SendMessage(GetDlgItem(handler, CP_Telefono), WM_GETTEXT, sizeof(actualizar->telefono) / sizeof(actualizar->telefono[0]), (LPARAM)actualizar->telefono);


			if (regex_match(actualizar->telefono, n_tel)) {
				SendMessage(GetDlgItem(handler, CP_Telefono), WM_GETTEXT, sizeof(actualizar->telefono) / sizeof(actualizar->telefono[0]), (LPARAM)actualizar->telefono);//int

				SendDlgItemMessage(handler, CP_FechaNacimiento, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (actualizar->nacimiento));
				SendMessage(GetDlgItem(handler, CP_GrupoOc), WM_GETTEXT, sizeof(actualizar->Gpo_ocupacional) / sizeof(actualizar->Gpo_ocupacional[0]), (LPARAM)actualizar->Gpo_ocupacional);//int
				SendMessage(GetDlgItem(handler, CP_PerfilRiesgo), WM_GETTEXT, sizeof(actualizar->Perfil_riesgo) / sizeof(actualizar->Perfil_riesgo[0]), (LPARAM)actualizar->Perfil_riesgo);//int
				SendMessage(GetDlgItem(handler, CP_Sexo), WM_GETTEXT, sizeof(actualizar->sexo) / sizeof(actualizar->sexo[0]), (LPARAM)actualizar->sexo);
				if (*actualizar->sexo == NULL || *actualizar->Perfil_riesgo == NULL || *actualizar->Gpo_ocupacional == NULL) {
					MessageBox(handler, "Debe llenar todos los campos", "Aviso", MB_OK | MB_ICONINFORMATION);
				}
				dato_limpiar = guardar_ciudadanos;
				dato_guardar = guardar_ciudadanos;
				Guardar_usuarios();
				limpiar();
				Cargar_ciudadanos();
				MessageBox(handler, "Informacion Actualizada", "Aviso", MB_OK | MB_ICONINFORMATION);
			}
			else {
				MessageBox(handler, "El numero de telefono deben de ser 10 digitos", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				dato_limpiar = guardar_ciudadanos;
				limpiar();
				Cargar_ciudadanos();

			}
		}

	}
	else {
		MessageBox(handler, "Los campos siguentes:\n->Nombre\n->A.Paterno\nA.Materno\nDeben contenener minimo un caracter y solo letras", "Aviso", MB_OK | MB_ICONEXCLAMATION);
		dato_limpiar = guardar_ciudadanos;
		limpiar();
		Cargar_ciudadanos();
	}






}
//CONFIRMAR REGISTRO USUARIO
void confirmar_registro(HWND handler) {
	usuario* nuevo = new usuario();
	char n_usuario[40];
	char n_contrase�a[40];

	SendMessage(GetDlgItem(handler, AU_Nombre), WM_GETTEXT, sizeof(nuevo->nombre_usuario) / sizeof(nuevo->nombre_usuario[0]), (LPARAM)nuevo->nombre_usuario);
	SendMessage(GetDlgItem(handler, AU_ApellidoP), WM_GETTEXT, sizeof(nuevo->A_Paterno) / sizeof(nuevo->A_Paterno[0]), (LPARAM)nuevo->A_Paterno);
	SendMessage(GetDlgItem(handler, AU_ApellidoM), WM_GETTEXT, sizeof(nuevo->A_Materno) / sizeof(nuevo->A_Materno[0]), (LPARAM)nuevo->A_Materno);

	SendMessage(GetDlgItem(handler, AU_Usuario), WM_GETTEXT, sizeof(n_usuario) / sizeof(n_usuario[0]), (LPARAM)n_usuario);
	SendMessage(GetDlgItem(handler, AU_Contrasena), WM_GETTEXT, sizeof(n_contrase�a) / sizeof(n_contrase�a[0]), (LPARAM)n_contrase�a);

	if (*n_usuario == NULL || *n_contrase�a == NULL || *nuevo->A_Paterno == NULL || *nuevo->A_Materno == NULL || *nuevo->nombre_usuario == NULL) {
		MessageBox(handler, "Debe llenar todos los campos", "Aviso", MB_OK | MB_ICONEXCLAMATION);
		delete nuevo;
	}
	else {


		if (strchr(n_usuario, 32)) {

			MessageBox(handler, "Usuario no debe tener saltos de linea", "Aviso", MB_OK | MB_ICONWARNING);
			delete nuevo;
		}
		else {


			inicio_uc = validar_dup_usuario(n_usuario, n_contrase�a);

			if (encontrar_usuario == 1) {
				MessageBox(handler, "Usuario ya existente", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				delete nuevo;
			}
			else {
				strcpy_s(nuevo->clave_usuario, n_usuario);
				strcpy_s(nuevo->contrasena, n_contrase�a);

				anadir(nuevo);
				dato_guardar = guardar_usuarios;
				Guardar_usuarios();
				dato_limpiar = guardar_usuarios;
				limpiar();
				Cargar_usuarios();

				//MessageBox(handler, "Usuario registrado correctamente", "Exito", MB_OK | MB_ICONINFORMATION);

				EndDialog(handler, 0);
			}
		}
	}
}
//LOGIN
BOOL CALLBACK Inicio_Sesion(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {
	switch (mensaje) {

	case WM_INITDIALOG: {
		Cargar_usuarios();
		Cargar_vacunas();
		Cargar_ciudadanos();
		Cargar_indice();
		Cargar_carnet();
			
		break;
	}
	case WM_COMMAND: {
		int interacion = LOWORD(wParam);
		switch (interacion) {
		case B_Login: {
			usuario* aux = inicio_uc;
			char validar_usuario[25];
			char validar_contrasena[25];
			SendDlgItemMessage(handler, Login_Usuario, WM_GETTEXT, sizeof(validar_usuario) / sizeof(validar_usuario[0]), (LPARAM)validar_usuario);
			SendDlgItemMessage(handler, Login_Contrasena, WM_GETTEXT, sizeof(validar_contrasena) / sizeof(validar_contrasena[0]), (LPARAM)validar_contrasena);
			validar_sesionR(aux, false, validar_usuario, validar_contrasena, handler);

			break;
		}
		case B_RegistroUsuario: {
			DialogBox(NULL, MAKEINTRESOURCE(V_Registro), handler, (DLGPROC)Registro_Usuario);

			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		}
		break;
	}
	}
	return false;
}
BOOL CALLBACK Registro_Usuario(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {
	switch (mensaje) {
	case WM_COMMAND: {
		int interacion = LOWORD(wParam);
		switch (interacion) {

		case AU_Guardar: {
			confirmar_registro(handler);
			break;
		}
		case WM_DESTROY: {
			EndDialog(handler, 0);
			break;
		}
		}
		break;
	}
	}
	return false;
}
BOOL CALLBACK MenuPrincipal(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {
	//ShowWindow(handler, 1);
	switch (mensaje) {
	case WM_INITDIALOG: {
		char texto_1[] = "Registro Control COVID-19 ";
		char texto_com[256];
		strcpy_s(texto_com, texto_1);
		strcat_s(texto_com, " - Usuario activo: ");
		strcat_s(texto_com, usuario_activo);
		SetWindowText(handler, texto_com);

		break;
	}

	case WM_COMMAND: {
		int interacion = LOWORD(wParam);
		switch (interacion) {
		case ID_Salir: {
			EndDialog(handler, 0);
			break;
		}
		case WM_DESTROY: {
			EndDialog(handler, 0);
			break;

		}
					   //ALTAS
		case ID_REGISTRO_PERSONA: {
			DialogBox(NULL, MAKEINTRESOURCE(V_RegistrarPersona), handler, (DLGPROC)Registro_Personas);
			break;
		}
		case ID_REGISTRO_VACUNA: {
			DialogBox(NULL, MAKEINTRESOURCE(V_RegistrarVacuna), handler, (DLGPROC)Registro_Vacunas);
			break;
		}
		case ID_REGISTRO_CARNET: {
			DialogBox(NULL, MAKEINTRESOURCE(V_RegistrarCarnet), handler, (DLGPROC)Registro_Carnet);
			break;
		}
							   //BAJAS
		case ID_BAJA_VACUNA: {
			DialogBox(NULL, MAKEINTRESOURCE(V_EliminarVacuna), handler, (DLGPROC)Eliminar_Vacunas);
			break;
		}
		case ID_BAJA_CARNET: {
			DialogBox(NULL, MAKEINTRESOURCE(V_EliminarCarnet), handler, (DLGPROC)Eliminar_Carnet);
			break;
		}

						   //CAMBIOS
		case ID_MODIFICAR_PERSONA: {
			DialogBox(NULL, MAKEINTRESOURCE(V_EditarPersona), handler, (DLGPROC)Editar_Personas);
			break;
		}
		case ID_MODIFICAR_VACUNA: {
			DialogBox(NULL, MAKEINTRESOURCE(V_EditarVacuna), handler, (DLGPROC)Editar_Vacunas);
			break;
		}
		case ID_MODIFICAR_CARNET: {
			DialogBox(NULL, MAKEINTRESOURCE(V_EditarCarnet), handler, (DLGPROC)Editar_Carnet);
			break;
		}
								//REPORTE Y BUSQUEDA
 case ID_REPORTE: {
		 DialogBox(NULL, MAKEINTRESOURCE(V_ReportesPersonas), handler, (DLGPROC)Reportes);
			break;
	  }
		case ID_BUSCAR_CARNET: {
			DialogBox(NULL, MAKEINTRESOURCE(V_BusquedaCarnet), handler, (DLGPROC)Buscar);
			break;
		}
		}
	}
				   break;
	}
	return false;
}
//ALTAS
BOOL CALLBACK Registro_Personas(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {
	switch (mensaje) {
	case WM_INITDIALOG: {
		SendMessage(GetDlgItem(handler, AP_EstadoCivil), CB_ADDSTRING, NULL, (LPARAM)"Soltero/a");
		SendMessage(GetDlgItem(handler, AP_EstadoCivil), CB_ADDSTRING, NULL, (LPARAM)"Casado/a");
		SendMessage(GetDlgItem(handler, AP_EstadoCivil), CB_ADDSTRING, NULL, (LPARAM)"Union Libre");
		SendMessage(GetDlgItem(handler, AP_EstadoCivil), CB_ADDSTRING, NULL, (LPARAM)"Separado/a");
		SendMessage(GetDlgItem(handler, AP_EstadoCivil), CB_ADDSTRING, NULL, (LPARAM)"Divorciado/a");
		SendMessage(GetDlgItem(handler, AP_EstadoCivil), CB_ADDSTRING, NULL, (LPARAM)"Viudo/a");

		SendMessage(GetDlgItem(handler, AP_Sexo), CB_ADDSTRING, NULL, (LPARAM)"Masculino");
		SendMessage(GetDlgItem(handler, AP_Sexo), CB_ADDSTRING, NULL, (LPARAM)"Femenino");
		SendMessage(GetDlgItem(handler, AP_Sexo), CB_ADDSTRING, NULL, (LPARAM)"Otro");


		char texto_1[] = "Registro personas ";
		char texto_com[256];
		strcpy_s(texto_com, texto_1);
		strcat_s(texto_com, " - Usuario activo: ");
		strcat_s(texto_com, usuario_activo);
		SetWindowText(handler, texto_com);


		break;
	}
	case WM_COMMAND: {
		int interacion = LOWORD(wParam);
		switch (interacion) {

		case WM_DESTROY: {
			EndDialog(handler, 0);
			break;
		}
		case AP_Examinar: {
			OPENFILENAME abrir;
			ZeroMemory(&abrir, sizeof(abrir));
			abrir.lStructSize = sizeof(abrir);
			abrir.hwndOwner = handler;
			abrir.lpstrFile = foto_persona;
			abrir.lpstrFilter = "Mapa de Bits\0*.bmp*\0Imagen PNG\0*.png*\0Imagen JPG\0*.jpg*";
			abrir.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
			abrir.lpstrFile[0] = NULL;
			abrir.nMaxFile = sizeof(abrir);
			abrir.nFilterIndex = 0;
			if (GetOpenFileName(&abrir) == TRUE) {

				HBITMAP foto = (HBITMAP)LoadImage(NULL, foto_persona, IMAGE_BITMAP, 256, 256, LR_LOADFROMFILE);
				SendMessage(GetDlgItem(handler, AP_Foto), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);

			}
			break;
		}
		case AP_Guardar: {
			validar_registro(handler);
			break;
		}
		}
		break;
	}
	}

	return false;

}
BOOL CALLBACK Registro_Vacunas(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {
	switch (mensaje) {

	case WM_INITDIALOG: {
		SendMessage(GetDlgItem(handler, AV_Tipo), CB_ADDSTRING, NULL, (LPARAM)"ARN mensajero");
		SendMessage(GetDlgItem(handler, AV_Tipo), CB_ADDSTRING, NULL, (LPARAM)"Subunidades proteicas");
		SendMessage(GetDlgItem(handler, AV_Tipo), CB_ADDSTRING, NULL, (LPARAM)"Vector viral");


		char texto_1[] = "Registro vacuna ";
		char texto_com[256];
		strcpy_s(texto_com, texto_1);
		strcat_s(texto_com, " - Usuario activo: ");
		strcat_s(texto_com, usuario_activo);
		SetWindowText(handler, texto_com);


		break;
	}

	case WM_COMMAND: {
		int interacion = LOWORD(wParam);
		switch (interacion) {
		case WM_DESTROY: {
			EndDialog(handler, 0);
			break;
		}
		case AV_Guardar: {
			validar_vacuna(handler);
			break;
		}
		}
		break;
	}
	}

	return false;

}
BOOL CALLBACK Registro_Carnet(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {
	char n_dosi[5];
	switch (mensaje) {

	case WM_INITDIALOG: {
		char texto_1[] = "Nuevo Carnet ";
		char texto_com[256];
		strcpy_s(texto_com, texto_1);
		strcat_s(texto_com, " - Usuario activo: ");
		strcat_s(texto_com, usuario_activo);
		SetWindowText(handler, texto_com);

		if (inicio_v != NULL) {
			vacuna* lista = inicio_v;

			if (lista->siguiente != inicio_v) {
				while (lista->siguiente != inicio_v) {
					SendMessage(GetDlgItem(handler, AC_Vacuna), CB_ADDSTRING, NULL, (LPARAM)lista->marca);
					lista = lista->siguiente;
				}
			}
			else {
				SendMessage(GetDlgItem(handler, AC_Vacuna), CB_ADDSTRING, NULL, (LPARAM)lista->marca);
			}
		}
		break;

	}
	case WM_COMMAND: {
		char CURP[35];
		char Edad[10];
		char Fecha[1024];
		int interacion = LOWORD(wParam);
		switch (interacion) {
		case WM_DESTROY: {
			EndDialog(handler, 0);
			break;
		}
		case AC_BuscaCURP: {
			SendDlgItemMessage(handler, AC_CURP, WM_GETTEXT, sizeof(CURP) / sizeof(CURP[0]), (LPARAM)CURP);
			if (*CURP != NULL) {
				persona* mostrar_CURP = CURP_RFC(CURP);
				lista = mostrar_CURP;
				if (mostrar_CURP != NULL) {

					SendMessage(GetDlgItem(handler, AC_Nombre), WM_SETTEXT, sizeof(mostrar_CURP->nombre_persona) / sizeof(mostrar_CURP->nombre_persona[0]), (LPARAM)mostrar_CURP->nombre_persona);
					SendMessage(GetDlgItem(handler, AC_ApellidoP), WM_SETTEXT, sizeof(mostrar_CURP->paterno) / sizeof(mostrar_CURP->paterno[0]), (LPARAM)mostrar_CURP->paterno);
					SendMessage(GetDlgItem(handler, AC_ApellidoM), WM_SETTEXT, sizeof(mostrar_CURP->materno) / sizeof(mostrar_CURP->materno[0]), (LPARAM)mostrar_CURP->materno);
					_itoa_s(mostrar_CURP->edad, Edad, sizeof(Edad));
					SendMessage(GetDlgItem(handler, AC_Edad), WM_SETTEXT, sizeof(Edad) / sizeof(Edad[0]), (LPARAM)Edad);
					GetDateFormat(NULL, 0, &(mostrar_CURP->nacimiento), NULL, Fecha, 1024);
					SendMessage(GetDlgItem(handler, AC_FechaNacimiento), WM_SETTEXT, sizeof(Fecha) / sizeof(Fecha[0]), (LPARAM)Fecha);
					SendMessage(GetDlgItem(handler, AC_Sexo), WM_SETTEXT, sizeof(mostrar_CURP->sexo) / sizeof(mostrar_CURP->sexo[0]), (LPARAM)mostrar_CURP->sexo);
					HBITMAP foto = (HBITMAP)LoadImage(NULL, mostrar_CURP->foto_c, IMAGE_BITMAP, 256, 256, LR_LOADFROMFILE);
					strcpy_s(aux_foto, mostrar_CURP->foto_c);
					SendMessage(GetDlgItem(handler, AC_Foto), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);
				}
				else {
					MessageBox(handler, "No existe CURP ingresado", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				}
			}
			else {
				MessageBox(handler, "Ingrese CURP para busqueda", "Aviso", MB_OK | MB_ICONEXCLAMATION);

			}
			break;
		}
		case AC_Registrar: {
			char aux_lugar[255], aux_vacuna[50], aux_tipo[50], aux_nombre[50];
			SendDlgItemMessage(handler, AC_CURP, WM_GETTEXT, sizeof(CURP) / sizeof(CURP[0]), (LPARAM)CURP);
			SendMessage(GetDlgItem(handler, AC_Lugar), WM_GETTEXT, sizeof(aux_lugar) / sizeof(aux_lugar[0]), (LPARAM)aux_lugar);
			//lista_carnet[indice_carnet].No_dosis = atoi(n_dosi);
			SendMessage(GetDlgItem(handler, AC_Vacuna), WM_GETTEXT, sizeof(aux_vacuna) / sizeof(aux_vacuna[0]), (LPARAM)aux_vacuna);
			SendMessage(GetDlgItem(handler, AC_TipoVacuna), WM_GETTEXT, sizeof(aux_tipo) / sizeof(aux_tipo[0]), (LPARAM)aux_tipo);
			SendMessage(GetDlgItem(handler, AC_Dosis), WM_GETTEXT, sizeof(n_dosi) / sizeof(n_dosi[0]), (LPARAM)n_dosi);
			SendMessage(GetDlgItem(handler, AC_Nombre), WM_GETTEXT, sizeof(aux_nombre) / sizeof(aux_nombre[0]), (LPARAM)aux_nombre);
			if (strcmp(aux_nombre, "") == 0) {
				MessageBox(handler, "No hay persona ingresada para registrar carnet", "Aviso", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				//SendDlgItemMessage(handler, RNC_CURP, WM_GETTEXT, sizeof(CURP) / sizeof(CURP[0]), (LPARAM)CURP);
				if (*CURP != NULL || *aux_lugar != NULL || aux_vacuna != NULL || strcmp(aux_tipo, "") == 0 || *n_dosi != NULL) {
					SYSTEMTIME fecha_actual;
					SYSTEMTIME fecha_apl;
					SYSTEMTIME prox_dosis;

					SendDlgItemMessage(handler, AC_FechaDosisActual, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (fecha_apl));
					SendDlgItemMessage(handler, AC_FechaDosisSiguiente, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (prox_dosis));
					GetLocalTime(&fecha_actual);

					long long diferencia_t = 0;
					long long diferencia_t2 = 0;
					long long diferencia_t3 = 0;

					//diferencia_t = tiempo(fecha_apl) - tiempo(fecha_actual);
					diferencia_t2 = tiempo(prox_dosis) - tiempo(fecha_actual);

					if (diferencia_t2 <= 0) {
						MessageBox(NULL, "Fecha de proxima dosis\nno debe ser anterior a la fecha actual", "Error", MB_OK | MB_ICONERROR);
					}
					else {

						diferencia_t3 = tiempo(prox_dosis) - tiempo(fecha_apl);
						if (diferencia_t3 <= 0) {
							MessageBox(NULL, "Fecha prox dosis no debe ser menor a la de aplicacion\nno debe ser anterior a la fecha actual", "Error", MB_OK | MB_ICONERROR);

						}
						else {

							lista_carnet[indice_carnet].No_carnet = indice_carnet + 1;
							strcpy_s(lista_carnet[indice_carnet].CURP, CURP);
							SendDlgItemMessage(handler, AC_CURP, WM_GETTEXT, sizeof(lista_carnet[indice_carnet].CURP) / sizeof(lista_carnet[indice_carnet].CURP[0]), (LPARAM)lista_carnet[indice_carnet].CURP);
							/*	*/
							SendDlgItemMessage(handler, AC_Nombre, WM_GETTEXT, sizeof(lista_carnet[indice_carnet].nombre_persona) / sizeof(lista_carnet[indice_carnet].nombre_persona[0]), (LPARAM)lista_carnet[indice_carnet].nombre_persona);
							SendMessage(GetDlgItem(handler, AC_ApellidoP), WM_GETTEXT, sizeof(lista_carnet[indice_carnet].npaterno) / sizeof(lista_carnet[indice_carnet].npaterno[0]), (LPARAM)lista_carnet[indice_carnet].npaterno);
							SendMessage(GetDlgItem(handler, AC_ApellidoM), WM_GETTEXT, sizeof(lista_carnet[indice_carnet].nmaterno) / sizeof(lista_carnet[indice_carnet].nmaterno[0]), (LPARAM)lista_carnet[indice_carnet].nmaterno);
							SendMessage(GetDlgItem(handler, AC_Edad), WM_GETTEXT, sizeof(Edad) / sizeof(Edad[0]), (LPARAM)Edad);
							lista_carnet[indice_carnet].edad = atoi(Edad);
							SendMessage(GetDlgItem(handler, AC_Sexo), WM_GETTEXT, sizeof(lista_carnet[indice_carnet].sexo) / sizeof(lista_carnet[indice_carnet].sexo[0]), (LPARAM)lista_carnet[indice_carnet].sexo);

							SendMessage(GetDlgItem(handler, AC_Lugar), WM_GETTEXT, sizeof(lista_carnet[indice_carnet].lugar) / sizeof(lista_carnet[indice_carnet].lugar[0]), (LPARAM)lista_carnet[indice_carnet].lugar);
							lista_carnet[indice_carnet].No_dosis = atoi(n_dosi);
							SendMessage(GetDlgItem(handler, AC_Vacuna), WM_GETTEXT, sizeof(lista_carnet[indice_carnet].vacuna) / sizeof(lista_carnet[indice_carnet].vacuna[0]), (LPARAM)lista_carnet[indice_carnet].vacuna);
							SendMessage(GetDlgItem(handler, AC_TipoVacuna), WM_GETTEXT, sizeof(lista_carnet[indice_carnet].z_vac) / sizeof(lista_carnet[indice_carnet].z_vac[0]), (LPARAM)lista_carnet[indice_carnet].z_vac);

							GetLocalTime(&lista_carnet[indice_carnet].fecha_aplicacion);
							SendDlgItemMessage(handler, AC_FechaDosisSiguiente, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (lista_carnet[indice_carnet].prox_dosis));
							lista_carnet[indice_carnet].fecha_nac = lista->nacimiento;
							strcat_s(lista_carnet[indice_carnet].calle, lista->calle);
							strcat_s(lista_carnet[indice_carnet].colonia, lista->colonia);
							strcat_s(lista_carnet[indice_carnet].estado, lista->estado);
							strcat_s(lista_carnet[indice_carnet].ciudad, lista->ciudad);
							strcat_s(lista_carnet[indice_carnet].Gpo_ocupacional, lista->Gpo_ocupacional);
							strcat_s(lista_carnet[indice_carnet].Perfil_riesgo, lista->Perfil_riesgo);
							strcat_s(lista_carnet[indice_carnet].e_civil, lista->e_civil);
							strcat_s(lista_carnet[indice_carnet].telefono, lista->telefono);
							strcat_s(lista_carnet[indice_carnet].RFC, lista->RFC);
							lista_carnet[indice_carnet].numero = lista->numero;



							strcpy_s(lista_carnet[indice_carnet].foto, aux_foto);
							
							dato_guardar = guardar_carnet;
							Guardar_usuarios();
							indice_carnet++;
							Guardar_indicee(); //posiblearreglo?
						//Cargar_carnet();
							EndDialog(handler, 0);
							//termnar el if aqui
						}

					}

				}
				else {
					MessageBox(handler, "Los campos de la vacuna no deben estar vacios", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				}
			}

			break;
		}

		case AC_BuscaVacuna: {
			char indice[100], precio[10];
			SendDlgItemMessage(handler, AC_Vacuna, WM_GETTEXT, sizeof(indice) / sizeof(indice[0]), (LPARAM)indice);

			if (*indice != NULL) {
				vacuna* buscar = Marca(indice);
				SendDlgItemMessage(handler, AC_Lote, WM_SETTEXT, sizeof(buscar->numero_lote) / sizeof(buscar->numero_lote[0]), (LPARAM)buscar->numero_lote);
				SendDlgItemMessage(handler, AC_TipoVacuna, WM_SETTEXT, sizeof(buscar->tipo) / sizeof(buscar->tipo[0]), (LPARAM)buscar->tipo);
				snprintf(precio, sizeof(precio), "%.2f", buscar->precio);
				SendDlgItemMessage(handler, AC_Precio, WM_SETTEXT, sizeof(precio) / sizeof(precio[0]), (LPARAM)precio);

			}

			break;
		}
		}
		break;
	}
	}
	return false;
}
//BAJAS
BOOL CALLBACK Eliminar_Vacunas(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {
	switch (mensaje) {

	case WM_INITDIALOG: {

		char texto_1[] = "Eliminar Vacuna ";
		char texto_com[256];
		strcpy_s(texto_com, texto_1);
		strcat_s(texto_com, " - Usuario activo: ");
		strcat_s(texto_com, usuario_activo);
		SetWindowText(handler, texto_com);

		break;
	}
	case WM_COMMAND: {
		int interacion = LOWORD(wParam);
		switch (interacion) {
		case WM_DESTROY: {
			EndDialog(handler, 0);
			break;
		}
		case BV_BuscarMarca: {
			char marca[25];
			SendDlgItemMessage(handler, BV_Marca, WM_GETTEXT, sizeof(marca) / sizeof(marca[0]), (LPARAM)marca);
			if (strcmp(marca, "") == 0) {
				MessageBox(handler, "Vacuna no seleccionada", "Aviso", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				vacuna* mostrar = Marca(marca);
				bool repetido = Marca(marca);
				if (repetido == true) {

					//mostrar->
					char precio[10];
					char c_dosis[10];
					char r_dosis[10];
					SendMessage(GetDlgItem(handler, BV_Clave), WM_SETTEXT, sizeof(mostrar->numero_lote) / sizeof(mostrar->numero_lote[0]), (LPARAM)mostrar->numero_lote);
					SendMessage(GetDlgItem(handler, BV_Descripcion), WM_SETTEXT, sizeof(mostrar->descripcion) / sizeof(mostrar->descripcion[0]), (LPARAM)mostrar->descripcion);
					SendMessage(GetDlgItem(handler, BV_Marca), WM_SETTEXT, sizeof(mostrar->marca) / sizeof(mostrar->marca[0]), (LPARAM)mostrar->marca);
					snprintf(precio, sizeof(precio), "%.2f", mostrar->precio);
					SendMessage(GetDlgItem(handler, BV_Precio), WM_SETTEXT, sizeof(precio) / sizeof(precio[0]), (LPARAM)precio);
					_itoa_s(mostrar->cantidad_dosis, c_dosis, sizeof(c_dosis));
					SendMessage(GetDlgItem(handler, BV_Dosis), WM_SETTEXT, sizeof(c_dosis) / sizeof(c_dosis[0]), (LPARAM)c_dosis);
					_itoa_s(mostrar->dosis_requeridas, r_dosis, sizeof(r_dosis));
					SendMessage(GetDlgItem(handler, BV_Dosis), WM_SETTEXT, sizeof(r_dosis) / sizeof(r_dosis[0]), (LPARAM)r_dosis);
					SendMessage(GetDlgItem(handler, BV_Tipo), WM_SETTEXT, sizeof(mostrar->tipo) / sizeof(mostrar->tipo[0]), (LPARAM)mostrar->tipo);

				}
				else {
					MessageBox(handler, "Vacuna no registrada", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				}
			}


			break;
		}
		case BV_Eliminar: {
			if (inicio_v != NULL) {
				char marca[25];
				SendDlgItemMessage(handler, BV_Marca, WM_GETTEXT, sizeof(marca) / sizeof(marca[0]), (LPARAM)marca);
				if (strcmp(marca, "") == 0) {
					MessageBox(handler, "Vacuna no seleccionada", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				}
				else {
					vacuna* mostrar = Marca(marca);
					eliminar(mostrar);
					dato_limpiar = guardar_vacunas;
					dato_guardar = guardar_vacunas;
					Guardar_usuarios();
					limpiar();
					Cargar_vacunas();
					EndDialog(handler, 0);
				}


			}
			else {
				MessageBox(handler, "No hay datos para eliminar", "Aviso", MB_OK | MB_ICONEXCLAMATION);
			}

			break;
		}
		}
		break;
	}
	}

	return false;
}
BOOL CALLBACK Eliminar_Carnet(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {
	int ID_busqueda = 0;
	char ID_bus[3];
	char aux_ed_edad[10];
	char formato_fecha[20];
	switch (mensaje) {
	case WM_INITDIALOG: {
		char texto_1[] = "Eliminar Carnet ";
		char texto_com[256];
		strcpy_s(texto_com, texto_1);
		strcat_s(texto_com, " - Usuario activo: ");
		strcat_s(texto_com, usuario_activo);
		SetWindowText(handler, texto_com);
		break;
	}

	case WM_COMMAND: {
		int interacion = LOWORD(wParam);
		switch (interacion) {
		case BC_Buscar: {
			SendMessage(GetDlgItem(handler, BC_Carnet), WM_GETTEXT, sizeof(ID_bus) / sizeof(ID_bus[0]), (LPARAM)ID_bus);
			if (*ID_bus != NULL) {
				ID_busqueda = atoi(ID_bus);
				busqueda_binaria(ID_busqueda);
				if (encontrar == true) {
					encontrar = false;
					MessageBox(handler, "Carnet encontrado", "Aviso", MB_OK);
					valor_busqueda--;
					SendMessage(GetDlgItem(handler, BC_Nombre), WM_SETTEXT, sizeof(lista_carnet[valor_busqueda].nombre_persona) / sizeof(lista_carnet[valor_busqueda].nombre_persona[0]), (LPARAM)lista_carnet[valor_busqueda].nombre_persona);
					SendMessage(GetDlgItem(handler, BC_ApellidoP), WM_SETTEXT, sizeof(lista_carnet[valor_busqueda].npaterno) / sizeof(lista_carnet[valor_busqueda].npaterno[0]), (LPARAM)lista_carnet[valor_busqueda].npaterno);
					SendMessage(GetDlgItem(handler, BC_ApellidoM), WM_SETTEXT, sizeof(lista_carnet[valor_busqueda].nmaterno) / sizeof(lista_carnet[valor_busqueda].nmaterno[0]), (LPARAM)lista_carnet[valor_busqueda].nmaterno);
					SendMessage(GetDlgItem(handler, BC_Sexo), WM_SETTEXT, sizeof(lista_carnet[valor_busqueda].sexo) / sizeof(lista_carnet[valor_busqueda].sexo[0]), (LPARAM)lista_carnet[valor_busqueda].sexo);
					_itoa_s(lista_carnet[valor_busqueda].edad, aux_ed_edad, sizeof(aux_ed_edad));
					SendMessage(GetDlgItem(handler, BC_Edad), WM_SETTEXT, sizeof(aux_ed_edad) / sizeof(aux_ed_edad[0]), (LPARAM)aux_ed_edad);
					GetDateFormat(NULL, 0, &(lista_carnet[valor_busqueda].prox_dosis), NULL, formato_fecha, sizeof(formato_fecha));
					SendMessage(GetDlgItem(handler, BC_FechaNacimiento), WM_SETTEXT, sizeof(formato_fecha) / sizeof(formato_fecha), (LPARAM)formato_fecha);

					SendMessage(GetDlgItem(handler, BC_Vacuna), WM_SETTEXT, sizeof(lista_carnet[valor_busqueda].vacuna) / sizeof(lista_carnet[valor_busqueda].vacuna[0]), (LPARAM)lista_carnet[valor_busqueda].vacuna);
					// SendMessage(GetDlgItem(handler, EDC_AUXV), WM_SETTEXT, sizeof(lista_carnet[valor_busqueda].vacuna) / sizeof(lista_carnet[valor_busqueda].vacuna[0]), (LPARAM)lista_carnet[valor_busqueda].vacuna);
					char aux_dosis[10];
					_itoa_s(lista_carnet[valor_busqueda].No_dosis, aux_dosis, sizeof(aux_dosis));
					SendMessage(GetDlgItem(handler, BC_Dosis), WM_SETTEXT, sizeof(aux_dosis) / sizeof(aux_dosis), (LPARAM)aux_dosis);
					char aux_f_a[20];
					char aux_f_a2[20];
					GetDateFormat(NULL, 0, &(lista_carnet[valor_busqueda].fecha_aplicacion), NULL, aux_f_a, sizeof(aux_f_a));
					GetDateFormat(NULL, 0, &(lista_carnet[valor_busqueda].prox_dosis), NULL, aux_f_a2, sizeof(aux_f_a2));

					SendMessage(GetDlgItem(handler, BC_ProxDosis), WM_SETTEXT, sizeof(aux_f_a) / sizeof(aux_f_a[0]), (LPARAM)aux_f_a);
					// SendMessage(GetDlgItem(handler, EDC_PDOSIS2), WM_SETTEXT, sizeof(aux_f_a2) / sizeof(aux_f_a2[0]), (LPARAM)aux_f_a2);
					SendMessage(GetDlgItem(handler, BC_Lugar), WM_SETTEXT, sizeof(lista_carnet[valor_busqueda].lugar) / sizeof(lista_carnet[valor_busqueda].lugar[0]), (LPARAM)lista_carnet[valor_busqueda].lugar);
					HBITMAP foto = (HBITMAP)LoadImage(NULL, lista_carnet[valor_busqueda].foto, IMAGE_BITMAP, 256, 256, LR_LOADFROMFILE);
					SendMessage(GetDlgItem(handler, BC_Foto), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);
				}
			}
			else {
				MessageBox(handler, "No ha ingresado numero de carnet", "Aviso", MB_OK | MB_ICONEXCLAMATION);
			}
			break;
		}
		case BC_Eliminar: {
			*ID_bus = NULL;
			char aux_validar[50];
			SendMessage(GetDlgItem(handler, BC_Carnet), WM_GETTEXT, sizeof(ID_bus) / sizeof(ID_bus[0]), (LPARAM)ID_bus);
			ID_busqueda = atoi(ID_bus);

			if (strcmp(ID_bus, "") == 0) {
				MessageBox(handler, "Campo ID no debe estar vacio", "Aviso", MB_OK | MB_ICONEXCLAMATION);
			}
			else {


				SendMessage(GetDlgItem(handler, BC_Nombre), WM_GETTEXT, sizeof(aux_validar) / sizeof(aux_validar[0]), (LPARAM)aux_validar);
				if (strcmp(aux_validar, "") == 0) {
					MessageBox(handler, "No hay un carnet ingresado a eliminar", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				}
				else {

					for (int x = indice_carnet; x >= 0; x--) {
						lista_carnet[x + 1] = lista_carnet[x];
					}
					lista_carnet[ID_busqueda - 1] = lista_carnet[ID_busqueda];
					indice_carnet--;
					dato_guardar = guardar_carnet;
					Guardar_usuarios();
					MessageBox(handler, "Carnet Eliminado", "Aviso", MB_OK | MB_ICONEXCLAMATION);
					EndDialog(handler, 0);
				}
			}
			break;
		}
		case WM_DESTROY: {
			EndDialog(handler, 0);
			break;
		}

		}
		break;
	}
	}
	return false;
}
//CAMBIOS
BOOL CALLBACK Editar_Personas(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {
	persona* visual = nullptr;
	bool existe = false;
	char data[50];//recoger rfc o curp dependiendo la opcion 
	char edad[5];
	char numero[10];
	char telefono[12];
	char Codigo[10];
	char Fecha[30];
	switch (mensaje) {

	case WM_INITDIALOG: {
		SendMessage(GetDlgItem(handler, CP_EstadoCivil), CB_ADDSTRING, NULL, (LPARAM)"Soltero/a");
		SendMessage(GetDlgItem(handler, CP_EstadoCivil), CB_ADDSTRING, NULL, (LPARAM)"Casado/a");
		SendMessage(GetDlgItem(handler, CP_EstadoCivil), CB_ADDSTRING, NULL, (LPARAM)"Union Libre");
		SendMessage(GetDlgItem(handler, CP_EstadoCivil), CB_ADDSTRING, NULL, (LPARAM)"Separado/a");
		SendMessage(GetDlgItem(handler, CP_EstadoCivil), CB_ADDSTRING, NULL, (LPARAM)"Divorciado/a");
		SendMessage(GetDlgItem(handler, CP_EstadoCivil), CB_ADDSTRING, NULL, (LPARAM)"Viudo/a");

		SendMessage(GetDlgItem(handler, CP_Sexo), CB_ADDSTRING, NULL, (LPARAM)"Masculino");
		SendMessage(GetDlgItem(handler, CP_Sexo), CB_ADDSTRING, NULL, (LPARAM)"Femenino");
		SendMessage(GetDlgItem(handler, CP_Sexo), CB_ADDSTRING, NULL, (LPARAM)"Otro");


		char texto_1[] = "Editar Personas ";
		char texto_com[256];
		strcpy_s(texto_com, texto_1);
		strcat_s(texto_com, " - Usuario activo: ");
		strcat_s(texto_com, usuario_activo);
		SetWindowText(handler, texto_com);


		break;
	}
	case WM_COMMAND: {


		int interacion = LOWORD(wParam);
		switch (interacion) {
		case E_PorRFC: {
			seleccion_p = por_rfc;
			break;
		}
		case E_PorCURP: {
			seleccion_p = por_curp;
			break;
		}
		case CP_Buscar: {
			switch (seleccion_p) {
			case 1:
				SendDlgItemMessage(handler, CP_Dato, WM_GETTEXT, sizeof(data) / sizeof(data[0]), (LPARAM)data);
				SendDlgItemMessage(handler, CP_Static, WM_SETTEXT, NULL, (LPARAM)"RFC");
				visual = RFC_CURP(data);
				existe = RFC_CURP(data);
				break;
			case 2:
				SendDlgItemMessage(handler, CP_Dato, WM_GETTEXT, sizeof(data) / sizeof(data[0]), (LPARAM)data);
				SendDlgItemMessage(handler, CP_Static, WM_SETTEXT, NULL, (LPARAM)"CURP");
				visual = CURP_RFC(data);
				existe = CURP_RFC(data);

			}
			if (strcmp(data, "") == 0) {
				MessageBox(handler, "Debe haber RFC o CURP para buscar persona", "Aviso", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				if (existe == true) {

					_itoa_s(visual->edad, edad, sizeof(edad));
					_itoa_s(visual->numero, numero, sizeof(numero));
					_itoa_s(visual->codigo_postal, Codigo, sizeof(Codigo));
					GetDateFormat(NULL, 0, &(visual->nacimiento), NULL, Fecha, sizeof(Fecha));

					SendMessage(GetDlgItem(handler, CP_Nombre), WM_SETTEXT, sizeof(visual->nombre_persona) / sizeof(visual->nombre_persona[0]), (LPARAM)visual->nombre_persona);
					SendMessage(GetDlgItem(handler, CP_ApellidoP), WM_SETTEXT, sizeof(visual->paterno) / sizeof(visual->paterno[0]), (LPARAM)visual->paterno);
					SendMessage(GetDlgItem(handler, CP_ApellidoM), WM_SETTEXT, sizeof(visual->materno) / sizeof(visual->materno[0]), (LPARAM)visual->materno);
					//	SendMessage(GetDlgItem(handler, EDITP_RFC), WM_SETTEXT, sizeof(visual->RFC) / sizeof(visual->RFC[0]), (LPARAM)visual->RFC);
					//	SendMessage(GetDlgItem(handler, EDITP_CURP), WM_SETTEXT, sizeof(visual->CURP) / sizeof(visual->CURP[0]), (LPARAM)visual->CURP);
					SendMessage(GetDlgItem(handler, CP_Edad), WM_SETTEXT, sizeof(edad) / sizeof(edad[0]), (LPARAM)edad);//int
					SendMessage(GetDlgItem(handler, CP_Telefono), WM_SETTEXT, sizeof(visual->telefono) / sizeof(visual->telefono[0]), (LPARAM)visual->telefono);//int
					SendMessage(GetDlgItem(handler, CP_GrupoOc), WM_SETTEXT, sizeof(visual->Gpo_ocupacional) / sizeof(visual->Gpo_ocupacional[0]), (LPARAM)visual->Gpo_ocupacional);//int
					SendMessage(GetDlgItem(handler, CP_PerfilRiesgo), WM_SETTEXT, sizeof(visual->Perfil_riesgo) / sizeof(visual->Perfil_riesgo[0]), (LPARAM)visual->Perfil_riesgo);//int

					SendMessage(GetDlgItem(handler, CP_Calle), WM_SETTEXT, sizeof(visual->calle) / sizeof(visual->calle[0]), (LPARAM)visual->calle);
					SendMessage(GetDlgItem(handler, CP_Numero), WM_SETTEXT, sizeof(numero) / sizeof(numero[0]), (LPARAM)numero);//int
					SendMessage(GetDlgItem(handler, CP_Colonia), WM_SETTEXT, sizeof(visual->colonia) / sizeof(visual->colonia[0]), (LPARAM)visual->colonia);
					SendMessage(GetDlgItem(handler, CP_Ciudad), WM_SETTEXT, sizeof(visual->ciudad) / sizeof(visual->ciudad[0]), (LPARAM)visual->ciudad);
					SendMessage(GetDlgItem(handler, CP_Estado), WM_SETTEXT, sizeof(visual->estado) / sizeof(visual->estado[0]), (LPARAM)visual->estado);
					SendMessage(GetDlgItem(handler, CP_CodigoPostal), WM_SETTEXT, sizeof(Codigo) / sizeof(Codigo[0]), (LPARAM)Codigo);//int
					SendMessage(GetDlgItem(handler, CP_EstadoCivilAnterior), WM_SETTEXT, sizeof(visual->e_civil) / sizeof(visual->e_civil[0]), (LPARAM)visual->e_civil);//i
					SendMessage(GetDlgItem(handler, CP_SexoAnterior), WM_SETTEXT, sizeof(visual->sexo) / sizeof(visual->sexo[0]), (LPARAM)visual->sexo);

					SendMessage(GetDlgItem(handler, CP_FechaNacimientoAnterior), WM_SETTEXT, sizeof(Fecha) / sizeof(Fecha[0]), (LPARAM)Fecha);//int

					HBITMAP foto = (HBITMAP)LoadImage(NULL, visual->foto_c, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
					SendMessage(GetDlgItem(handler, CP_Foto), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);

				}
				else {
					MessageBox(handler, "No se encontro persona", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				}
			}
			break;
		}
		case WM_DESTROY: {
			EndDialog(handler, 0);
			break;
		}
		case CP_Editar: {
			switch (seleccion_p) {
			case 1:
				SendDlgItemMessage(handler, CP_Dato, WM_GETTEXT, sizeof(data) / sizeof(data[0]), (LPARAM)data);
				visual = RFC_CURP(data);
				existe = RFC_CURP(data);
				break;
			case 2:
				SendDlgItemMessage(handler, CP_Dato, WM_GETTEXT, sizeof(data) / sizeof(data[0]), (LPARAM)data);
				visual = CURP_RFC(data);
				existe = CURP_RFC(data);

			}

			if (strcmp(data, "") == 0) {
				MessageBox(handler, "Debe haber RFC o CURP para buscar persona", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				dato_limpiar = guardar_ciudadanos;
				limpiar();
				Cargar_ciudadanos();
			}
			else {
				char aux_sex[50], aux_ecvivl[50]; SYSTEMTIME aux_fecha;
				SendDlgItemMessage(handler, CP_Sexo, WM_GETTEXT, sizeof(aux_sex) / sizeof(aux_sex[0]), (LPARAM)aux_sex);
				SendDlgItemMessage(handler, CP_EstadoCivil, WM_GETTEXT, sizeof(aux_ecvivl) / sizeof(aux_ecvivl[0]), (LPARAM)aux_ecvivl);
				SendDlgItemMessage(handler, CP_FechaNacimiento, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (aux_fecha));

				if (strcmp(aux_sex, "") == 0 || strcmp(aux_ecvivl, "") == 0) {
					MessageBox(handler, "faltan datos por llenar", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				}
				else {
					SendDlgItemMessage(handler, CP_Dato, WM_GETTEXT, sizeof(data) / sizeof(data[0]), (LPARAM)data);
					actualizar(handler, visual);
					dato_guardar = guardar_ciudadanos;
					dato_limpiar = guardar_ciudadanos;
					Guardar_usuarios();
					limpiar();
					Cargar_ciudadanos();
					EndDialog(handler, 0);
				}


			}

			break;
		}
		case CP_Examina: {
			OPENFILENAME abrir;
			ZeroMemory(&abrir, sizeof(abrir));
			abrir.lStructSize = sizeof(abrir);
			abrir.hwndOwner = handler;
			abrir.lpstrFile = foto_persona;
			abrir.lpstrFilter = "Mapa de Bits\0*.bmp*\0Imagen PNG\0*.png*\0Imagen JPG\0*.jpg*";
			abrir.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
			abrir.lpstrFile[0] = NULL;
			abrir.nMaxFile = sizeof(abrir);
			abrir.nFilterIndex = 0;
			if (GetOpenFileName(&abrir) == TRUE) {

				HBITMAP foto = (HBITMAP)LoadImage(NULL, foto_persona, IMAGE_BITMAP, 256, 256, LR_LOADFROMFILE);
				SendMessage(GetDlgItem(handler, AP_Foto), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);

			}
			break;

		}
		}
		break;
	}
	}
	return false;
}
BOOL CALLBACK Editar_Vacunas(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {
	switch (mensaje) {

	case WM_INITDIALOG: {
		SendMessage(GetDlgItem(handler, CV_Tipo), CB_ADDSTRING, NULL, (LPARAM)"ARN mensajero");
		SendMessage(GetDlgItem(handler, CV_Tipo), CB_ADDSTRING, NULL, (LPARAM)"Subunidades proteicas");
		SendMessage(GetDlgItem(handler, CV_Tipo), CB_ADDSTRING, NULL, (LPARAM)"Vector viral");


		char texto_1[] = "Editar Vacunas ";
		char texto_com[256];
		strcpy_s(texto_com, texto_1);
		strcat_s(texto_com, " - Usuario activo: ");
		strcat_s(texto_com, usuario_activo);
		SetWindowText(handler, texto_com);
		break;


		break;
	}

	case WM_COMMAND: {
		int interacion = LOWORD(wParam);
		switch (interacion) {
		case WM_DESTROY: {
			EndDialog(handler, 0);
			break;
		}
		case CV_Buscar: {
			char marca[25];
			SendDlgItemMessage(handler, CV_Marca, WM_GETTEXT, sizeof(marca) / sizeof(marca[0]), (LPARAM)marca);
			if (strcmp(marca, "") == 0) {
				MessageBox(handler, "No hay vacuna seleccionada", "Aviso", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				vacuna* mostrar = Marca(marca);
				bool repetido = Marca(marca);
				if (repetido == true) {

					//mostrar->
					char precio[10];
					char c_dosis[10];
					char r_dosis[10];
					SendMessage(GetDlgItem(handler, CV_Tipo), WM_SETTEXT, sizeof(mostrar->numero_lote) / sizeof(mostrar->numero_lote[0]), (LPARAM)mostrar->numero_lote);
					SendMessage(GetDlgItem(handler, CV_Descripcion), WM_SETTEXT, sizeof(mostrar->descripcion) / sizeof(mostrar->descripcion[0]), (LPARAM)mostrar->descripcion);
					SendMessage(GetDlgItem(handler, CV_Marca), WM_SETTEXT, sizeof(mostrar->marca) / sizeof(mostrar->marca[0]), (LPARAM)mostrar->marca);
					snprintf(precio, sizeof(precio), "%.2f", mostrar->precio);
					SendMessage(GetDlgItem(handler, CV_Precio), WM_SETTEXT, sizeof(precio) / sizeof(precio[0]), (LPARAM)precio);
					_itoa_s(mostrar->cantidad_dosis, c_dosis, sizeof(c_dosis));
					SendMessage(GetDlgItem(handler, CV_VacunasImp), WM_SETTEXT, sizeof(c_dosis) / sizeof(c_dosis[0]), (LPARAM)c_dosis);
					_itoa_s(mostrar->dosis_requeridas, r_dosis, sizeof(r_dosis));
					SendMessage(GetDlgItem(handler, CV_DosisReq), WM_SETTEXT, sizeof(r_dosis) / sizeof(r_dosis[0]), (LPARAM)r_dosis);
					SendMessage(GetDlgItem(handler, CV_Tipo), CB_SETCURSEL, sizeof(mostrar->tipo) / sizeof(mostrar->tipo[0]), (LPARAM)mostrar->tipo);
					SendMessage(GetDlgItem(handler, CV_TipoAnterior), WM_SETTEXT, sizeof(mostrar->tipo) / sizeof(mostrar->tipo[0]), (LPARAM)mostrar->tipo);

				}
				else {
					MessageBox(handler, "Vacuna no registrada", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				}
			}


			break;
		}
		case CV_Editar: {
			char marca[25];
			vacuna* mostrar = Marca(marca);
			SendDlgItemMessage(handler, CV_Marca, WM_GETTEXT, sizeof(marca) / sizeof(marca[0]), (LPARAM)marca);

			if (strcmp(marca, "") == 0) {
				MessageBox(handler, "No hay vacuna seleccionada", "Aviso", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				vacuna* mostrar = Marca(marca);
				actualizar(handler, mostrar);
				EndDialog(handler, 0);
			}

			break;
		}
		}
		break;
	}
	}

	return false;

}
BOOL CALLBACK Editar_Carnet(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {
	int ID_busqueda = 0;
	char ID_bus[3];
	char aux_ed_edad[10];
	char formato_fecha[20];

	switch (mensaje) {
	case WM_INITDIALOG: {
		if (inicio_v != NULL) {
			vacuna* lista = inicio_v;

			if (lista->siguiente != inicio_v) {
				while (lista->siguiente != inicio_v) {
					SendMessage(GetDlgItem(handler, CC_VacunaAAplicar), CB_ADDSTRING, NULL, (LPARAM)lista->marca);
					lista = lista->siguiente;
				}
			}
			else {
				SendMessage(GetDlgItem(handler, CC_VacunaAAplicar), CB_ADDSTRING, NULL, (LPARAM)lista->marca);
			}
		}

		char texto_1[] = "Editar Carnet ";
		char texto_com[256];
		strcpy_s(texto_com, texto_1);
		strcat_s(texto_com, " - Usuario activo: ");
		strcat_s(texto_com, usuario_activo);
		SetWindowText(handler, texto_com);

		break;

	}


	case WM_COMMAND: {
		int interacion = LOWORD(wParam);
		switch (interacion) {
		case CC_Buscar: {
			SendMessage(GetDlgItem(handler, CC_Carnet), WM_GETTEXT, sizeof(ID_bus) / sizeof(ID_bus[0]), (LPARAM)ID_bus);
			if (*ID_bus != NULL) {
				ID_busqueda = atoi(ID_bus);
				busqueda_binaria(ID_busqueda);
				if (encontrar == true) {
					encontrar = false;
					MessageBox(handler, "Carnet encontrado", "Aviso", MB_OK);
					valor_busqueda = valor_busqueda - 1 ;
					SendMessage(GetDlgItem(handler, CC_Nombre), WM_SETTEXT, sizeof(lista_carnet[valor_busqueda].nombre_persona) / sizeof(lista_carnet[valor_busqueda].nombre_persona[0]), (LPARAM)lista_carnet[valor_busqueda].nombre_persona);
					SendMessage(GetDlgItem(handler, CC_ApellidoP), WM_SETTEXT, sizeof(lista_carnet[valor_busqueda].npaterno) / sizeof(lista_carnet[valor_busqueda].npaterno[0]), (LPARAM)lista_carnet[valor_busqueda].npaterno);
					SendMessage(GetDlgItem(handler, CC_ApellidoM), WM_SETTEXT, sizeof(lista_carnet[valor_busqueda].nmaterno) / sizeof(lista_carnet[valor_busqueda].nmaterno[0]), (LPARAM)lista_carnet[valor_busqueda].nmaterno);
					SendMessage(GetDlgItem(handler, CC_Sexo), WM_SETTEXT, sizeof(lista_carnet[valor_busqueda].sexo) / sizeof(lista_carnet[valor_busqueda].sexo[0]), (LPARAM)lista_carnet[valor_busqueda].sexo);
					_itoa_s(lista_carnet[valor_busqueda].edad, aux_ed_edad, sizeof(aux_ed_edad));
					SendMessage(GetDlgItem(handler, CC_Edad), WM_SETTEXT, sizeof(aux_ed_edad) / sizeof(aux_ed_edad[0]), (LPARAM)aux_ed_edad);
					GetDateFormat(NULL, 0, &(lista_carnet[valor_busqueda].fecha_aplicacion), NULL, formato_fecha, sizeof(formato_fecha));
					SendMessage(GetDlgItem(handler, CC_FechaNacimiento), WM_SETTEXT, sizeof(formato_fecha) / sizeof(formato_fecha), (LPARAM)formato_fecha);

					SendMessage(GetDlgItem(handler, CC_VacunaAAplicarAnterior), WM_SETTEXT, sizeof(lista_carnet[valor_busqueda].vacuna) / sizeof(lista_carnet[valor_busqueda].vacuna[0]), (LPARAM)lista_carnet[valor_busqueda].vacuna);
					char aux_dosis[10];
					_itoa_s(lista_carnet[valor_busqueda].No_dosis, aux_dosis, sizeof(aux_dosis));
					SendMessage(GetDlgItem(handler, CC_Dosis), WM_SETTEXT, sizeof(aux_dosis) / sizeof(aux_dosis), (LPARAM)aux_dosis);
					char aux_f_a[20];
					char aux_f_a2[20];
					GetDateFormat(NULL, 0, &(lista_carnet[valor_busqueda].fecha_aplicacion), NULL, aux_f_a, sizeof(aux_f_a));
					GetDateFormat(NULL, 0, &(lista_carnet[valor_busqueda].prox_dosis), NULL, aux_f_a2, sizeof(aux_f_a2));

					SendMessage(GetDlgItem(handler, CC_FechaAplicacionAnterior), WM_SETTEXT, sizeof(aux_f_a) / sizeof(aux_f_a[0]), (LPARAM)aux_f_a);
					SendMessage(GetDlgItem(handler, CC_ProximaDosisAnterior), WM_SETTEXT, sizeof(aux_f_a2) / sizeof(aux_f_a2[0]), (LPARAM)aux_f_a2);
					SendMessage(GetDlgItem(handler, CC_Lugar), WM_SETTEXT, sizeof(lista_carnet[valor_busqueda].lugar) / sizeof(lista_carnet[valor_busqueda].lugar[0]), (LPARAM)lista_carnet[valor_busqueda].lugar);
					HBITMAP foto = (HBITMAP)LoadImage(NULL, lista_carnet[valor_busqueda].foto, IMAGE_BITMAP, 256, 256, LR_LOADFROMFILE);
					SendMessage(GetDlgItem(handler, CC_Foto), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);
				}
			}
			else {
				MessageBox(handler, "No ha ingresado numero de carnet", "Aviso", MB_OK | MB_ICONEXCLAMATION);
			}
			break;
		}
		case CC_Editar: {
			char aux_lugar[255], aux_vacuna[50], aux_nombre[50];
			SYSTEMTIME fecha_actual, ed_prox;

			SendMessage(GetDlgItem(handler, CC_VacunaAAplicar), WM_GETTEXT, sizeof(aux_vacuna) / sizeof(aux_vacuna[0]), (LPARAM)aux_vacuna);
			SendMessage(GetDlgItem(handler, CC_Lugar), WM_GETTEXT, sizeof(aux_lugar) / sizeof(aux_lugar[0]), (LPARAM)aux_lugar);
			SendDlgItemMessage(handler, CC_ProximaDosis, DTM_GETSYSTEMTIME, NULL, (LPARAM) & (ed_prox));
			SendMessage(GetDlgItem(handler, CC_Nombre), WM_GETTEXT, sizeof(aux_nombre) / sizeof(aux_nombre[0]), (LPARAM)aux_nombre);
			GetLocalTime(&fecha_actual);
			long long ed_tiempo;
			ed_tiempo = tiempo(ed_prox) - tiempo(fecha_actual);
			if (strcmp(aux_nombre, "") == 0) {
				MessageBox(handler, "No hay carnet ingresado para editar", "Aviso", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				if (ed_tiempo <= 0) {
					MessageBox(handler, "Fecha de proxima dosis no puede ser menor o igual. a la actual", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				}
				else {
					SendMessage(GetDlgItem(handler, CC_Carnet), WM_GETTEXT, sizeof(ID_bus) / sizeof(ID_bus[0]), (LPARAM)ID_bus);
					if (*ID_bus != NULL || *aux_lugar != NULL || *aux_vacuna != NULL) {

						ID_busqueda = atoi(ID_bus);
						lista_carnet[ID_busqueda].prox_dosis = ed_prox;
						strcpy_s(lista_carnet[ID_busqueda].vacuna, aux_vacuna);
						strcpy_s(lista_carnet[ID_busqueda].lugar, aux_lugar);
						dato_guardar = guardar_carnet;
						Guardar_usuarios();
						MessageBox(handler, "Informacion actualizada", "Aviso", MB_OK | MB_ICONINFORMATION);
						EndDialog(handler, 0);
					}
					else {
						MessageBox(handler, "ID Carnet\nLugar\nVacuna no deben estar vacios", "Aviso", MB_OK | MB_ICONEXCLAMATION);
					}

				}
			}


			break;
		}
		case WM_DESTROY: {
			EndDialog(handler, 0);
			break;
		}
		}
		break;
	}
	}
	return false;


}
//REPORTES
BOOL CALLBACK Reportes(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {

	switch (mensaje) {
	case WM_INITDIALOG: {
		char texto_1[] = "Reportes ";
		char texto_com[256];
		strcpy_s(texto_com, texto_1);
		strcat_s(texto_com, " - Usuario activo: ");
		strcat_s(texto_com, usuario_activo);
		SetWindowText(handler, texto_com);
		break;
	}

	case WM_COMMAND: {
		int interacion = LOWORD(wParam);
		switch (interacion) {
		case RP_NombreQuick: {
			seleccion_reporte = busqueda_nombre; //QuickSort
			break;
		}
		case RP_CarnetHeap: {
			seleccion_reporte = busqueda_carnet; //HeapSort
			break;
		}
		case RP_Reportes: {
			HWND hwnd_lista = GetDlgItem(handler, RP_Lista);
			SendMessage(hwnd_lista, LB_RESETCONTENT, NULL, NULL);
			for (int a = 0; a < indice_carnet; a++) {
				lista_heapquick[a] = {};
			}
			//MessageBox(NULL, "Reporte generado", "Aviso", MB_OK);
			switch (seleccion_reporte) {
			case 2:
				for (int a = 0; a < indice_carnet; a++) {
					lista_heapquick[a] = lista_carnet[a];
				}
				heapsort();
				//	MessageBox(NULL, "Reporte generado\nse visualiza por carnet", "Aviso", MB_OK);

				break;

			case 1:
				for (int a = 0; a < indice_carnet; a++) {
					lista_heapquick[a] = lista_carnet[a];
				}
				quicksort(0, indice_carnet);
				//MessageBox(NULL, "Reporte generado\nse visualiza por nombre", "Aviso", MB_OK);
				break;
			}

			/**///SendMessage(GetDlgItem(listbox, M_PRODUCTOS), LB_ADDSTRING, NULL, (LPARAM)aux->nombrep);
			switch (seleccion_reporte) {
			case 2:
				for (int a = 0; a < indice_carnet; a++) {
					char no_carnet[5];
					_itoa_s(lista_heapquick[a].No_carnet, no_carnet, sizeof(no_carnet));
					SendMessage(GetDlgItem(handler, RP_Lista), LB_ADDSTRING, NULL, (LPARAM)no_carnet);
				}

				break;

			case 1:
				for (int a = 0; a < indice_carnet; a++) {

					SendMessage(GetDlgItem(handler, RP_Lista), LB_ADDSTRING, NULL, (LPARAM)lista_heapquick[a].nombre_persona);
				}
				break;
			}


			SendMessage(GetDlgItem(handler, RP_Nombre), WM_SETTEXT, sizeof(lista_heapquick[0].nombre_persona) / sizeof(lista_heapquick[0].nombre_persona[0]), (LPARAM)lista_heapquick[0].nombre_persona);
			SendMessage(GetDlgItem(handler, RP_ApellidoP), WM_SETTEXT, sizeof(lista_heapquick[0].npaterno) / sizeof(lista_heapquick[0].npaterno[0]), (LPARAM)lista_heapquick[0].npaterno);
			SendMessage(GetDlgItem(handler, RP_ApellidoM), WM_SETTEXT, sizeof(lista_heapquick[0].nmaterno) / sizeof(lista_heapquick[0].nmaterno[0]), (LPARAM)lista_heapquick[0].nmaterno);
			SendMessage(GetDlgItem(handler, RP_Sexo), WM_SETTEXT, sizeof(lista_heapquick[0].sexo) / sizeof(lista_heapquick[0].sexo[0]), (LPARAM)lista_heapquick[0].sexo);
			SendMessage(GetDlgItem(handler, RP_CURP), WM_SETTEXT, sizeof(lista_heapquick[0].CURP) / sizeof(lista_heapquick[0].CURP[0]), (LPARAM)lista_heapquick[0].CURP);
			SendMessage(GetDlgItem(handler, RP_RFC), WM_SETTEXT, sizeof(lista_heapquick[0].RFC) / sizeof(lista_heapquick[0].RFC[0]), (LPARAM)lista_heapquick[0].RFC);

			char rep_edad[10];

			_itoa_s(lista_heapquick->numero, rep_edad, sizeof(rep_edad));
			SendMessage(GetDlgItem(handler, RP_Edad), WM_SETTEXT, sizeof(rep_edad) / sizeof(rep_edad[0]), (LPARAM)rep_edad);
			char rep_fecha[50];
			GetDateFormat(NULL, 0, &(lista_heapquick[0].fecha_nac), NULL, rep_fecha, sizeof(rep_fecha));
			SendMessage(GetDlgItem(handler, RP_Nacimiento), WM_SETTEXT, sizeof(rep_fecha) / sizeof(rep_fecha[0]), (LPARAM)rep_fecha);
			SendMessage(GetDlgItem(handler, RP_EstadoCivil), WM_SETTEXT, sizeof(lista_heapquick[0].e_civil) / sizeof(lista_heapquick[0].e_civil[0]), (LPARAM)lista_heapquick[0].e_civil);
			SendMessage(GetDlgItem(handler, RP_GrupoOcupacional), WM_SETTEXT, sizeof(lista_heapquick[0].Gpo_ocupacional) / sizeof(lista_heapquick[0].Gpo_ocupacional[0]), (LPARAM)lista_heapquick[0].Gpo_ocupacional);
			SendMessage(GetDlgItem(handler, RP_PerfilRiesgo), WM_SETTEXT, sizeof(lista_heapquick[0].Perfil_riesgo) / sizeof(lista_heapquick[0].Perfil_riesgo[0]), (LPARAM)lista_heapquick[0].Perfil_riesgo);
			SendMessage(GetDlgItem(handler, RP_Telefono), WM_SETTEXT, sizeof(lista_heapquick[0].telefono) / sizeof(lista_heapquick[0].telefono[0]), (LPARAM)lista_heapquick[0].telefono);

			SendMessage(GetDlgItem(handler, RP_Calle), WM_SETTEXT, sizeof(lista_heapquick[0].calle) / sizeof(lista_heapquick[0].calle[0]), (LPARAM)lista_heapquick[0].calle);
			SendMessage(GetDlgItem(handler, RP_Colonia), WM_SETTEXT, sizeof(lista_heapquick[0].colonia) / sizeof(lista_heapquick[0].colonia[0]), (LPARAM)lista_heapquick[0].colonia);
			char rep_num[10];
			_itoa_s(lista_heapquick[0].numero, rep_num, sizeof(rep_num));
			SendMessage(GetDlgItem(handler, RP_Numero), WM_SETTEXT, sizeof(rep_num) / sizeof(rep_num[0]), (LPARAM)rep_num);
			SendMessage(GetDlgItem(handler, RP_Ciudad), WM_SETTEXT, sizeof(lista_heapquick[0].ciudad) / sizeof(lista_heapquick[0].ciudad[0]), (LPARAM)lista_heapquick[0].ciudad);
			SendMessage(GetDlgItem(handler, RP_Estado), WM_SETTEXT, sizeof(lista_heapquick[0].estado) / sizeof(lista_heapquick[0].estado[0]), (LPARAM)lista_heapquick[0].estado);
			SendMessage(GetDlgItem(handler, RP_Marca), WM_SETTEXT, sizeof(lista_heapquick[0].vacuna) / sizeof(lista_heapquick[0].vacuna[0]), (LPARAM)lista_heapquick[0].vacuna);
			char rep_ndosis[10];
			//lista_heapquick[0]
			_itoa_s(lista_heapquick[0].No_dosis, rep_ndosis, sizeof(rep_ndosis));
			SendMessage(GetDlgItem(handler, RP_DosisActual), WM_SETTEXT, sizeof(rep_ndosis) / sizeof(rep_ndosis[0]), (LPARAM)rep_ndosis);
			char rep_prox[50], rep_uda[50];
			GetDateFormat(NULL, 0, &(lista_heapquick[0].fecha_aplicacion), NULL, rep_uda, sizeof(rep_uda));
			GetDateFormat(NULL, 0, &(lista_heapquick[0].prox_dosis), NULL, rep_prox, sizeof(rep_prox));
			SendMessage(GetDlgItem(handler, RP_ProximaDosis), WM_SETTEXT, sizeof(rep_prox) / sizeof(rep_prox[0]), (LPARAM)rep_prox);
			SendMessage(GetDlgItem(handler, RP_UltimaDosis), WM_SETTEXT, sizeof(rep_uda) / sizeof(rep_uda[0]), (LPARAM)rep_uda);

			HBITMAP foto = (HBITMAP)LoadImage(NULL, lista_heapquick[0].foto, IMAGE_BITMAP, 128, 128, LR_LOADFROMFILE);
			SendMessage(GetDlgItem(handler, RP_Foto), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);
			Guardar_reportes();


			break;

		}
		case RP_Lista: {
			if (HIWORD(wParam) == LBN_DBLCLK) {
				int seleccion = SendMessage(GetDlgItem(handler, RP_Lista), LB_GETCURSEL, NULL, NULL);
				SendMessage(GetDlgItem(handler, RP_Nombre), WM_SETTEXT, sizeof(lista_heapquick[seleccion].nombre_persona) / sizeof(lista_heapquick[seleccion].nombre_persona[0]), (LPARAM)lista_heapquick[seleccion].nombre_persona);
				SendMessage(GetDlgItem(handler, RP_ApellidoP), WM_SETTEXT, sizeof(lista_heapquick[seleccion].npaterno) / sizeof(lista_heapquick[seleccion].npaterno[0]), (LPARAM)lista_heapquick[seleccion].npaterno);
				SendMessage(GetDlgItem(handler, RP_ApellidoM), WM_SETTEXT, sizeof(lista_heapquick[seleccion].nmaterno) / sizeof(lista_heapquick[seleccion].nmaterno[0]), (LPARAM)lista_heapquick[seleccion].nmaterno);
				SendMessage(GetDlgItem(handler, RP_Sexo), WM_SETTEXT, sizeof(lista_heapquick[seleccion].sexo) / sizeof(lista_heapquick[seleccion].sexo[0]), (LPARAM)lista_heapquick[seleccion].sexo);
				SendMessage(GetDlgItem(handler, RP_CURP), WM_SETTEXT, sizeof(lista_heapquick[seleccion].CURP) / sizeof(lista_heapquick[seleccion].CURP[0]), (LPARAM)lista_heapquick[seleccion].CURP);
				SendMessage(GetDlgItem(handler, RP_RFC), WM_SETTEXT, sizeof(lista_heapquick[seleccion].RFC) / sizeof(lista_heapquick[seleccion].RFC[0]), (LPARAM)lista_heapquick[seleccion].RFC);

				char rep_edad[10];

				_itoa_s(lista_heapquick->numero, rep_edad, sizeof(rep_edad));
				SendMessage(GetDlgItem(handler, RP_Edad), WM_SETTEXT, sizeof(rep_edad) / sizeof(rep_edad[0]), (LPARAM)rep_edad);
				char rep_fecha[50];
				GetDateFormat(NULL, 0, &(lista_heapquick[seleccion].fecha_nac), NULL, rep_fecha, sizeof(rep_fecha));
				SendMessage(GetDlgItem(handler, RP_Nacimiento), WM_SETTEXT, sizeof(rep_fecha) / sizeof(rep_fecha[0]), (LPARAM)rep_fecha);
				SendMessage(GetDlgItem(handler, RP_EstadoCivil), WM_SETTEXT, sizeof(lista_heapquick[seleccion].e_civil) / sizeof(lista_heapquick[seleccion].e_civil[0]), (LPARAM)lista_heapquick[seleccion].e_civil);
				SendMessage(GetDlgItem(handler, RP_GrupoOcupacional), WM_SETTEXT, sizeof(lista_heapquick[seleccion].Gpo_ocupacional) / sizeof(lista_heapquick[seleccion].Gpo_ocupacional[0]), (LPARAM)lista_heapquick[seleccion].Gpo_ocupacional);
				SendMessage(GetDlgItem(handler, RP_PerfilRiesgo), WM_SETTEXT, sizeof(lista_heapquick[seleccion].Perfil_riesgo) / sizeof(lista_heapquick[seleccion].Perfil_riesgo[0]), (LPARAM)lista_heapquick[seleccion].Perfil_riesgo);
				SendMessage(GetDlgItem(handler, RP_Telefono), WM_SETTEXT, sizeof(lista_heapquick[seleccion].telefono) / sizeof(lista_heapquick[seleccion].telefono[0]), (LPARAM)lista_heapquick[seleccion].telefono);

				SendMessage(GetDlgItem(handler, RP_Calle), WM_SETTEXT, sizeof(lista_heapquick[seleccion].calle) / sizeof(lista_heapquick[seleccion].calle[0]), (LPARAM)lista_heapquick[seleccion].calle);
				SendMessage(GetDlgItem(handler, RP_Colonia), WM_SETTEXT, sizeof(lista_heapquick[seleccion].colonia) / sizeof(lista_heapquick[seleccion].colonia[0]), (LPARAM)lista_heapquick[seleccion].colonia);
				char rep_num[10];
				_itoa_s(lista_heapquick[seleccion].numero, rep_num, sizeof(rep_num));
				SendMessage(GetDlgItem(handler, RP_Numero), WM_SETTEXT, sizeof(rep_num) / sizeof(rep_num[0]), (LPARAM)rep_num);
				SendMessage(GetDlgItem(handler, RP_Ciudad), WM_SETTEXT, sizeof(lista_heapquick[seleccion].ciudad) / sizeof(lista_heapquick[seleccion].ciudad[0]), (LPARAM)lista_heapquick[seleccion].ciudad);
				SendMessage(GetDlgItem(handler, RP_Estado), WM_SETTEXT, sizeof(lista_heapquick[seleccion].estado) / sizeof(lista_heapquick[seleccion].estado[0]), (LPARAM)lista_heapquick[seleccion].estado);
				SendMessage(GetDlgItem(handler, RP_Marca), WM_SETTEXT, sizeof(lista_heapquick[seleccion].vacuna) / sizeof(lista_heapquick[seleccion].vacuna[0]), (LPARAM)lista_heapquick[seleccion].vacuna);
				char rep_ndosis[10];
				//lista_heapquick[0]
				_itoa_s(lista_heapquick[seleccion].No_dosis, rep_ndosis, sizeof(rep_ndosis));
				SendMessage(GetDlgItem(handler, RP_DosisActual), WM_SETTEXT, sizeof(rep_ndosis) / sizeof(rep_ndosis[0]), (LPARAM)rep_ndosis);
				char rep_prox[50], rep_uda[50];
				GetDateFormat(NULL, 0, &(lista_heapquick[seleccion].fecha_aplicacion), NULL, rep_uda, sizeof(rep_uda));
				GetDateFormat(NULL, 0, &(lista_heapquick[seleccion].prox_dosis), NULL, rep_prox, sizeof(rep_prox));
				SendMessage(GetDlgItem(handler, RP_ProximaDosis), WM_SETTEXT, sizeof(rep_prox) / sizeof(rep_prox[0]), (LPARAM)rep_prox);
				SendMessage(GetDlgItem(handler, RP_UltimaDosis), WM_SETTEXT, sizeof(rep_uda) / sizeof(rep_uda[0]), (LPARAM)rep_uda);

				HBITMAP foto = (HBITMAP)LoadImage(NULL, lista_heapquick[seleccion].foto, IMAGE_BITMAP, 128, 128, LR_LOADFROMFILE);
				SendMessage(GetDlgItem(handler, RP_Foto), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);
			}
			break;
		}
		case WM_DESTROY: {
			EndDialog(handler, 0);
		}

		}
		break;
	}
	}
	return false;

}
BOOL CALLBACK Buscar(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {

	int aux_carnetint = 0;
	char aux_nombre[100];
	char aux_carnet[10];
	char aux_edad[10];
	char aux_dosis[10];
	char fecha_1[64];
	char fecha_2[64];
	char fecha_nac[64];
	HWND hwnd_lista = GetDlgItem(handler, BBCC_ListaBusqueda);
	switch (mensaje) {
	case WM_INITDIALOG: {
		char texto_1[] = "Buscar Carnet ";
		char texto_com[256];
		strcpy_s(texto_com, texto_1);
		strcat_s(texto_com, " - Usuario activo: ");
		strcat_s(texto_com, usuario_activo);
		SetWindowText(handler, texto_com);
		break;
	}

	case WM_COMMAND: {

		int interacion = LOWORD(wParam);
		switch (interacion) {
		case B_NombreSec: {
			seleccion_busqueda = busqueda_nombre;
			SendMessage(GetDlgItem(handler, BBCC_Cambio), WM_SETTEXT, NULL, (LPARAM)"Nombre:");
			ShowWindow(hwnd_lista, SW_SHOW);
			break;
		}
		case B_CarnetBin: {
			seleccion_busqueda = busqueda_carnet;
			SendMessage(GetDlgItem(handler, BBCC_Cambio), WM_SETTEXT, NULL, (LPARAM)"No.Carnet:");

			ShowWindow(hwnd_lista, SW_HIDE);
			break;
		}
		case BBCC_Buscar: {
			SendMessage(hwnd_lista, LB_RESETCONTENT, NULL, NULL);
			for (int y = 0; y < mostrar; y++) {
				lista_search[y] = {};
			}
			mostrar = 0;
			for (int y = 0; y < indice_carnet; y++) {
				lista_search[y] = lista_carnet[y];
			}
			switch (seleccion_busqueda) {
			case 1: // busqueda por nombre
				SendMessage(GetDlgItem(handler, BBCC_Dato), WM_GETTEXT, sizeof(aux_nombre) / sizeof(aux_nombre[0]), (LPARAM)aux_nombre);
				if (regex_match(aux_nombre, letras)) {
					if (*aux_nombre != NULL) {
						busqueda_secuencial(aux_nombre);
						if (lista_search[0].nombre_persona == NULL) {
							MessageBox(NULL, "No hay registros", "Aviso", MB_OK | MB_ICONEXCLAMATION);
						}
						else {
							int i = 0;
							while (i != mostrar) {
								char aux_mostra_lista[512];
								strcpy_s(aux_mostra_lista, lista_search[i].nombre_persona);
								strcat_s(aux_mostra_lista, " ");
								strcat_s(aux_mostra_lista, lista_search[i].npaterno);
								strcat_s(aux_mostra_lista, " ");
								strcat_s(aux_mostra_lista, lista_search[i].nmaterno);
								MessageBox(NULL, "Se muestran nombres iguales", "Aviso", MB_OK | MB_ICONEXCLAMATION);
								SendMessage(GetDlgItem(handler, BBCC_ListaBusqueda), LB_ADDSTRING, NULL, (LPARAM)aux_mostra_lista);
								i++;
							}
						}
					}
					else {
						MessageBox(NULL, "Llene el campo de busqueda", "Aviso", MB_OK | MB_ICONEXCLAMATION);
					}
				}
				else {
					MessageBox(NULL, "Busqueda por nombre deben de ser letras", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				}

				break;

			case 2:// busqueda por carnet 
				SendMessage(GetDlgItem(handler, BBCC_Dato), WM_GETTEXT, sizeof(aux_carnet) / sizeof(aux_carnet[0]), (LPARAM)aux_carnet);
				if (regex_match(aux_carnet, numeros)) {
					if (*aux_carnet != NULL) {
						aux_carnetint = atoi(aux_carnet);
						busqueda_binaria(aux_carnetint);
						if (encontrar == true) {
							encontrar = false;
							/**/
							valor_busqueda = valor_busqueda - 1;
							MessageBox(NULL, "Carnet encontrado", "Aviso", MB_OK | MB_ICONEXCLAMATION);
							GetDateFormat(NULL, 0, &(lista_search[valor_busqueda].fecha_aplicacion), NULL, fecha_1, sizeof(fecha_1));
							GetDateFormat(NULL, 0, &(lista_search[valor_busqueda].prox_dosis), NULL, fecha_2, sizeof(fecha_2));
							GetDateFormat(NULL, 0, &(lista_search[valor_busqueda].fecha_nac), NULL, fecha_nac, sizeof(fecha_nac));


							SendDlgItemMessage(handler, BBCC_Nombre, WM_SETTEXT, sizeof(lista_search[valor_busqueda].nombre_persona) / sizeof(lista_search[valor_busqueda].nombre_persona[0]), (LPARAM)lista_search[valor_busqueda].nombre_persona);
							SendMessage(GetDlgItem(handler, BBCC_ApellidoP), WM_SETTEXT, sizeof(lista_search[valor_busqueda].npaterno) / sizeof(lista_search[valor_busqueda].npaterno[0]), (LPARAM)lista_search[valor_busqueda].npaterno);
							SendMessage(GetDlgItem(handler, BBCC_ApellidoM), WM_SETTEXT, sizeof(lista_search[valor_busqueda].nmaterno) / sizeof(lista_search[valor_busqueda].nmaterno[0]), (LPARAM)lista_search[valor_busqueda].nmaterno);
							_itoa_s(lista_search[valor_busqueda].edad, aux_edad, sizeof(aux_edad));
							SendMessage(GetDlgItem(handler, BBCC_Edad), WM_SETTEXT, sizeof(aux_edad) / sizeof(aux_edad[0]), (LPARAM)aux_edad);
							SendMessage(GetDlgItem(handler, BBCC_Sexo), WM_SETTEXT, sizeof(lista_search[valor_busqueda].sexo) / sizeof(lista_search[valor_busqueda].sexo[0]), (LPARAM)lista_search[valor_busqueda].sexo);

							//SendMessage(GetDlgItem(handler, BBCC_Clave), WM_SETTEXT, sizeof(lista_search[valor_busqueda].lote) / sizeof(lista_search[valor_busqueda].lote[0]), (LPARAM)lista_search[valor_busqueda].lote);
							_itoa_s(lista_search[valor_busqueda].No_dosis, aux_dosis, sizeof(aux_dosis));
							SendMessage(GetDlgItem(handler, BBCC_LugarAplicacion), WM_SETTEXT, sizeof(lista_search[valor_busqueda].lugar) / sizeof(lista_search[valor_busqueda].lugar[0]), (LPARAM)lista_search[valor_busqueda].lugar);

							SendMessage(GetDlgItem(handler, BBCC_DosisAplicar), WM_SETTEXT, sizeof(aux_dosis) / sizeof(aux_dosis[0]), (LPARAM)aux_dosis);
							SendMessage(GetDlgItem(handler, BBCC_VacunaAplicar), WM_SETTEXT, sizeof(lista_search[valor_busqueda].vacuna) / sizeof(lista_search[valor_busqueda].vacuna[0]), (LPARAM)lista_search[valor_busqueda].vacuna);
							SendMessage(GetDlgItem(handler, BBCC_TipoVacuna), WM_SETTEXT, sizeof(lista_search[valor_busqueda].z_vac) / sizeof(lista_search[valor_busqueda].z_vac[0]), (LPARAM)lista_search[valor_busqueda].z_vac);

							SendMessage(GetDlgItem(handler, BBCC_FechaAplicacion), WM_SETTEXT, sizeof(fecha_1) / sizeof(fecha_1[0]), (LPARAM)fecha_1);

							SendMessage(GetDlgItem(handler, BBCC_ProximaDosis), WM_SETTEXT, sizeof(fecha_2) / sizeof(fecha_2[0]), (LPARAM)fecha_2);

							SendMessage(GetDlgItem(handler, BBCC_FechaNacimiento), WM_SETTEXT, sizeof(fecha_nac) / sizeof(fecha_nac[0]), (LPARAM)fecha_nac);


							HBITMAP foto = (HBITMAP)LoadImage(NULL, lista_search[valor_busqueda].foto, IMAGE_BITMAP, 256, 256, LR_LOADFROMFILE);

							SendMessage(GetDlgItem(handler, BBCC_Foto), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);
						}
						else {
							MessageBox(NULL, "Carnet no existe", "Aviso", MB_OK | MB_ICONEXCLAMATION);
						}

					}
					else {
						MessageBox(NULL, "Llene el campo de busqueda", "Aviso", MB_OK | MB_ICONEXCLAMATION);
					}
				}
				else {
					MessageBox(NULL, "Busqueda por carnet deben ser numeros", "Aviso", MB_OK | MB_ICONEXCLAMATION);
				}

				break;

			}
			break;

		}
		case BBCC_ListaBusqueda: {
			if (HIWORD(wParam) == LBN_DBLCLK) {
				int seleccion = SendMessage(GetDlgItem(handler, BBCC_ListaBusqueda), LB_GETCURSEL, NULL, NULL);

				MessageBox(NULL, "Carnet encontrado", "Aviso", MB_OK | MB_ICONEXCLAMATION);

				GetDateFormat(NULL, 0, &(lista_search[seleccion].fecha_aplicacion), NULL, fecha_1, sizeof(fecha_1));
				GetDateFormat(NULL, 0, &(lista_search[seleccion].prox_dosis), NULL, fecha_2, sizeof(fecha_2));
				GetDateFormat(NULL, 0, &(lista_search[seleccion].fecha_nac), NULL, fecha_nac, sizeof(fecha_nac));

				SendDlgItemMessage(handler, BBCC_Nombre, WM_SETTEXT, sizeof(lista_search[seleccion].nombre_persona) / sizeof(lista_search[seleccion].nombre_persona[0]), (LPARAM)lista_search[seleccion].nombre_persona);
				SendMessage(GetDlgItem(handler, BBCC_ApellidoP), WM_SETTEXT, sizeof(lista_search[seleccion].npaterno) / sizeof(lista_search[seleccion].npaterno[0]), (LPARAM)lista_search[seleccion].npaterno);
				SendMessage(GetDlgItem(handler, BBCC_ApellidoM), WM_SETTEXT, sizeof(lista_search[seleccion].nmaterno) / sizeof(lista_search[seleccion].nmaterno[0]), (LPARAM)lista_search[seleccion].nmaterno);
				_itoa_s(lista_search[seleccion].edad, aux_edad, sizeof(aux_edad));
				SendMessage(GetDlgItem(handler, BBCC_Edad), WM_SETTEXT, sizeof(aux_edad) / sizeof(aux_edad[0]), (LPARAM)aux_edad);
				SendMessage(GetDlgItem(handler, BBCC_Sexo), WM_SETTEXT, sizeof(lista_search[seleccion].sexo) / sizeof(lista_search[seleccion].sexo[0]), (LPARAM)lista_search[seleccion].sexo);

				//SendMessage(GetDlgItem(handler, BBCC_Clave), WM_SETTEXT, sizeof(lista_search[seleccion].lote) / sizeof(lista_search[seleccion].lote[0]), (LPARAM)lista_search[seleccion].lote);
				_itoa_s(lista_search[seleccion].No_dosis, aux_dosis, sizeof(aux_dosis));
				SendMessage(GetDlgItem(handler, BBCC_LugarAplicacion), WM_SETTEXT, sizeof(lista_search[seleccion].lugar) / sizeof(lista_search[seleccion].lugar[0]), (LPARAM)lista_search[seleccion].lugar);

				SendMessage(GetDlgItem(handler, BBCC_DosisAplicar), WM_SETTEXT, sizeof(aux_dosis) / sizeof(aux_dosis[0]), (LPARAM)aux_dosis);
				SendMessage(GetDlgItem(handler, BBCC_VacunaAplicar), WM_SETTEXT, sizeof(lista_search[seleccion].vacuna) / sizeof(lista_search[seleccion].vacuna[0]), (LPARAM)lista_search[seleccion].vacuna);
				SendMessage(GetDlgItem(handler, BBCC_TipoVacuna), WM_SETTEXT, sizeof(lista_search[seleccion].z_vac) / sizeof(lista_search[seleccion].z_vac[0]), (LPARAM)lista_search[seleccion].z_vac);

				SendMessage(GetDlgItem(handler, BBCC_FechaAplicacion), WM_SETTEXT, sizeof(fecha_1) / sizeof(fecha_1[0]), (LPARAM)fecha_1);

				SendMessage(GetDlgItem(handler, BBCC_ProximaDosis), WM_SETTEXT, sizeof(fecha_2) / sizeof(fecha_2[0]), (LPARAM)fecha_2);

				SendMessage(GetDlgItem(handler, BBCC_FechaNacimiento), WM_SETTEXT, sizeof(fecha_nac) / sizeof(fecha_nac[0]), (LPARAM)fecha_nac);

				HBITMAP foto = (HBITMAP)LoadImage(NULL, lista_search[valor_busqueda].foto, IMAGE_BITMAP, 256, 256, LR_LOADFROMFILE);
				SendMessage(GetDlgItem(handler, BBCC_Foto), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);
			}
			break;
		}
		case IDCANCEL: {
			EndDialog(handler, 0);
			break;
		}

		}
		break;
	}
	}
	return false;
}



int WINAPI WinMain(HINSTANCE instancia, HINSTANCE legado, LPSTR comando, int modo) {
	DialogBox(instancia, MAKEINTRESOURCE(V_Login), NULL, (DLGPROC)Inicio_Sesion);
	return 0;
}