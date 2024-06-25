#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Rezervare Rezervare;
typedef struct Hashtable Hashtable;
typedef struct Nod Nod;

struct Rezervare {
	unsigned int id;
	char* numeHotel;
	unsigned char categorieHotel;
	char* numeClient;
	unsigned char zileRezervate;
	float suma;
};

struct Nod {
	Rezervare info;
	Nod* next;
};

struct Hashtable {
	int dim;
	Nod** lista;
};

void afisareRezervare(Rezervare r) {
	printf("Rezervarea cu id-ul %u, numele hotelului %s, categoria %hhu, numele clientului %s, cu nr de zile rezervate %hhu cu suma de %.2f\n",
		r.id, r.numeHotel, r.categorieHotel, r.numeClient, r.zileRezervate,r.suma);
}

int functieHash(const char* numeHotel, int dimTotala) {
	int suma = 0;
	for (int i = 0; i < strlen(numeHotel); i++) {
		suma += numeHotel[i];
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

void inserareHashtable(Hashtable* tabela, Rezervare r) {
	int index = functieHash(r.numeHotel, tabela->dim);
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = r;
	nou->next = tabela->lista[index];
	tabela->lista[index] = nou;
}

void afisareHashtable(Hashtable* tabela) {
	if (tabela) {
		for (int i = 0; i < tabela->dim; i++) {
			Nod* nod = tabela->lista[i];
			while (nod) {
				afisareRezervare(nod->info);
				nod = nod->next;
			}
		}
	}
}

void dezalocareHashtable(Hashtable* tabela) {
	if (tabela) {
		for (int i = 0; i < tabela->dim; i++) {
			Nod* nod = tabela->lista[i];
			while (nod) {
				Nod* temp = nod;
				nod = nod->next;
				free(temp->info.numeClient);
				free(temp->info.numeHotel);
				free(temp);
			}
		}
		free(tabela->lista);
		free(tabela);
	}
}

Rezervare citireRezervare(FILE* f) {
	Rezervare r;
	char buffer[256];

	if (fgets(buffer, 256, f) == NULL) exit(1);
	sscanf(buffer, "%u", &r.id);

	if (fgets(buffer, 256, f) == NULL)exit(1);
	buffer[strcspn(buffer, "\n")] = 0;
	r.numeHotel = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
	strcpy(r.numeHotel, buffer);

	if (fgets(buffer, 256, f) == NULL)exit(1);
	sscanf(buffer, "%hhu", &r.categorieHotel);

	if (fgets(buffer, 256, f) == NULL) exit(1);
	buffer[strcspn(buffer, "\n")] = 0;
	r.numeClient = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
	strcpy(r.numeClient, buffer);

	if (fgets(buffer, 256, f) == NULL) exit(1);
	sscanf(buffer, "%hhu", &r.zileRezervate);

	if (fgets(buffer, 256, f) == NULL) exit(1);
	sscanf(buffer, "%f", &r.suma);

	return r;
}

Hashtable* citesteTabela(const char* file) {
	Hashtable* tabela = NULL;
	//tabela->dim = 0;
	//tabela->lista = NULL;
	if (file && strlen(file) != 0) {
		FILE* f = fopen(file, "r");
		if (f != NULL) {
			int dim = 0;
			fscanf(f, "%d\n", &dim);
			tabela = (Hashtable*)malloc(sizeof(Hashtable));
			*tabela = initHashtable(dim);
			for (int i = 0; i < dim; i++) {
				Rezervare r = citireRezervare(f);
				inserareHashtable(tabela, r);
			}
			fclose(f);
		}
	}
	return tabela;
}

int nrRezervariPeCategorie(Hashtable* tabela, unsigned char categorieCautata) {
	if (!tabela) return 0;
	int counter = 0;
	for (int i = 0; i < tabela->dim; i++) {
		Nod* nod = tabela->lista[i];
		while (nod) {
			if (nod->info.categorieHotel == categorieCautata) {
				counter++;
			}
			nod = nod->next;
		}
	}
	return counter;
}

void modificaCategorie(Hashtable* tabela, const char* denumireHotel, unsigned char* categorieNoua) {
	if (tabela) {
		for (int i = 0; i < tabela->dim; i++) {
			Nod* nod = tabela->lista[i];
			while (nod) {
				if (strcmp(nod->info.numeHotel, denumireHotel) == 0) {
					nod->info.categorieHotel = categorieNoua;
				}
				nod = nod->next;
			}
		}
	}
}

void achizitieHotelDeAltHotel(Hashtable* tabela, const char* hotelCumparator, const char* hotelAchizitionat) {
	if (tabela) {
		for (int i = 0; i < tabela->dim; i++) {
			Nod* nod = tabela->lista[i];
			while (nod) {
				if (strcmp(nod->info.numeHotel, hotelAchizitionat) == 0) {
					free(nod->info.numeHotel);
					nod->info.numeHotel = (char*)malloc(strlen(hotelCumparator) + 1);
					strcpy(nod->info.numeHotel, hotelCumparator);
				}
				nod = nod->next;
			}
		}
	}
}

Hashtable* salveazaInHashTable(Hashtable* tabelaOriginala, unsigned char categorie) {
	Hashtable* tabelaNoua = (Hashtable*)malloc(sizeof(Hashtable));
	*tabelaNoua = initHashtable(tabelaOriginala->dim);

	for (int i = 0; i < tabelaOriginala->dim; i++) {
		Nod* nod = tabelaOriginala->lista[i];
		while (nod) {
			if (nod->info.categorieHotel == categorie) {
				Rezervare newRezervare;
				newRezervare.id = nod->info.id;
				newRezervare.numeHotel = (char*)malloc(strlen(nod->info.numeHotel) + 1);
				strcpy(newRezervare.numeHotel, nod->info.numeHotel);
				newRezervare.categorieHotel = nod->info.categorieHotel;
				newRezervare.numeClient = (char*)malloc(strlen(nod->info.numeClient) + 1);
				strcpy(newRezervare.numeClient, nod->info.numeClient);
				newRezervare.zileRezervate = nod->info.zileRezervate;
				newRezervare.suma = nod->info.suma;

				inserareHashtable(tabelaNoua, newRezervare);
			}
			nod = nod->next;
		}
	}
	return tabelaNoua;
}

typedef struct NodLista NodLista;

struct NodLista {
	Rezervare info;
	NodLista* next;
};

void inserareNodInLista(NodLista** cap, Rezervare r) {
	NodLista* nod = (NodLista*)malloc(sizeof(NodLista));
	nod->info.id = r.id;
	nod->info.numeHotel = (char*)malloc(strlen(r.numeHotel) + 1);
	strcpy(nod->info.numeHotel, r.numeHotel);
	nod->info.categorieHotel = r.categorieHotel;
	nod->info.numeClient = (char*)malloc(strlen(r.numeClient) + 1);
	strcpy(nod->info.numeClient, r.numeClient);
	nod->info.zileRezervate = r.zileRezervate;
	nod->info.suma = r.suma;

	nod->next = *cap;
	*cap = nod;
}

void afisareLista(NodLista* cap) {
	NodLista* nod = cap;
	while (nod) {
		afisareRezervare(nod->info);
		nod = nod->next;
	}
}

void dezalocareLista(NodLista* cap) {
	NodLista* nod = cap;
	while (nod) {
		Nod* temp = nod;
		nod = nod->next;
		free(temp->info.numeHotel);
		free(temp->info.numeClient);
		free(temp);
	}
}

NodLista* salvareInListaDinHashtable(Hashtable* tabela) {
	NodLista* lista = NULL;
	for (int i = 0; i < tabela->dim; i++) {
		Nod* nod = tabela->lista[i];
		while (nod) {
			if (nod->info.suma > 300.5) {
				inserareNodInLista(&lista, nod->info);
			}
			nod = nod->next;
		}
	}
	return lista;
}

void main() {
	Hashtable* tabela = citesteTabela("rezervari.txt");
	afisareHashtable(tabela);

	unsigned char categorie = 4;
	int nrRezervari = nrRezervariPeCategorie(tabela, categorie);
	printf("\n\nNr de de rezevari din categoria %hhu sunt: %d", categorie, nrRezervari);

	const char* denumireHotel = "Ramada";
	unsigned char categorieNoua = 10;
	modificaCategorie(tabela, denumireHotel, categorieNoua);
	printf("\n\nTabela dupa modificarea categoriei hotelului %s in categoria %hhu este: \n", denumireHotel, categorieNoua);
	afisareHashtable(tabela);

	const char* hotelCumparator = "Ramada";
	const char* hotelAchizitionat = "Victoria";
	printf("\n\nTabela dupa achizionarea hotelului %s de catre hotelul %s: \n", hotelAchizitionat, hotelCumparator);
	achizitieHotelDeAltHotel(tabela, hotelCumparator, hotelAchizitionat);
	afisareHashtable(tabela);

	Hashtable* tabelaNou= salveazaInHashTable(tabela, categorie);
	printf("\n\nTabela noua:\n");
	afisareHashtable(tabelaNou);

	NodLista* listaSimpla = salvareInListaDinHashtable(tabela);
	printf("\n\nLista simpla inlantuita salvata DIN hashtable este:\n");
	afisareLista(listaSimpla);

	dezalocareHashtable(tabela);
	dezalocareHashtable(tabelaNou);
	dezalocareLista(listaSimpla);
	tabela = NULL;
	denumireHotel = NULL;
	hotelCumparator = NULL;
	hotelAchizitionat = NULL;
}