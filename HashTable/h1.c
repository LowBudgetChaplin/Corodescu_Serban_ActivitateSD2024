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
	int index = functieHash(r.numeClient, tabela->dim);
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = r;
	nou->next = tabela->lista[index];
	tabela->lista[index] = nou;
}

void afisareHashtable(Hashtable* tabela) {
	if (tabela != NULL) {
		for (int i = 0; i < tabela->dim; i++) {
			Nod* current = tabela->lista[i];
			while (current) {
				afisareRezervare(current->info);
				current = current->next;
			}
		}
	}
}

void dezalocareHashtable(Hashtable* tabela) {
	if (tabela != NULL) {
		for (int i = 0; i < tabela->dim; i++) {
			Nod* current = tabela->lista[i];
			while (current) {
				Nod* temp = current;
				current = current->next;
				free(temp->info.numeHotel);
				free(temp->info.numeClient);
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

	dezalocareHashtable(tabela);
	tabela = NULL;
	denumireHotel = NULL;
	hotelCumparator = NULL;
	hotelAchizitionat = NULL;
}