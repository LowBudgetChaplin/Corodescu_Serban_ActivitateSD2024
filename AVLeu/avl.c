#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Rezervare Rezervare;
typedef struct Nod Nod;

struct Rezervare {
    unsigned int id;
    char* denumireHotel;
    unsigned char categorieHotel;
    char* numeClient;
    unsigned char zileRezervare;
    float sumaPlatita;
};

struct Nod {
    Rezervare info;
    Nod* st;
    Nod* dr;
};

void afisareRezervare(Rezervare r) {
    printf("Rezervare: ID %u, Hotel %s, Categorie %hhu, Client %s, Zile %hhu, Suma %.2f\n",
        r.id, r.denumireHotel, r.categorieHotel, r.numeClient, r.zileRezervare, r.sumaPlatita);
}

int determinaInaltime(Nod* rad) {
    if (rad) {
        int inaltimeSt = determinaInaltime(rad->st);
        int inaltimeDr = determinaInaltime(rad->dr);
        return 1 + (inaltimeSt > inaltimeDr ? inaltimeSt : inaltimeDr);
    }
    return 0;
}

int gradEchilibru(Nod* rad) {
    if (rad) {
        int inaltimeStanga = determinaInaltime(rad->st);
        int inaltimeDreapta = determinaInaltime(rad->dr);
        return inaltimeStanga - inaltimeDreapta;
    }
    return 0;
}

void rotireStanga(Nod** rad) {
    Nod* aux = (*rad)->dr;
    (*rad)->dr = aux->st;
    aux->st = (*rad);
    *rad = aux;
}


void rotireDreapta(Nod** rad) {
    Nod* aux = (*rad)->st;
    (*rad)->st = aux->dr;
    aux->dr = (*rad);
    *rad = aux;
}

void inserareNodAVL(Nod** rad, Rezervare r) {
    if (*rad) {
        if ((*rad)->info.id > r.id) {
            inserareNodAVL(&((*rad)->st), r);
        }
        else {
            inserareNodAVL(&((*rad)->dr), r);
        }

        int grad = gradEchilibru(*rad);
        if (grad == 2) {
            if (gradEchilibru((*rad)->st) >= 0) {
                rotireDreapta(rad);
            }
            else {
                rotireStanga(&(*rad)->st);
                rotireDreapta(rad);
            }
        }
        if (grad == -2) {
            if (gradEchilibru((*rad)->dr) <= 0) {
                rotireStanga(rad);
            }
            else {
                rotireDreapta(&(*rad)->dr);
                rotireStanga(rad);
            }
        }
    }
    else {
        Nod* nou = (Nod*)malloc(sizeof(Nod));
        nou->st = NULL;
        nou->dr = NULL;
        nou->info = r;
        *rad = nou;
    }
}

void afisareInOrdine(Nod* rad) {
    if (rad) {
        afisareInOrdine(rad->st);
        afisareRezervare(rad->info);
        afisareInOrdine(rad->dr);
    }
}

void dezalocareArbore(Nod* rad) {
    if (rad) {
        dezalocareArbore(rad->st);
        dezalocareArbore(rad->dr);
        free(rad->info.denumireHotel);
        free(rad->info.numeClient);
        free(rad);
    }
}

Rezervare citireProiect(FILE* f) {
    Rezervare r;
    char buffer[256];

    if (fgets(buffer, 256, f) == NULL) exit(1);
    sscanf(buffer, "%u", &r.id);

    if (fgets(buffer, 256, f) == NULL) exit(1);
    buffer[strcspn(buffer, "\n")] = 0;
    r.denumireHotel= (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(r.denumireHotel, buffer);

    if (fgets(buffer, 256, f) == NULL) exit(1);
    sscanf(buffer, "%hhu", &r.categorieHotel);

    if (fgets(buffer, 256, f) == NULL) exit(1);
    buffer[strcspn(buffer, "\n")] = 0;
    r.numeClient = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(r.numeClient, buffer);

    if (fgets(buffer, 256, f) == NULL) exit(1);
    sscanf(buffer, "%hhu", &r.zileRezervare);

    if (fgets(buffer, 256, f) == NULL) exit(1);
    sscanf(buffer, "%f", &r.sumaPlatita);

    return r;
}

Nod* citireArbore(const char* file) {
    Nod* rad = NULL;
    if (file && strlen(file) != 0) {
        FILE* f = fopen(file, "r");
        if (f != NULL) {
            int dim = 0;
            fscanf(f, "%d\n", &dim);
            for (int i = 0; i < dim; i++) {
                Rezervare r = citireProiect(f);
                inserareNodAVL(&rad, r);
            }
            fclose(f);
        }
    }
    return rad;
}

int detNrRezervari(Nod* rad, unsigned char categorie) {
    if (!rad) return 0;
    
    int count = 0;
    if (rad->info.categorieHotel == categorie) {
        count = 1;
    }
    return count + detNrRezervari(rad->st, categorie) + detNrRezervari(rad->dr, categorie);
    
}

void schimbaCategorieHotel(Nod* rad, const char* numeHotel, unsigned char categorieNoua) {
    if (rad) {
        schimbaCategorieHotel(rad->st, numeHotel, categorieNoua);
        if (strcmp(rad->info.denumireHotel, numeHotel) == 0) {
            rad->info.categorieHotel = categorieNoua;
        }
        schimbaCategorieHotel(rad->dr, numeHotel, categorieNoua);
    }
}

void modificaAVLDupaAchizitie(Nod* rad, const char* hotelAchizitionat, const char* hotelCumparator) {
    if (rad) {
        if (strcmp(rad->info.denumireHotel, hotelAchizitionat) == 0) {
            free(rad->info.denumireHotel);
            rad->info.denumireHotel = (char*)malloc(strlen(hotelCumparator)+1);
            strcpy(rad->info.denumireHotel, hotelCumparator);
        }
        modificaAVLDupaAchizitie(rad->st, hotelAchizitionat, hotelCumparator);
        modificaAVLDupaAchizitie(rad->dr, hotelAchizitionat, hotelCumparator);
    }
}

void main() {
    Nod* radacina = citireArbore("rezervari.txt");
    afisareInOrdine(radacina);

    unsigned char categorieCautata = 4;
    int nrRezervariSpecifice = detNrRezervari(radacina, categorieCautata);
    printf("\n\nNr de rezervari pentru categoria %hhu este %d", categorieCautata, nrRezervariSpecifice);


    const char* hotelCautat = "HotelLux";
    unsigned char nouaCategorie = 6;
    schimbaCategorieHotel(radacina, hotelCautat, nouaCategorie);
    printf("\n\nArborele AVL dupa nodificarea categoriei hotelului %s\n", hotelCautat);
    afisareInOrdine(radacina);

    const char* hotelAchizitionat = "HotelLux";
    const char* hotelCumparator = "HotelPremium";
    modificaAVLDupaAchizitie(radacina, hotelAchizitionat, hotelCumparator);
    printf("\n\nArborele AVL dupa achizionarea hotelui %s de catre hotelul %s\n", hotelAchizitionat, hotelCumparator);
    afisareInOrdine(radacina);
    
    dezalocareArbore(radacina);
    hotelAchizitionat = NULL;
    hotelCumparator = NULL;
}