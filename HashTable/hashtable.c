#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Proiect Proiect;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Proiect {
	unsigned int codProiect;
	char* titluProiect;
	char* beneficiar;
	unsigned char nrExecutanti;
	float buget;
};

struct Nod {
	Proiect info;
	Nod* next;
};

struct HashTable {
	int dim;
	Nod** lista;
};

int functieHash(const char* beneficiar, int dimensiuneTotala) {
	int suma = 0;
	for (int i = 0; i < strlen(beneficiar); i++) {
		suma += beneficiar[i];
	}
	return suma % dimensiuneTotala;
}

HashTable initHashTable(int dim) {
	HashTable tabela;
	tabela.dim = dim;
	tabela.lista = (Nod**)malloc(sizeof(Nod*) * dim);
	for (int i = 0; i < dim; i++) {
		tabela.lista[i] = NULL;
	}
	return tabela;
}

void inserareHashTable(HashTable* tabela, Proiect p) {
	int index = functieHash(p.beneficiar, tabela->dim);
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = p;
	nou->next = tabela->lista[index];
	tabela->lista[index] = nou;
}

Proiect citireProiect(FILE* f) {
    Proiect p;
    char buffer[256];
    fscanf(f, "%u", &p.codProiect);
    fscanf(f, "%s", buffer);
    p.titluProiect = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(p.titluProiect, buffer);
    fscanf(f, "%s", buffer);
    p.beneficiar = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(p.beneficiar, buffer);
    fscanf(f, "%hhu", &p.nrExecutanti);
    fscanf(f, "%f", &p.buget);
    return p;
}

HashTable* citireHashTable(const char* file) {
	HashTable* hashTable = NULL;
	if (file != NULL && strlen(file) != 0) {
		FILE* f = fopen(file, "r");
		if (f != NULL) {
			int dim = 0;
			fscanf(f, "%d", &dim);
			hashTable = (HashTable*)malloc(sizeof(HashTable));
			*hashTable = initHashTable(dim);
			for (int i = 0; i < dim; i++) {
				Proiect p = citireProiect(f);
				inserareHashTable(hashTable, p);
			}
			fclose(f);
		}
	}

	return hashTable;
}

void afisareProiect(Proiect p) {
	printf("Proiectul cu codul %u, titlul %s, cu beneficiarul %s, cu numar de executati %hhu si bugetul %.2f\n",
		p.codProiect, p.titluProiect, p.beneficiar, p.nrExecutanti, p.buget);
}

void afisareHashTable(HashTable* tabela) {
	if (tabela != NULL) {
		for (int i = 0; i < tabela->dim; i++) {
			Nod* current = tabela->lista[i];
			while (current != NULL) {
				afisareProiect(current->info);
				current = current->next;
			}
		}
	}
}

void dezalocareHashTable(HashTable* tabela) {
	if (tabela != NULL) {
		for (int i = 0; i < tabela->dim; i++) {
			Nod* current = tabela->lista[i];
			while (current != NULL) {
				Nod* temp = current;
				current = current->next;
				free(temp->info.beneficiar);
				free(temp->info.titluProiect);
				free(temp);
			}
		}
		free(tabela->lista);
		free(tabela);
	}
}

void inserareLIstaColiziuni(Nod** cap, Proiect p) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = p;
	nou->next = NULL;
	if ((*cap) == NULL) {
		(*cap) = nou;
	}
	else {
		Nod* copie = (*cap);
		while (copie->next != NULL) {
			copie = copie->next;
		}
		copie->next = nou;
	}
}

float ruteurnareBuget(HashTable tabela, const char* beneficiar) {
	int poz = functieHash(beneficiar, tabela.dim);
	float bugetTotal = 0;
	Nod* temp = tabela.lista[poz];
	while (temp != NULL) {
		if (strcmp(temp->info.beneficiar, beneficiar) == 0) {
			bugetTotal += temp->info.buget;
		}
		temp=temp->next;
	}
	return bugetTotal;
}

void inserareProiectInHashTable(HashTable hash, Proiect p) {
	int poz = functieHash(p.beneficiar, hash.dim);
	inserareLIstaColiziuni(&hash.lista[poz], p);
}

void main() {
	HashTable* tabela = citireHashTable("proiecte.txt");
	afisareHashTable(tabela);


	dezalocareHashTable(tabela);
}
