#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;
typedef struct Reteta Reteta;


struct Reteta {
	unsigned int id;
	char* numePacient;
	char* numeMedic;
	char* statut;
	unsigned char nrMedicamente;
	float compensare;
};

struct Nod {
	Reteta info;
	Nod* next;
	Nod* prev;
};

struct ListaDubla {
	Nod* cap;
	Nod* coada;
};


void afisareReteta(Reteta c) {
	printf("Reteta cu id %u, al pacientului %s, medicul %s, cu statusul %s, nr medicamente %hhu si compensatia %.2f\n",
		c.id, c.numePacient, c.numeMedic, c.statut, c.nrMedicamente, c.compensare);
}

void inserareNod(ListaDubla* lista, Reteta r) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = r;
	nod->next = NULL;
	nod->prev = lista->coada;
	if (lista->coada != NULL) {
		lista->coada->next = nod;
	}
	else {
		lista->cap = nod;
	}
	lista->coada = nod;
}

Reteta citireReteta(FILE* f) {
	Reteta r;
	char buffer[256];

	if (fgets(buffer, 256, f) == NULL) exit(1);
	sscanf(buffer, "%u", &r.id);

	if (fgets(buffer, 256, f) == NULL) exit(1);
	buffer[strcspn(buffer, "\n")] = 0;
	r.numePacient = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
	strcpy(r.numePacient, buffer);

	if (fgets(buffer, 256, f) == NULL) exit(1);
	buffer[strcspn(buffer, "\n")] = 0;
	r.numeMedic = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
	strcpy(r.numeMedic, buffer);

	if (fgets(buffer, 256, f) == NULL) exit(1);
	buffer[strcspn(buffer, "\n")] = 0;
	r.statut = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
	strcpy(r.statut, buffer);

	if (fgets(buffer, 245, f) == NULL) exit(1);
	sscanf(buffer, "%hhu", &r.nrMedicamente);

	if (fgets(buffer, 256, f) == NULL) exit(1);
	sscanf(buffer, "%f", &r.compensare);

	return r;
}

ListaDubla* citireLista(const char* file) {
	ListaDubla* lista = (ListaDubla*)malloc(sizeof(ListaDubla));
	lista->cap = NULL;
	lista->coada = NULL;
	if (file != NULL && strlen(file) != 0) {
		FILE* f = fopen(file, "r");
		if (f != NULL) {
			int dim = 0;
			fscanf(f, "%d\n", &dim);
			for (int i = 0; i < dim; i++) {
				Reteta r = citireReteta(f);
				inserareNod(lista, r);
			}
			fclose(f);
		}else {
			perror("Error opening file");
			free(lista);
			exit(1);
		}
	}
	return lista;
}

void afisareLista(ListaDubla* lista) {
	Nod* nod = lista->cap;
	while (nod) {
		afisareReteta(nod->info);
		nod = nod->next;
	}
}

void eliberareLista(ListaDubla* lista) {
	Nod* current = lista->cap;
	while (current) {
		Nod* temp = current;
		current = current->next;
		free(temp->info.numePacient);
		free(temp->info.numeMedic);
		free(temp->info.statut);
		free(temp);
	}
	lista->cap = NULL;
	lista->coada = NULL;
}

int nrMedicamentePestePrag(ListaDubla* lista, int prag) {
	Nod* nod = lista->cap;
	int nrMedicamente = 0;
	while (nod) {
		if (nod->info.nrMedicamente > prag) {
			nrMedicamente++;
		}
		nod = nod->next;
	}
	return nrMedicamente;
}

void actualizareProcent(ListaDubla* lista, float procent, const char* pacient) {
	Nod* nod = lista->cap;
	while (nod) {
		if (strcmp(nod->info.numePacient, pacient) == 0) {
			nod->info.compensare = procent;
		}
		nod = nod->next;
	}
}

void stergeNoduri(ListaDubla* lista, unsigned char prag) {
	Nod* current = lista->cap;
	while (current) {
		Nod* next = current->next;
		if (current->info.nrMedicamente < prag) {
			if (current->prev) {
				current->prev->next = current->next;
			}
			else {
				lista->cap = current->next;
			}
			if (current->next) {
				current->next->prev = current->prev;
			}
			else {
				lista->coada = current->prev;
			}
			free(current->info.numePacient);
			free(current->info.numeMedic);
			free(current->info.statut);
			free(current);
		}
		current = next;
	}
}


void main() {
	ListaDubla* lista = citireLista("retete.txt");
	afisareLista(lista);
	int prag = 1;
	int nrMedicamente = nrMedicamentePestePrag(lista, prag);
	printf("\n\nNr de medicamente peste pragul de %d este: %d", prag, nrMedicamente);

	const char* nume = "Serban Corodescu";
	float procent = 60.02;
	actualizareProcent(lista, procent, nume);
	printf("\n\nLista cu actualizare compensatie paciet:\n");
	afisareLista(lista);

	unsigned char pragMinim = 2;
	stergeNoduri(lista, pragMinim);
	printf("\n\nLista cu pacienti peste %hhu medicamente: \n", pragMinim);
	afisareLista(lista);

	eliberareLista(lista);
	free(lista);
}