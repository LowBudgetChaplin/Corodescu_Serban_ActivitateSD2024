#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct CursaAeriana CursaAeriana;
typedef struct Hashtable Hashtable;
typedef struct Nod Nod;

struct CursaAeriana {
	unsigned short int id;
	char* desinatie;
	char* companie;
	unsigned short int distanta;
	unsigned short int nrPasageri;
};

struct Nod {
	CursaAeriana info;
	Nod* next;
};

struct Hashtable {
	int dim;
	Nod** lista;
};

void afisareRezervare(CursaAeriana c) {
	printf("Cursa cu id-ul %hu, de la cu destinatia %s, compania aeriana %s, pe distanta de %hu si nr de pasageri %hu\n",
		c.id, c.desinatie, c.companie, c.distanta, c.nrPasageri);
}

int functieHash(const char* destinatie, int dimTotala) {
	int suma = 0;
	for (int i = 0; i < strlen(destinatie); i++) {
		suma += destinatie[i];
	}
	return suma % dimTotala;
}

Hashtable initHashtable(int dim) {
	Hashtable tabela;
	tabela.dim = dim;
	tabela.lista = (Nod**)malloc(sizeof(Nod*) * dim);
	for (int i = 0; i < dim; i++) {
		tabela.lista[i] = NULL;
	}
	return tabela;
}

void inserareHashtable(Hashtable* tabela, CursaAeriana c) {
	int index = functieHash(c.desinatie, tabela->dim);
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = c;
	nou->next = tabela->lista[index];
	tabela->lista[index] = nou;
}

void afisareHashtable(Hashtable* tabela) {
	if (tabela) {
		for (int i = 0; i < tabela->dim; i++) {
			Nod* nod = tabela->lista[i];
			while (nod) {
				afisareRezervare(nod->info);
				nod=nod->next;
			}
		}
	}
}

void stergeHashTable(Hashtable* tabela) {
	if (tabela) {
		for (int i = 0; i < tabela->dim; i++) {
			Nod* nod = tabela->lista[i];
			while (nod) {
				Nod* temp = nod;
				nod = nod->next;
				free(temp->info.companie);
				free(temp->info.desinatie);
				free(temp);
			}
		}
		free(tabela->lista);
		free(tabela);
	}
}

CursaAeriana citireCursa(FILE* f) {
	CursaAeriana c;
	char buffer[256];

	if (fgets(buffer, 256, f) == NULL) exit(1);
	sscanf(buffer, "%hu", &c.id);

	if (fgets(buffer, 256, f) == NULL)exit(1);
	buffer[strcspn(buffer, "\n")] = 0;
	c.desinatie = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
	strcpy(c.desinatie, buffer);

	if (fgets(buffer, 256, f) == NULL)exit(1);
	buffer[strcspn(buffer, "\n")] = 0;
	c.companie = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
	strcpy(c.companie, buffer);

	if (fgets(buffer, 256, f) == NULL) exit(1);
	sscanf(buffer, "%hu", &c.distanta);

	if (fgets(buffer, 256, f) == NULL) exit(1);
	sscanf(buffer, "%hu", &c.nrPasageri);

	return c;
}

Hashtable* citireHashtable(const char* file) {
	Hashtable* tabela = NULL;
	if (file && strlen(file) != 0) {
		FILE* f = fopen(file, "r");
		if (f != NULL) {
			int dim = 0;
			fscanf(f, "%d\n", &dim);
			tabela = (Hashtable*)malloc(sizeof(Hashtable));
			*tabela = initHashtable(dim);
			for (int i = 0; i < dim; i++) {
				CursaAeriana c = citireCursa(f);
				inserareHashtable(tabela, c);
			}
			fclose(f);
		}
	}
	return tabela;
}

int detNrCurseSpecifice(Hashtable* tabela, const char* destinatie) {
	if (tabela) {
		int counter = 0;
		for (int i = 0; i < tabela->dim; i++) {
			Nod* nod = tabela->lista[i];
			while (nod) {
				if (strcmp(nod->info.desinatie, destinatie) == 0) {
					counter++;
				}
				nod = nod->next;
			}
		}
		return counter;
	}
}

void schimbaDenumireCompanie(Hashtable* tabela, const char* denumireVeche,const char* nouaDenumire) {
	if (tabela) {
		for (int i = 0; i < tabela->dim; i++) {
			Nod* nod = tabela->lista[i];
			while (nod) {
				if (strcmp(nod->info.companie, denumireVeche) == 0) {
					free(nod->info.companie);
					nod->info.companie = (char*)malloc(strlen(nouaDenumire) + 1);
					strcpy(nod->info.companie, nouaDenumire);
				}
				nod = nod->next;
			}
		}
	}
}

void stergeCurseDupaDestinatie(Hashtable* tabela, const char* destinatie) {
	if (tabela) {
		for (int i = 0; i < tabela->dim; i++) {
			Nod* current = tabela->lista[i];
			Nod* prev = NULL;
			while (current) {
				if (strcmp(current->info.desinatie, destinatie) == 0) {
					if (prev) {
						prev->next = current->next;
					}
					else {
						tabela->lista[i] = current->next;
					}
					Nod* temp = current;
					current = current->next;
					free(temp->info.companie);
					free(temp->info.desinatie);
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
	Hashtable* tabela = citireHashtable("curse.txt");
	afisareHashtable(tabela);
	const char* destinatie = "Madrid";
	int nrCurseCautate = detNrCurseSpecifice(tabela, destinatie);
	printf("\nNr de curse cu destinatia %s este de %d", destinatie, nrCurseCautate);

	const char* vecheaDenumire = "WizzAir";
	const char* nouaDenumire = "Mihai zboara";
	printf("\n\nTabela dupa modificarea numele companiei %s in %s este:\n", vecheaDenumire, nouaDenumire);
	schimbaDenumireCompanie(tabela,vecheaDenumire,nouaDenumire);
	afisareHashtable(tabela);

	printf("\n\nTabela dupa stergerea curselor cu destinatia %s:\n", destinatie);
	stergeCurseDupaDestinatie(tabela, destinatie);
	afisareHashtable(tabela);

	stergeHashTable(tabela);
	destinatie = NULL;
	vecheaDenumire = NULL;
	nouaDenumire = NULL;
}
