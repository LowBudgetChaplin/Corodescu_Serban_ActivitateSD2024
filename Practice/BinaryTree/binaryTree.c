#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Rezervare Rezervare;
typedef struct Nod Nod;

struct Rezervare {
    unsigned int id;
    char* denumireHotel;
    unsigned char camere;
    char* numeClient;
    float suma;
};

struct Nod {
    Rezervare info;
    Nod* st;
    Nod* dr;
};

void afisareRezervare(Rezervare r) {
    printf("\nCamera cu id %d, nume %s, numar camere %hhu, nume client %s, cu suma de %.2f",
        r.id, r.denumireHotel, r.camere, r.numeClient, r.suma);
}


void inserare(Nod** radacina, Rezervare r) {
    if (*radacina) {
        if ((*radacina)->info.id > r.id) {
            inserare(&((*radacina)->st), r);
        }
        else {
            inserare(&((*radacina)->dr), r);
        }
    }
    else {
        Nod* nod = malloc(sizeof(Nod));
        nod->dr = NULL;
        nod->st = NULL;
        nod->info = r;
        *radacina = nod;
    }
}

Rezervare citireRezervare(FILE* f) {
    Rezervare r;
    char buffer[256];
    
    fscanf(f, "%u", &r.id);


    fscanf(f, "%s", buffer);
    r.denumireHotel = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(r.denumireHotel, buffer);

    fscanf(f, "%hhu", &r.camere);

    fscanf(f, "%s", buffer);
    r.numeClient = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(r.numeClient, buffer);

    fscanf(f, "%f", &r.suma);
    return r;
}

Nod* citireFisier(const char* fisier) {
    Nod* radacina = NULL;
    if (fisier != NULL && strlen(fisier) != 0) {
        FILE* f = fopen(fisier, "r");
        if(f != NULL)
        {
            int dim = 0;
            fscanf(f, "%d", &dim);
            for (int i = 0; i < dim; i++) {
                Rezervare r;
                r = citireRezervare(f);
                inserare(&radacina, r);
            }
            fclose(f);
        }
      else {
            printf("Nu se poate deschide fisierul\n");
        }
    }
  
    return radacina;
}

void dezalocare(Nod* rad) {
    if (rad) {
        dezalocare(rad->st);
        dezalocare(rad->dr);
        free(rad->info.denumireHotel);
        free(rad->info.numeClient);
        free(rad);
    }
}


void afisareInOrdine(Nod* rad) {
    if (rad) {
        afisareInOrdine(rad->st);
        afisareRezervare(rad->info);
        afisareInOrdine(rad->dr);
    }
}

int calculNrTotaleCamere(Nod* radacina) {
    if (radacina == NULL) {
        return 0;
    }
    
    return radacina->info.camere + calculNrTotaleCamere(radacina->st) +
        calculNrTotaleCamere(radacina->dr);
}

int inaltimeArbore(Nod* rad) {
    if (rad == NULL) {
        return 0;
    }

    int inaltimeStanga = inaltimeArbore(rad->st);
    int inaltimeDreapta = inaltimeArbore(rad->dr);
    return (inaltimeStanga > inaltimeDreapta) ? inaltimeStanga + 1 : inaltimeDreapta + 1;
}

Nod* nodValoareMinima(Nod* rad) {
    Nod* current = rad;
    while (current && current->st != NULL) {
        current = current->st;
    }
    return current;
}

Nod* stergereRadacina(Nod* rad) {
    if (rad == NULL) {
        return rad;
    }

    if (rad->st == NULL) {
        Nod* temp = rad->dr;
        free(rad->info.denumireHotel);
        free(rad->info.numeClient);
        free(rad);
        return temp;
    }

    if (rad->dr == NULL) {
        Nod* temp = rad->st;
        free(rad->info.denumireHotel);
        free(rad->info.numeClient);
        free(rad);
        return temp;
    }

    Nod* temp = nodValoareMinima(rad->dr);
    rad->info = temp->info;
    rad->dr = stergereRadacina(rad->dr);

    return rad;
    
}

int* salveazaInVector(Nod* rad, unsigned int id, int* dim) {
    *dim = 0;
    int* drum = (int*)malloc(sizeof(int) * 100); //pp ca are adancime de 100
    Nod* current = rad;

    while (current != NULL) {
        drum[(*dim)++] = current->info.id;
        if (current->info.id == id) {
            return drum;
        }
        else if(current->info.id > id){
            current = current->st;
        }
        else {
            current = current->dr;
        }
    }
    free(drum);
    *dim = 0;
    return NULL;

}

void main() {
    Nod* radacina = NULL;
    radacina = citireFisier("rezervari.txt");
    afisareInOrdine(radacina);
    int suma  = calculNrTotaleCamere(radacina);
    printf("\n\nNr total de camere: %d", suma);

    int inaltimeTotala = inaltimeArbore(radacina);
    printf("\n\nInaltime arbore %d", inaltimeTotala);

    radacina = stergereRadacina(radacina);
    printf("\n\nArborele dupa stergerea radicinii");
    afisareInOrdine(radacina);
    printf("\n");

    unsigned int idCautat = 5;
    int dim;
    int* drum = salveazaInVector(radacina, idCautat, &dim);
    if (drum != NULL) {
        printf("\nDrumul de la radacina la nodul cu id-ul cautat %u este: ", idCautat);
        for (int i = 0; i < dim; i++) {
            printf("%d\n", drum[i]);
        }
        printf("\n");
        free(drum);
    }
    else {
        printf("\nNodul cu id %u nu a fost gasit in arbore.\n", idCautat);
    }

    dezalocare(radacina);
    
}