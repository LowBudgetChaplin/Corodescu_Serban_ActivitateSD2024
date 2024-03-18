#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include <stdlib.h>

typedef struct Masina Masina;
struct Masina {
	int id;
	char* producator;
	int nrUsi;
	float pret;
};

typedef struct Nod Nod;

struct Nod {
	Masina info;
	Nod* next;
};

Masina initMasina(int id, const char* producator, int nrUsi, float pret) {
	Masina m;

	m.id = id;
	m.nrUsi = nrUsi;
	m.pret = pret;
	m.producator = (char*)malloc((strlen(producator) + 1) * sizeof(char));
	strcpy(m.producator, producator);
	
	return m;
}

void afisareMasina(Masina m) {
	printf("Masina cu id-ul %d are proudcatorul: %s, cu numarul de usi: %d si pretul: %5.2f\n", m.id, m.producator,m.nrUsi,m.pret);
}

Nod* inserareNod(Nod* cap, Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = cap;
	return nou;
}

void afisareLista(Nod* cap) {
	while (cap != NULL) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void stergeNod(Nod** cap, int index) {
	if (*cap == NULL) {
		return;
	}
	Nod* temp = *cap;
	Nod* prev = NULL;

	if (index == 0) {
		*cap = temp->next;
		free(temp->info.producator);
		free(temp);
		return;
	}

	if (temp == NULL) {
		return;
	}

	for (int i = 0; i < index && *cap != NULL; i++) {
		prev = temp;
		temp = temp->next;
	}

	prev->next = temp->next;
	free(temp->info.producator);
	free(temp);
}



void main() {
	Masina m1;
	Masina m2;
	Masina m3;
	Masina m4;
	m1 = initMasina(1, "Renault", 4, 12500.5);
	m2 = initMasina(2, "Logan", 4, 10000);
	m3 = initMasina(3, "BMW", 4, 20000);
	m4 = initMasina(4, "AUDI", 4, 5000);

	//afisareMasina(m1);

	Nod* cap = NULL;
	Nod* cap2 = NULL;
	cap = inserareNod(cap, m1);
	cap = inserareNod(cap, m2);
	cap2 = inserareNod(cap2, m3);
	cap2= inserareNod(cap2, m4);
	printf("Linked list:\n\n");
	afisareLista(cap);
	afisareLista(cap2);

	stergeNod(&cap, 0);

	afisareLista(cap);


}