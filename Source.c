																		//LISTE DUBLE INLANTUITE
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

//structul
typedef struct Librarie Librarie;
struct Librarie {
	char* nume;
	int nr_carti;
	float suprafata;
};
//LDI
typedef struct ListaDubla ListaDubla;
struct ListaDubla {
	NodLDI* prim;
	NodLDI* ultim;
};
//nod
typedef struct NodLDI NodLDI;
struct NodLDI {
	Librarie info;
	NodLDI* next;
	NodLDI* prev;
};

//init librarie

Librarie initializareLibrarie(const char* nume, int nr_crti, float suprafata)
{
	Librarie l;
	l.nume = (char)malloc((strlen(nume) + 1) * sizeof(char));
	strcpy(l.nume, nume);
	l.nr_carti = nr_crti;
	l.suprafata = suprafata;
	return l;
}

void afisareLibrarie(Librarie l) {
	printf("Libraria %s are %d carti si suprafata de %f mp2\n", l.nume, l.nr_carti, l.suprafata);
}

//INSERARE LDI inceput

struct ListaDubla inserareInceputLdi(struct ListaDubla lista, Librarie librarie) {
	NodLDI* nou = malloc(sizeof(NodLDI));
	nou->info = librarie;
	nou->next = lista.prim;
	nou->prev = NULL;

	if (lista.prim) {
		lista.prim->prev = nou;
	}
	else {
		lista.ultim = nou;
	}
	lista.prim = nou;
	return lista;
}

//INSERARE LDI sfarsit

struct ListaDubla inserareSfarsitLDI(struct ListaDubla lista, Librarie librarie) {
	NodLDI* nou = malloc(sizeof(NodLDI));
	nou->next = NULL;
	nou->prev = lista.ultim;
	nou->info = librarie;
	if (lista.ultim != NULL) {
		lista.ultim->next = nou;
	}
	else {
		lista.prim = nou;
	}
	lista.ultim = nou;
	return lista;
}

//afisare LDI de la inceput

void afisareLDI(struct ListaDubla lista) {
	while (lista.prim) {
		afisareLibrarie(lista.prim->info);
		lista.prim = lista.prim->next;
	}
}

//afisare LDI de la sfarsit

void afisareLDISfarsit(ListaDubla lista) {
	NodLDI* p = lista.ultim;
	while (p) {
		afisareLibrarie(p->info);
		p = p->prev;
	}
}


void main() {

	Librarie l;
	l = initializareLibrarie("Eminescu", 400, 50.5f);
	Librarie l1;
	l1 = initializareLibrarie("Carturesti", 25, 150.6);
	struct ListaDubla lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	lista = inserareInceputLdi(lista, l);
	lista = inserareInceputLdi(lista, l1);
}


