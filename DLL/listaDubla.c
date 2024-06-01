#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Proiect Proiect;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;
typedef struct Heap Heap;

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
	Nod* prev;
};

struct ListaDubla {
	Nod* cap;
	Nod* coada;
};

struct Heap {
	Proiect* array;
	int size;
	int capacity;
};


void afisareProiect(Proiect p) {
	printf("Proiectul cu codul %u, titlul %s, cu beneficiarul %s, cu numar de executati %hhu si bugetul %.2f\n",
		p.codProiect, p.titluProiect, p.beneficiar, p.nrExecutanti, p.buget);
}

void inserareNodInceput(ListaDubla* listaDubla, Proiect p) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = p;
	nou->next = listaDubla->cap;
	nou->prev = NULL;
	if (listaDubla->cap != NULL) {
		listaDubla->cap->prev = nou;
	}
	else {
		listaDubla->coada = nou;
	}
	listaDubla->cap = nou;
}

Proiect citireProiect(FILE* f) {
	Proiect p;
	char buffer[100];

		fscanf(f, "%u", &p.codProiect);

		fscanf(f, "%s", buffer);
		p.titluProiect = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
		strcpy(p.titluProiect, buffer);

		fscanf(f, "%s", buffer);
		p.beneficiar = (char*)malloc(sizeof(char) * strlen(buffer)+1);
		strcpy(p.beneficiar, buffer);

		fscanf(f, "%hhu", &p.nrExecutanti);
		fscanf(f, "%f", &p.buget);


	return p;
}

ListaDubla* citireLista(const char* fisier){
	ListaDubla* lista = (ListaDubla*)malloc(sizeof(ListaDubla));
	lista->cap = NULL;
	lista->coada = NULL;
	if (fisier != NULL && strlen(fisier) != 0) {
		FILE* f = fopen(fisier, "r");
		if (f != NULL) {
			int dim = 0;
			fscanf(f, "%d", &dim);
			for (int i = 0; i < dim; i++) {
				Proiect p;
				p = citireProiect(f);
				inserareNodInceput(lista, p);
			}
			fclose(f);
		}	
	}
	else {
		printf("Fisierul nu exista!");
		return lista;
	}

	return lista;
}

void afisareListaDublaInainte(ListaDubla* listaDubla) {
	Nod* nod = listaDubla->cap;
	while (nod) {
		afisareProiect(nod->info);
		nod = nod->next;
	}
}

void afisareListaDublaInvers(ListaDubla* listaDubla) {
	Nod* nod = listaDubla->coada;
	while (nod) {
		afisareProiect(nod->info);
		nod = nod->prev;
	}
}

void eliberareMemorie(ListaDubla* lista) {
	Nod* nod = lista->cap;
	while (nod) {
		Nod* temp = nod;
		nod = nod->next;
		free(temp->info.beneficiar);
		free(temp->info.titluProiect);
		free(temp);
	}
	free(lista);
}

int determinareNumarProiecte(ListaDubla* lista ,unsigned char prag) {
	Nod* nod = lista->cap;
	int counter = 0;
	while (nod->next != NULL) {
		if (nod->info.nrExecutanti > prag) {
			counter++;
		}
		nod = nod->next;
	}
	return counter;
}

void actualizareBuget(ListaDubla* lista, const char* beneficiar, float procent) {
	Nod* nod = lista->cap;
	while (nod) {
		if(strcmp(nod->info.beneficiar, beneficiar) == 0){
			nod->info.buget += nod->info.buget * procent/100;
		}
		nod = nod->next;
	}
}

void sortareNoduri(ListaDubla* lista) {
	if (lista->cap == NULL) return;

	int swapped;
	Nod* current;


	do {
		swapped = 0;
		current = lista->cap;
		while (current->next) {
			if (current->info.buget > current->info.buget) {
				Proiect temp = current->info;
				current->info = current->next->info;
				current->next->info = temp;
				swapped = 1;
			}
			current = current->next;
		}
		current = NULL;
	} while (swapped);
}

Heap* creareHeap(int capacity) {
	Heap* heap = (Heap*)malloc(sizeof(Heap));
	heap->capacity = capacity;
	heap->size = 0;
	heap->array = (Proiect*)malloc(capacity * sizeof(Proiect));
	return heap;
}

void afisareHeap(Heap* heap) {
	for (int i = 0; i < heap->size; i++) {
		afisareProiect(heap->array[i]);
	}
}

void inserareInHeap(Heap* heap, Proiect p) {
	if (heap->size == heap->capacity) {
		heap->capacity *= 2;
		heap->array = (Proiect*)realloc(heap->array, heap->capacity * sizeof(Proiect));
	}

	heap->array[heap->size] = p;
	int current = heap->size;
	heap->size++;

	//MAX HEAP - parintele este mereu mai > decat copiii sai
	while (current != 0 && heap->array[(current - 1) / 2].codProiect < heap->array[current].codProiect) {
		Proiect temp = heap->array[current];
		heap->array[current] = heap->array[(current - 1)/2];
		heap->array[(current-1)/2] = temp;
		current = (current - 1) / 2;
	}
}

void salvareInHeap(ListaDubla* listaDubla, Heap* heap, const char* benficiar) {
	Nod* nod = listaDubla->cap;
	while (nod) {

		if (strcmp(nod->info.beneficiar, benficiar) == 0) {
			inserareInHeap(heap, nod->info);
		}
		nod = nod->next;
	}
}

void eliberareHeap(Heap* heap) {
	free(heap->array);
	free(heap);
}

void main() {
	ListaDubla* listaDubla;
	listaDubla = citireLista("proiecte.txt");
	afisareListaDublaInainte(listaDubla);
	printf("\n");
	//afisareListaDublaInvers(listaDubla);

	unsigned char prag = 1;
	int nrProiecte =determinareNumarProiecte(listaDubla, prag);
	printf("\n\nNr total de proiecte: %d", nrProiecte);

	char* beneficiar2 = "Serban";
	char* beneficiar = "Pavel";
	float procent = 20.0;
	printf("\n\nBuget actualizat: \n");
	actualizareBuget(listaDubla, beneficiar, procent);
	afisareListaDublaInainte(listaDubla);
	printf("\n\n");
	afisareListaDublaInvers(listaDubla);
	printf("\n\n");

	sortareNoduri(listaDubla);
	afisareListaDublaInainte(listaDubla);
	printf("\n\n");
	afisareListaDublaInvers(listaDubla);

	printf("\n");
	Heap* heap = creareHeap(10);
	salvareInHeap(listaDubla, heap, beneficiar);
	salvareInHeap(listaDubla, heap, beneficiar2);
	printf("Proiectele salvate in heap:\n");
	afisareHeap(heap);
	

	eliberareMemorie(listaDubla);
	eliberareHeap(heap);
	beneficiar = NULL;
}