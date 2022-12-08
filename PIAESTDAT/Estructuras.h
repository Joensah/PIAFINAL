#include <Windows.h>
#include <CommCtrl.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include "resource.h"
#include <mmsystem.h>
#include <CommDlg.h>
#include <iostream>
#include <wchar.h>
#pragma once
using namespace std;

struct USUARIO {
	char nombre_usuario[100];
	char contrasena[100];
	//Datos del nombre_usuario.
	char nombre_usuario[100];
	char Uapellido[100];

	void setUsuario(string usuarioP) {
		strcpy_s(nombre_usuario, 100, usuarioP.c_str());
	}
	string getUsuario() {
		string str(nombre_usuario);
		return str;
	}
	void setContra(string contraP) {
		strcpy_s(contrasena, 100, contraP.c_str());
	}
	string getContra() {
		string str(contrasena);
		return str;
	}
	void setUnombre(string UnombreP) {
		strcpy_s(nombre_usuario, 100, UnombreP.c_str());
	}
	string getUnombre() {
		string str(nombre_usuario);
		return str;
	}
	void setUapellido(string UapellidoP) {
		strcpy_s(Uapellido, 100, UapellidoP.c_str());
	}
	string getUapellido() {
		string str(Uapellido);
		return str;
	}
};

struct NODOUSUARIO {
	USUARIO* nombre_usuario = NULL;
	NODOUSUARIO* siguiente = NULL;
	NODOUSUARIO* anterior = NULL;
};

struct LISTAUSUARIOS {
	NODOUSUARIO* origen = NULL;
	NODOUSUARIO* fin = NULL;
	void agregar(USUARIO* nuevo) {
		if (origen == NULL) {
			origen = new NODOUSUARIO;
			origen->nombre_usuario = nuevo;
		}
		else {

			NODOUSUARIO* elemento = origen;
			while (elemento->siguiente != NULL) {
				elemento = elemento->siguiente;
			}
			elemento->siguiente = new NODOUSUARIO;
			elemento->siguiente->nombre_usuario = nuevo;
			elemento->siguiente->anterior = elemento;
			fin = elemento->siguiente;
		}
	}
}USUARIOS;

struct PERSONA {
	char paterno[100];
	char materno[100];
	char nombre_persona[100];
	char CURP[100];
	char RFC[100];
	char FECHANACIMIENTO[100];
	char calle[100];
	char colonia[100];
	char ciudad[100];
	char estado[100];
	char e_civil[100];
	char telefono[100];
	char foto_c[100];
	char RUTA[MAX_PATH];
	char sexo[20];
	char Gpo_ocupacional[100];
	char Perfil_riesgo[100];
	bool ACTIVO = true;

	void setAPEPAT(string apepatP) {
		strcpy_s(paterno, 100, apepatP.c_str());
	}
	string getAPEPAT() {
		string str(paterno);
		return str;
	}

	void setAPEMAT(string apematP) {
		strcpy_s(materno, 100, apematP.c_str());
	}
	string getAPEMAT() {
		string str(materno);
		return str;
	}

	void setNOMBRES(string nombresP) {
		strcpy_s(nombre_persona, 100, nombresP.c_str());
	}
	string getNOMBRES() {
		string str(nombre_persona);
		return str;
	}

	void setCURP(string curpP) {
		strcpy_s(CURP, 100, curpP.c_str());
	}
	string getCURP() {
		string str(CURP);
		return str;
	}

	void setRFC(string RFCP) {
		strcpy_s(RFC, 100, RFCP.c_str());
	}
	string getRFC() {
		string str(RFC);
		return str;
	}

	void setFECHA(string fechaP) {
		strcpy_s(FECHANACIMIENTO, 100, fechaP.c_str());
	}
	string getFECHA() {
		string str(FECHANACIMIENTO);
		return str;
	}

	void setCALLE(string calleP) {
		strcpy_s(calle, 100, calleP.c_str());
	}
	string getCALLE() {
		string str(calle);
		return str;
	}

	void setCOLONIA(string coloniaP) {
		strcpy_s(colonia, 100, coloniaP.c_str());
	}
	string getCOLONIA() {
		string str(colonia);
		return str;
	}

	void setCIUDAD(string ciudadP) {
		strcpy_s(ciudad, 100, ciudadP.c_str());
	}
	string getCIUDAD() {
		string str(ciudad);
		return str;
	}

	void setESTADO(string estadoP) {
		strcpy_s(estado, 100, estadoP.c_str());
	}
	string getESTADO() {
		string str(estado);
		return str;
	}

	void setESTACIVIL(string estacivilP) {
		strcpy_s(e_civil, 100, estacivilP.c_str());
	}
	string getESTACIVIL() {
		string str(e_civil);
		return str;
	}

	void setTELEFONO(string telefonoP) {
		strcpy_s(telefono, 100, telefonoP.c_str());
	}
	string getTELEFONO() {
		string str(telefono);
		return str;
	}

	void setDOCIDE(string docideP) {
		strcpy_s(foto_c, 100, docideP.c_str());
	}
	string getDOCIDE() {
		string str(foto_c);
		return str;
	}

	void setRUTA(string rutaP) {
		strcpy_s(RUTA, MAX_PATH, rutaP.c_str());
	}
	string getRUTA() {
		string str(RUTA);
		return str;
	}

	void setSEXO(string sexoP) {
		strcpy_s(sexo, 20, sexoP.c_str());
	}
	string getSEXO() {
		string str(sexo);
		return str;
	}

	void setGRUPO(string grupoP) {
		strcpy_s(Gpo_ocupacional, 100, grupoP.c_str());
	}
	string getGRUPO() {
		string str(Gpo_ocupacional);
		return str;
	}

	void setPERFIL(string PerfilP) {
		strcpy_s(Perfil_riesgo, 100, PerfilP.c_str());
	}
	string getPERFIL() {
		string str(Perfil_riesgo);
		return str;
	}
	void setACTIVO(bool ActivoP) {
		ACTIVO = ActivoP;
	}
	bool getACTIVO() {
		bool cambio(ACTIVO);
		return cambio;
	}
};

struct NODOPERSONA {
	PERSONA* persona = NULL;
	NODOPERSONA* siguiente = NULL;
	NODOPERSONA* anterior = NULL;
};

struct LISTAPERSONAS {
	NODOPERSONA* origen = NULL;
	NODOPERSONA* fin = NULL;
	void agregarP(PERSONA* nuevo) {
		if (origen == NULL) {
			origen = new NODOPERSONA;
			origen->persona = nuevo;
		}
		else {

			NODOPERSONA* elemento = origen;
			while (elemento->siguiente != NULL) {
				elemento = elemento->siguiente;
			}
			elemento->siguiente = new NODOPERSONA;
			elemento->siguiente->persona = nuevo;
			elemento->siguiente->anterior = elemento;
			fin = elemento->siguiente;
		}
	}
}PERSONAS;

struct VACUNA {
	char numero_lote[100] = "";
	int precio = NULL;
	char tipo[25] = "";
	int dosis_requeridas = NULL;
	char marca[50] = "";
	char descripcion[100] = "";

	void setCLAVEUNICA(string claveunicaP) {
		strcpy_s(numero_lote, 100, claveunicaP.c_str());
	}
	string getCLAVEUNICA() {
		string str(numero_lote);
		return str;
	}
	void setPRECIO(string precioP) {
		int num = stoi(precioP);
		precio = num;
	}
	int getPrecio() {
		int ente(precio);
		return ente;
	}
	void setTIPO(string tipovacunaP) {
		strcpy_s(tipo, 25, tipovacunaP.c_str());
	}
	string getTIPO() {
		string str(tipo);
		return str;
	}
	void setNODOSIS(string nodosisP) {
		int num = stoi(nodosisP);
		dosis_requeridas = num;
	}
	int getNODOSIS() {
		int ente(dosis_requeridas);
		return ente;
	}
	void setMARCA(string marcaP) {
		strcpy_s(marca, 50, marcaP.c_str());
	}
	string getMARCA() {
		string str(marca);
		return str;
	}
	void setDESC(string descP) {
		strcpy_s(descripcion, 50, descP.c_str());
	}
	string getDESC() {
		string str(descripcion);
		return str;
	}
};

struct NODOVACUNAS {
	VACUNA* vacuna = NULL;
	NODOVACUNAS* siguiente = NULL;
	NODOVACUNAS* anterior = NULL;
};

struct LISTAVACUNAS {
	NODOVACUNAS* origen = NULL;
	NODOVACUNAS* fin = NULL;
	void agregarV(VACUNA* nuevo) {
		if (origen == NULL) {
			origen = new NODOVACUNAS;
			origen->vacuna = nuevo;
		}
		else {

			NODOVACUNAS* elemento = origen;
			while (elemento->siguiente != NULL) {
				elemento = elemento->siguiente;
			}
			elemento->siguiente = new NODOVACUNAS;
			elemento->siguiente->vacuna = nuevo;
			elemento->siguiente->anterior = elemento;
			fin = elemento->siguiente;
		}
	}
	void eliminarV(VACUNA* existente) {
		if (origen == NULL) {
			return;
		}
		else {
			VACUNA* pasador = origen->vacuna;
			NODOVACUNAS* apoyo = origen;
			bool encontrar = false;
			while (apoyo != NULL) {
				pasador = apoyo->vacuna;
				if (strcmp(pasador->numero_lote, existente->numero_lote) == 0) {
					encontrar = true;
					break;
				}
				apoyo = apoyo->siguiente;//COMO AQU{I EST{A BUSCANDO, EL {ULTIMO DATO CON EL QUE SE QUEDA PASADOR ES EL DATO QUE NOSOTROS BUSCAMOS. POR ESO PODEMOS USARLO ABAJO
			}
			if (encontrar) {
				pasador = existente;
				if (apoyo->vacuna->numero_lote == pasador->numero_lote) {
					if (apoyo->anterior == NULL && apoyo->siguiente == NULL) { //El unico
						delete existente; //Borramos el unico elemento
						origen = NULL; //Regresamos todo a NULL para que se cumpla el primer if de ALTA
					}
					else if (apoyo->anterior == NULL) {
						origen = origen->siguiente;
						origen->anterior = NULL;
						delete existente;
					}
					else if (apoyo->siguiente == NULL) { //Ultimo
						apoyo->anterior->siguiente = NULL; // De aux (Ultimo) viajo a anterior (Elemento anterior) y su sig lo apunto a NULL para indicar que ese es el nuevo ultimo elemento
						delete existente; //Borro aux
					}
					else {
						apoyo->anterior->siguiente = apoyo->siguiente;
						apoyo->siguiente->anterior = apoyo->anterior;
						delete existente;
					}
					MessageBox(NULL,"La vacuna se ha eliminado","Eliminado", MB_ICONASTERISK | MB_TASKMODAL);
					//aquí son que se guarden los datos
				}
				else {
					MessageBox(NULL,"Error en el proceso de eliminar","Error de Eliminado", MB_ICONASTERISK | MB_TASKMODAL);
				}
			}
		}
	}
}VACUNAS;

struct CARNET {
	char CARCURP[100];
	char CARVACUNA[25];
	int CARDOSIS = NULL;
	char CARFEDOSIS[100];
	int LOTE = NULL;
	char CENTRO[100];
	int ID;

	NODOPERSONA* conectorCaP;//ESTA CONECTA A LA ESTRUCTURA PERSONAS

	void setCARCURP(string carcurpP) {
		strcpy_s(CARCURP, 100, carcurpP.c_str());
	}
	string getCARCURP() {
		string str(CARCURP);
		return str;
	}
	void setCARVACUNA(string carvacunaP) {
		strcpy_s(CARVACUNA, 25, carvacunaP.c_str());
	}
	string getCARVACUNA() {
		string str(CARVACUNA);
		return str;
	}
	void setCARDOSIS(string cardosisP) {
		int num = stoi(cardosisP);
		CARDOSIS = num;
	}
	int getCARDOSIS() {
		int ente(CARDOSIS);
		return ente;
	}
	void setCARFEDOSIS(string carfedosisP) {
		strcpy_s(CARFEDOSIS, 100, carfedosisP.c_str());
	}
	string getCARFEDOSIS() {
		string str(CARFEDOSIS);
		return str;
	}
	void setLOTE(string loteP) {
		int num = stoi(loteP);
		LOTE = num;
	}
	int getLOTE() {
		int ente(LOTE);
		return ente;
	}
	void setCENTRO(string centroP) {
		strcpy_s(CENTRO, 100, centroP.c_str());
	}
	string getCENTRO() {
		string str(CENTRO);
		return str;
	}
	void setID(int idP) {
		ID = idP;
	}
	int getID() {
		int ente(ID);
		return ente;
	}
	void setConector(NODOPERSONA* conector) {
		conectorCaP = conector;
	}
	NODOPERSONA getConector() {
		if (PERSONAS.origen != NULL) {
			NODOPERSONA* susorigen = PERSONAS.origen;
			PERSONA* elemento1 = PERSONAS.origen->persona;

			while (susorigen != NULL) {
				elemento1 = susorigen->persona;
				if (susorigen->persona != NULL && strcmp(elemento1->CURP, CARCURP) == 0) {
					break;
				}
				susorigen = susorigen->siguiente;
			}
			conectorCaP = susorigen;
		}

	}

	static CARNET* nuevoCarnet(string carcurp, string carvacuna, string cardosis, string carfedosis, string lote, string centro, NODOPERSONA* conector) {
		CARNET* nuevo = new CARNET;
		if (nuevo == NULL) {
			cout << "Error, no se pudo crear el carnet en memoria" << endl;
			return NULL;
		}
		nuevo->setCARCURP(carcurp);
		nuevo->setCARVACUNA(carvacuna);
		nuevo->setCARDOSIS(cardosis);
		nuevo->setCARFEDOSIS(carfedosis);
		nuevo->setLOTE(lote);
		nuevo->setCENTRO(centro);
		nuevo->setConector(conector);
		return nuevo;
	}
};

struct NODOCARNETS {
	CARNET* carnet = NULL;
	NODOCARNETS* siguiente = NULL;
	NODOCARNETS* anterior = NULL;

	static NODOCARNETS* nuevoNodo(string carcurp, string carvacuna, string cardosis, string carfedosis, string lote, string centro, NODOPERSONA* conector) {
		NODOCARNETS* nuevo = new NODOCARNETS;
		if (nuevo == NULL) {
			cout << "Error, no se pudo crear el carnet en memoria" << endl;
			return NULL;
		}
		nuevo->carnet = CARNET::nuevoCarnet(carcurp, carvacuna, cardosis, carfedosis, lote, centro, conector);
		nuevo->siguiente = NULL;
		nuevo->anterior = NULL;
		return nuevo;
	}
	static NODOCARNETS* nuevoNodo(CARNET* datos) {
		NODOCARNETS* nuevo = new NODOCARNETS;
		if (nuevo == NULL) {
			cout << "Error, no se pudo crear el carnet en memoria" << endl;
			return NULL;
		}
		nuevo->carnet = datos;
		nuevo->siguiente = NULL;
		nuevo->anterior = NULL;
		return nuevo;
	}
};

struct LISTACARNETS {
	NODOCARNETS* origen = NULL;
	NODOCARNETS* fin = NULL;
	static int maxId;

	void agregarCarnet(string carcurp, string carvacuna, string cardosis, string carfedosis, string lote, string centro, NODOPERSONA* conector) {
		agregarCarnet(carcurp, carvacuna, cardosis, carfedosis, lote, centro, 0, conector);
	}

	void agregarCarnet(string carcurp, string carvacuna, string cardosis, string carfedosis, string lote, string centro, int id, NODOPERSONA* conector) {
		CARNET* nuevo = CARNET::nuevoCarnet(carcurp, carvacuna, cardosis, carfedosis, lote, centro, conector);
		if (nuevo == NULL) {
			exit(-1);
		}
		agregarCarnet(nuevo, 0);
	}

	void agregarCarnet(CARNET* nuevo) {
		agregarCarnet(nuevo, 0);
	}
	void agregarCarnet(CARNET* nuevo, int idPersona) {
		if (idPersona == 0) {
			LISTACARNETS::maxId++;
			nuevo->ID = LISTACARNETS::maxId;
		}
		else {
			nuevo->ID = idPersona;
			LISTACARNETS::maxId = LISTACARNETS::maxId < idPersona ? idPersona : LISTACARNETS::maxId;
		}
		if (origen == NULL) {
			origen = NODOCARNETS::nuevoNodo(nuevo);
			fin = origen;
		}
		else {
			fin->siguiente = NODOCARNETS::nuevoNodo(nuevo);
			fin->siguiente->anterior = fin;
			fin = fin->siguiente;
		}
	}

	NODOCARNETS* buscarNodoCarnet(string nombre) {
		NODOCARNETS* indice = origen;
		while (indice != NULL) {
			if (_stricmp(nombre.c_str(), indice->carnet->conectorCaP->persona->nombre_persona) == 0)
				return indice;
			indice = indice->siguiente;
		}
		return indice;
	}
	CARNET* buscarCarnet(string personanombre) {
		NODOCARNETS* indice = buscarNodoCarnet(personanombre);
		return indice == NULL ? NULL : indice->carnet;
	}

	bool modificarCarnet(string nombrebuscar, string carcurp, string carvacuna, string cardosis, string carfedosis, string lote, string centro, int id, NODOPERSONA* conector) {
		CARNET* carnet = buscarCarnet(nombrebuscar);
		return modificarCarnet(carnet, carcurp, carvacuna, cardosis, carfedosis, lote, centro, id, conector);
	}

	bool modificarCarnet(CARNET* carnet, string carcurp, string carvacuna, string cardosis, string carfedosis, string lote, string centro, int id, NODOPERSONA* conector) {
		if (carnet != NULL) {
			carnet->setCARCURP(carcurp);
			carnet->setCARVACUNA(carvacuna);
			carnet->setCARDOSIS(cardosis);
			carnet->setCARFEDOSIS(carfedosis);
			carnet->setLOTE(lote);
			carnet->setCENTRO(centro);
			carnet->setID(id);
			carnet->setConector(conector);
			return true;
		}
		return false;
	}

	CARNET* eliminarCarnet(string nombreBuscar) {
		NODOCARNETS* carnet = buscarNodoCarnet(nombreBuscar);
		CARNET* p = NULL;
		if (carnet != NULL) {
			if (carnet->anterior == NULL) {
				origen = carnet->siguiente;
				if (carnet == fin) fin = NULL;
				else origen->anterior = NULL;
				p = carnet->carnet;
				delete carnet;
				CARNETS.maxId--;
			}
			else if (carnet->siguiente == NULL) {
				fin = carnet->anterior;
				if (fin != NULL)
					fin->siguiente = NULL;
				p = carnet->carnet;
				delete carnet;
				CARNETS.maxId--;
			}
			else {
				NODOCARNETS* anterior = carnet->anterior,
					* siguiente = carnet->siguiente;
				anterior->siguiente = siguiente;
				siguiente->anterior = anterior;
				p = carnet->carnet;
				delete carnet;
				CARNETS.maxId--;
			}
			return p;
		}
		return p;
	}

	void eliminarTodo() {
		NODOCARNETS* indice = origen;
		while (indice != NULL) {
			NODOCARNETS* actual = indice;
			indice = indice->siguiente;
			delete actual;
		}
		origen = NULL;
		fin = NULL;
	}
}CARNETS;
int LISTACARNETS::maxId = 0;

struct NODOARBOLCARNET {
	NODOARBOLCARNET* PADRE = NULL;
	NODOARBOLCARNET* IZQUIERDO = NULL;
	NODOARBOLCARNET* DERECHO = NULL;
	CARNET* CARNET = NULL;
	int balance = 0;
	static NODOARBOLCARNET* nuevoNodo(struct CARNET* info) {
		NODOARBOLCARNET* nuevo = new NODOARBOLCARNET;
		if (nuevo == NULL)
			exit(-1);
		nuevo->CARNET = info;
		nuevo->PADRE = NULL;
		nuevo->IZQUIERDO = NULL;
		nuevo->DERECHO = NULL;
		return nuevo;
	}
};

struct ARBOLCARNET {
	NODOARBOLCARNET* RAIZ = NULL;
	bool balanceo = false;
	/* Rotación Izquierda *
	*  A           B
	* / \         / \
	* a  B ==>   A   c
	*   / \     / \
	*   b  c    a  b
	* Sólo cambian los factores de balance de los nodos A y B
	* Los factores de balance de los sub-árboles no cambian. */
	NODOARBOLCARNET* rotacionIzquierda(NODOARBOLCARNET* t) {
		NODOARBOLCARNET* temp, * padre = t->PADRE;
		int x, y;
		temp = t;
		t = t->DERECHO;
		temp->DERECHO = t->IZQUIERDO;
		t->IZQUIERDO = temp;
		//Recalcula factores de balance de los dos nodos
		x = temp->balance; // oldbal(A)
		y = t->balance; // oldbal(B)
		temp->balance = x - 1 - max(y, 0); // nA
		t->balance = min(x - 2 + min(y, 0), y - 1); // nB

		return t;
	}
	/* Rotación derecha
	*
	*   A         B
	*  / \       / \
	*  B  c ==> a   A
	* / \          / \
	* a  b        b   c
	*
	*/
	NODOARBOLCARNET* rotacionDerecha(NODOARBOLCARNET* t) {
		NODOARBOLCARNET* temp = t, * padre = t->PADRE;
		int x, y;
		t = t->IZQUIERDO;
		temp->IZQUIERDO = t->DERECHO;
		t->DERECHO = temp;
		x = temp->balance; // oldbal(A)
		y = t->balance;    // oldbal(B)
		temp->balance = x + 1 - min(y, 0);  // nA
		t->balance = max(x + 2 + max(y, 0), y + 1); //nB
		return t;
	}
	int calcularEquilibrios(NODOARBOLCARNET* t) {
		if (t == NULL)
			return 0;
		int eIzquierdo = calcularEquilibrios(t->IZQUIERDO) + 1;
		int eDerecho = calcularEquilibrios(t->DERECHO) + 1;
		t->balance = eDerecho - eIzquierdo;
		return eIzquierdo > eDerecho ? eIzquierdo : eDerecho;
	}
	NODOARBOLCARNET* buscarNodo(NODOARBOLCARNET* nodo, CARNET* carnet) {
		if (nodo == NULL)
			return NULL;
		if (nodo->CARNET->ID > carnet->ID)
			return buscarNodo(nodo->IZQUIERDO, carnet);
		else if (nodo->CARNET->ID < carnet->ID)
			return buscarNodo(nodo->DERECHO, carnet);
		return nodo;
	}
	NODOARBOLCARNET* buscarCarnet(CARNET* carnet) {
		return buscarNodo(arbolPersonas.RAIZ, carnet);
	}
	//ESTO ES LO DE BINARIO, PARA BUSCAR CON EL ID
	//TENGO QUE CONVERITR EL ID A INT
	NODOARBOLCARNET* buscarCarnet(int idPersona) {
		CARNET carnetVacia;
		carnetVacia.ID = idPersona;
		return buscarNodo(arbolPersonas.RAIZ, &carnetVacia);
	}

	void eliminarNodo(NODOARBOLCARNET** nodo, NODOARBOLCARNET* padre, NODOARBOLCARNET* eliminar) {
		NODOARBOLCARNET* elemento = *nodo;
		if (elemento == NULL) {
			this->balanceo = false;
		}
		else if (elemento->CARNET->ID < eliminar->CARNET->ID) {
			eliminarNodo(&(elemento->DERECHO), elemento, eliminar);
			elemento->balance -= 1;
		}
		else if (elemento->CARNET->ID > eliminar->CARNET->ID) {
			eliminarNodo(&(elemento->IZQUIERDO), elemento, eliminar);
			elemento->balance += 1;
		}
		else {
			if (elemento->IZQUIERDO == NULL) {
				NODOARBOLCARNET* tmp = elemento;
				(*nodo) = tmp->DERECHO;
				delete tmp;
				this->balanceo = true;
				return;
			}
			else if (elemento->DERECHO == NULL) {
				NODOARBOLCARNET* tmp = elemento;
				(*nodo) = tmp->IZQUIERDO;
				delete tmp;
				this->balanceo = true;
				return;
			}
			else   /* Tiene dos hijos */ {
				if (elemento->balance < 0) {
					/* Si cargado a la izquierda, elimina mayor descendiente hijo izq */
					NODOARBOLCARNET* tmp = elemento;
					tmp = (*nodo)->IZQUIERDO;
					while (tmp->DERECHO != NULL) tmp = tmp->DERECHO;
					(*nodo)->CARNET = tmp->CARNET;
					eliminarNodo(&((*nodo)->DERECHO), (*nodo), tmp);
					elemento = (*nodo);
					elemento->balance += 1;
				}
				else {
					/* Si cargado a la derecha, elimina menor descendiente hijo der */
					NODOARBOLCARNET* tmp = elemento;
					tmp = (*nodo)->IZQUIERDO;
					while (tmp->IZQUIERDO != NULL) tmp = tmp->IZQUIERDO;
					(*nodo)->CARNET = tmp->CARNET;
					eliminarNodo(&((*nodo)->IZQUIERDO), (*nodo), tmp);
					elemento = (*nodo);
					elemento->balance -= 1;
				}
			}
		}
		/* Mantiene árbol balanceado avl. Sólo una o dos rotaciones por descarte */
		if (this->balanceo == true) {
			/* Hay que revisar factores de balance en el ascenso*/
			if (elemento->balance < -1)  /* Si quedó desbalanceado por la izquierda y dejó de ser AVL */ {
				if (elemento->IZQUIERDO->balance > 0)  /*espejos casos c, d y e */ {
					/* Si el hijo izquierdo está cargado a la derecha */
					elemento->IZQUIERDO = rotacionIzquierda(elemento->IZQUIERDO);
					this->balanceo = true; /*Continuar revisando factores */
				}
				else if (elemento->IZQUIERDO->balance == 0)
					this->balanceo = false; /*No debe seguir el rebalance */
				else
					this->balanceo = true;/* Debe seguir revisando factores de balance */
				(*nodo) = rotacionDerecha(*nodo);
			}
			else if (elemento->balance > 1)  /* Si quedó desbalaceado por la derecha */ {
				if (elemento->DERECHO->balance < 0) {
					/* Si hijo derecho está cargado a la izquierda */
					(*nodo)->DERECHO = rotacionDerecha((*nodo)->DERECHO);
					this->balanceo = true; //debe seguir revisando. Caso d.
				}
				else if (elemento->DERECHO->balance == 0)
					this->balanceo = false; /* No debe seguir el rebalance. Caso c. */
				else //positivo
					this->balanceo = true;/* Debe seguir revisando factores de balance. Caso e. */
				(*nodo) = rotacionIzquierda(*nodo);
			}
			else if (elemento->balance == 0) /* Si estaba en +1 ó -1 y queda en cero */
				this->balanceo = true; /* Debe seguir corrigiendo. Caso b.*/
			else /* Si estaba en cero y queda en -1 ó +1 */
				this->balanceo = false; /* No debe seguir rebalanceando. Caso a.*/
		}
	}
	void borrarNodo(CARNET* eliminar) {
		NODOARBOLCARNET* nodo = buscarCarnet(eliminar);
		if (nodo != NULL) {
			calcularEquilibrios(RAIZ);
			eliminarNodo(&RAIZ, NULL, nodo);
		}
	}
	void insertarNodo(NODOARBOLCARNET** nodo, NODOARBOLCARNET* padre, NODOARBOLCARNET* nuevo) {
		NODOARBOLCARNET* elemento = *nodo;
		if (elemento == NULL) {
			*nodo = nuevo;
			elemento = nuevo;
			nuevo->PADRE = padre;
			nuevo->balance = 0;
			balanceo = true;
			return;
		}
		else {
			if (elemento->CARNET->ID > nuevo->CARNET->ID) {
				insertarNodo(&(elemento->IZQUIERDO), elemento, nuevo);
				elemento->balance -= 1;
			}
			else {
				insertarNodo(&(elemento->DERECHO), elemento, nuevo);
				elemento->balance += 1;
			}
		}
		if (this->balanceo == true) {
			/*El código a continuación es el costo adicional para mantener propiedad AVL */
			/* Mantiene árbol balanceado avl. Sólo una o dos rotaciones por inserción */
			if (elemento->balance < -1) {
				/* Quedó desbalanceado por la izquierda. Espejos Casos c y d.*/
				if (elemento->IZQUIERDO->balance > 0) {
					/* Si hijo izquierdo está cargado a la derecha */
					elemento->IZQUIERDO = rotacionIzquierda(elemento->IZQUIERDO);
					elemento->IZQUIERDO->PADRE = elemento;
					if (elemento->IZQUIERDO->DERECHO != NULL)
						elemento->IZQUIERDO->DERECHO->PADRE = elemento->IZQUIERDO;
					if (elemento->IZQUIERDO->IZQUIERDO != NULL)
						elemento->IZQUIERDO->IZQUIERDO->PADRE = elemento->IZQUIERDO;
				}
				*nodo = rotacionDerecha(elemento);
				this->balanceo = false; /* El subárbol no aumenta su altura */
				(*nodo)->PADRE = padre;
				if ((*nodo)->IZQUIERDO != NULL)
					(*nodo)->IZQUIERDO->PADRE = (*nodo);
				if ((*nodo)->DERECHO != NULL)
					(*nodo)->DERECHO->PADRE = (*nodo);
			}
			else if (elemento->balance > 1) {
				/* Si quedó desbalanceado por la derecha: Casos c y d.*/
				if (elemento->DERECHO->balance < 0) {
					/* Si hijo derecho está cargado a la izquierda Caso d.*/
					elemento->DERECHO = rotacionDerecha(elemento->DERECHO);
					elemento->DERECHO->PADRE = elemento;
					if (elemento->DERECHO->IZQUIERDO != NULL)
						elemento->DERECHO->IZQUIERDO->PADRE = elemento->DERECHO;
					if (elemento->DERECHO->DERECHO != NULL)
						elemento->DERECHO->DERECHO->PADRE = elemento->DERECHO;
				}
				*nodo = rotacionIzquierda(elemento); /* Caso c.*/
				this->balanceo = false; /* El subárbol no aumenta su altura */
				(*nodo)->PADRE = padre;
				if ((*nodo)->IZQUIERDO != NULL)
					(*nodo)->IZQUIERDO->PADRE = (*nodo);
				if ((*nodo)->DERECHO != NULL)
					(*nodo)->DERECHO->PADRE = (*nodo);
			}
			else if (elemento->balance == 0)/* La inserción lo balanceo */
				this->balanceo = false; /* El subárbol no aumenta su altura. Caso a*/
			else /* Quedó desbalanceado con -1 ó +1 Caso b */
				this->balanceo = true; /* Propaga ascendentemente la necesidad de rebalancear */
		}
	}
	void agregarNodo(CARNET* info) {
		NODOARBOLCARNET* nuevo = NODOARBOLCARNET::nuevoNodo(info);
		insertarNodo(&RAIZ, NULL, nuevo);
		calcularEquilibrios(RAIZ);
	}
	void lista2Arbol(NODOCARNETS* listaDoble) {
		eliminarArbol();
		NODOCARNETS* carnet = listaDoble;
		while (carnet != NULL) {
			agregarNodo(carnet->carnet);
			carnet = carnet->siguiente;
			//el imprimir del archivo no sé usarlo
			//imprimirArbol();
		}
	}
	void eliminarArbolNodos(NODOARBOLCARNET* nodo) {
		if (nodo == NULL) {
			return;
		}
		else {
			eliminarArbolNodos(nodo->IZQUIERDO);
			eliminarArbolNodos(nodo->DERECHO);
			delete(nodo);
		}
	}
	void eliminarArbol() {
		eliminarArbolNodos(RAIZ);
		RAIZ = NULL;
	}
	/*
	void imprimirNodos(NODOARBOLPERSONA* nodo, int espacios) {
		if (nodo == NULL)
			return;
		string espacio = "";
		for (int i = 0; i < espacios; i++)
			espacio.append(" ");
		cout << espacio << "+";
		nodo->PERSONA->imprimirPersonaId();
		if (nodo->IZQUIERDO != NULL || nodo->DERECHO != NULL) {
			cout << endl << espacio << " |\n";
			imprimirNodos(nodo->IZQUIERDO, espacios + 2);
			imprimirNodos(nodo->DERECHO, espacios + 2);
			cout << espacio << " -\n";
		}
		else cout << endl;
	}
	void imprimirArbol() {
		cout << "\n\t--INICIO--" << endl;
		imprimirNodos(RAIZ, 0);
		cout << "\t--FIN--" << endl;
	}*/

} arbolPersonas;