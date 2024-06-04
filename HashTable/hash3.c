#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct DosarCandidat DosarCandidat;
typedef struct HashTable HashTable;
typedef struct Nod Nod;

struct DosarCandidat {
	char* numeCandidat;
	char* programStudiu;
	float medieBac;
	int codDosar;
};

struct HashTable {
	Nod** lista;
	int dim;

};

struct Nod {
	DosarCandidat info;
	Nod* next;
};

int functieHash(const char* beneficiar, int dimTotala) {
	int suma = 0;
	for (int i = 0; i < strlen(beneficiar); i++) {
		suma += beneficiar[i];
	}
	return suma % dimTotala;
}

//Varianta cu handle de coliziuni
//void handleColiziuni(Nod** lista, DosarCandidat d) {
//	Nod* nou = (Nod*)malloc(sizeof(Nod));
//	nou->info = d;
//	nou->next = NULL;
//
//	if (*lista == NULL) {
//		*lista = nou;
//	}
//	else {
//		Nod* current = *lista;
//		while (current->next != NULL) {
//			current = current->next;
//		}
//		current->next = nou;
//	}
//}
//
//void inserareCuColiziuni(HashTable* tabela, DosarCandidat d) {
//	int index = functieHash(d.numeCandidat, tabela->dim);
//	handleColiziuni(&tabela->lista[index], d);
//}

void inserareInHashTable(HashTable* tabela, DosarCandidat d) {
	int index = functieHash(d.numeCandidat, tabela->dim);
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = d;
	nou->next = tabela->lista[index];
	tabela->lista[index] = nou;
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

DosarCandidat citireDosar(FILE* f) {
	DosarCandidat d;
	char buffer[256];

	fscanf(f, "%s", buffer);
	d.numeCandidat = (char*)malloc(sizeof(char) * strlen(buffer)+1);
	strcpy(d.numeCandidat, buffer);

	fscanf(f, "%s", buffer);
	d.programStudiu = (char*)malloc(sizeof(char) * strlen(buffer)+1);
	strcpy(d.programStudiu, buffer);

	fscanf(f, "%f", &d.medieBac);
	fscanf(f, "%d", &d.codDosar);

	return d;
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
				DosarCandidat d = citireDosar(f);
				inserareInHashTable(hashTable, d);
			}
			fclose(f);
		}
	}

	return hashTable;
}

void afisareDosar(DosarCandidat d) {
	printf("Candidatul cu codul %d, numele %s, program de studiu %s si media la bac %.2f\n",
		d.codDosar, d.numeCandidat, d.programStudiu, d.medieBac);
}

void afisareHashTable(HashTable* tabela) {
	if (tabela != NULL) {
		for (int i = 0; i < tabela->dim; i++) {
			Nod* curent = tabela->lista[i];
			printf("Index %d:\n", i);
			while (curent != NULL) {
				afisareDosar(curent->info);
				curent = curent->next;
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
				free(temp->info.numeCandidat);
				free(temp->info.programStudiu);
				free(temp);
			}

		}
		free(tabela->lista);
		free(tabela);
	}
}


int nrCandidatiPtProgramStudiu(HashTable* tabela, const char* program) {
	int nrCandidati = 0;
	if (tabela != NULL) {
		for (int i = 0; i < tabela->dim; i++) {
			Nod* current = tabela->lista[i];
			while (current != NULL) {
				if (strcmp(current->info.programStudiu, program) == 0) {
					nrCandidati++;
				}
				current = current->next;
			}
		}
		return nrCandidati;
	}
	else {
		return nrCandidati;
	}
}

void stergeCandidatiCuMediaMica(HashTable* tabela, float prag) {
	if (tabela != NULL) {
		for (int i = 0; i < tabela->dim; i++) {
			Nod* current = tabela->lista[i];
			Nod* prev = NULL;
			while (current != NULL) {
				if (current->info.medieBac < prag) {
					if (prev == NULL) {
						tabela->lista[i] = current->next;
					}
					else {
						prev->next = current->next;
					}
					Nod* temp = current;
					current = current->next;
					free(temp->info.numeCandidat);
					free(temp->info.programStudiu);
					free(temp);
				}
				else {
					prev = current;
					current = current->next;
				}
			}
		}
	}
}

void main() {
	HashTable* tabela = citireHashTable("dosar.txt");
	afisareHashTable(tabela);

	const char* program = "CSIE";
	int nrCandidatiPtProgram = nrCandidatiPtProgramStudiu(tabela, program);
	printf("Nr total de candidati la CSIE: %d\n", nrCandidatiPtProgram);

	int prag = 9.00;
	stergeCandidatiCuMediaMica(tabela, prag);
	printf("Lista dupa sterge dupa media mica: \n");
	afisareHashTable(tabela);

	dezalocareHashTable(tabela);
	program = NULL;
}
