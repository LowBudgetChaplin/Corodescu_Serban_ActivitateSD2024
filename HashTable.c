#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

typedef struct Galerie Galerie;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Galerie {
	int cod;
	char* nume;
	float pretIntrare;
};

struct Nod{
	Galerie info;
	Nod* next;
};

struct HashTable {
	Nod** vector;
	int dim;
};

HashTable initHashTable(int dimesniune) {
	HashTable hash;
	hash.dim = dimesniune;
	hash.vector = (Nod**)malloc(sizeof(Nod*) * dimesniune);
	for (int i = 0; i < dimesniune; i++) {
		hash.vector[i] = NULL;
	}
	return hash;
}

Galerie initGalerie(int cod, const char* nume, float pret) {
	Galerie g;
	g.cod = cod;
	g.nume = (char*)malloc(strlen(nume) + 1);
	strcpy(g.nume, nume);
	g.pretIntrare = pret;
	return g;
}

int calculHash(int cod, const char* nume, int dimensiune) {
	if(dimensiune != 0)
	{
		int rez = cod * strlen(nume);
		return rez % dimensiune;
	}
	return -1;
}

void inserareLaFinal(Nod** cap, Galerie galerie) {
	Nod* temp = (Nod*)malloc(sizeof(Nod));
	temp->info = galerie;
	temp->next = NULL;
	if (*cap) {
		Nod* aux = *cap;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = temp;
	}
	else {
		*cap = temp;
	}
}

void inserareGalerieInTabela(HashTable hash, Galerie galerie) {
	if (hash.dim > 0) {
		int pozitie = calculHash(galerie.cod, galerie.nume, hash.dim);
		if (hash.vector[pozitie] != NULL) { //acest if este aici doar pt a prezenta situatia de coliziune
			inserareLaFinal(&(hash.vector[pozitie]), galerie);
		}
		else {
			inserareLaFinal(&(hash.vector[pozitie]), galerie);
		}
	}
}

void afisareGalerie(Galerie galerie) {
	printf("%d. Intrarea la galeria %s costa %.2f\n", galerie.cod, galerie.nume, galerie.pretIntrare);
}

void afisareListaGalerii(Nod* cap) {
	while (cap) {
		afisareGalerie(cap->info);
		cap = cap->next;
	}
}

void afisareTabela(HashTable tabela) {
	for (int i = 0; i < tabela.dim; i++) {
		if (tabela.vector[i] != NULL) {
			printf("Clusterul: %d\n", (i+1));
			afisareListaGalerii(tabela.vector[i]);
		}
	}
}


Galerie cautaGalerieDupaCheie(int cod, const char* nume, HashTable tabela) {
	int pozitie = calculHash(cod, nume, tabela.dim);
	if (pozitie >= 0 && pozitie < tabela.dim) {
		Nod* p = tabela.vector[pozitie];
		while (p && p->info.cod != cod && strcmp(p->info.nume) != 0) {
			p = p->next;
		}
		if (p) {
			return p->info; //shallow copy
		}
		else {
			return initGalerie(-1, "", 0);
		}
	}
	else {
		return initGalerie(-1, "", 0);
	}
}

void main() {
	Galerie galerie1 = initGalerie(1,"Antipa",12.5);
	Galerie galerie2 = initGalerie(2,"Orizont",10);
	Galerie galerie3 = initGalerie(3,"Louvre",20);
	Galerie galerie4 = initGalerie(4,"Cercul Armatei",15);
	Galerie galerie5 = initGalerie(5,"Geologie",17);
	Galerie galerie6 = initGalerie(6,"Istorie",30);
	HashTable tabela = initHashTable(5);

	inserareGalerieInTabela(tabela, galerie1);
	inserareGalerieInTabela(tabela, galerie2);
	inserareGalerieInTabela(tabela, galerie3);
	inserareGalerieInTabela(tabela, galerie4);
	inserareGalerieInTabela(tabela, galerie5);
	inserareGalerieInTabela(tabela, galerie6);

	afisareTabela(tabela);
	Galerie g = cautaGalerieDupaCheie(3, "Louvre", tabela);
	printf("Cautare: \n");
	afisareGalerie(g);
}