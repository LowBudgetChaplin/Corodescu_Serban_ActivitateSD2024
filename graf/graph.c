#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Traseu {
    unsigned int id;
    char* denumireStart;
    char* denumireStop;
    float distanta;
} Traseu;

typedef struct Nod {
    char* denumire;
    struct Nod* next;
    struct Traseu* trasee;
    int numarTrasee;
} Nod;

typedef struct Graf {
    Nod** noduri;
    int numarNoduri;
} Graf;


Graf* creareGraf(int numarNoduri) {
    Graf* graf = (Graf*)malloc(sizeof(Graf));
    graf->numarNoduri = numarNoduri;
    graf->noduri = (Nod**)malloc(numarNoduri * sizeof(Nod*));
    for (int i = 0; i < numarNoduri; i++) {
        graf->noduri[i] = NULL;
    }
    return graf;
}

Nod* creareNod(char* denumire) {
    Nod* nod = (Nod*)malloc(sizeof(Nod));
    nod->denumire = (char*)malloc(strlen(denumire) + 1);
    strcpy(nod->denumire, denumire);
    nod->next = NULL;
    nod->trasee = NULL;
    nod->numarTrasee = 0;
    return nod;
}

Traseu* creareTraseu(unsigned int id, char* start, char* stop, float distanta) {
    Traseu* traseu = (Traseu*)malloc(sizeof(Traseu));
    traseu->id = id;
    traseu->denumireStart = (char*)malloc(strlen(start) + 1);
    strcpy(traseu->denumireStart, start);
    traseu->denumireStop = (char*)malloc(strlen(stop) + 1);
    strcpy(traseu->denumireStop, stop);
    traseu->distanta = distanta;
    return traseu;
}

void adaugaNod(Graf* graf, char* denumire) {
    Nod* nod = creareNod(denumire);
    for (int i = 0; i < graf->numarNoduri; i++) {
        if (graf->noduri[i] == NULL) {
            graf->noduri[i] = nod;
            return;
        }
    }
}

void adaugaTraseu(Graf* graf, unsigned int id, char* start, char* stop, float distanta) {
    Traseu* traseu = creareTraseu(id, start, stop, distanta);
    for (int i = 0; i < graf->numarNoduri; i++) {
        if (graf->noduri[i] && strcmp(graf->noduri[i]->denumire, start) == 0) {
            graf->noduri[i]->trasee = (Traseu*)realloc(graf->noduri[i]->trasee, (graf->noduri[i]->numarTrasee + 1) * sizeof(Traseu));
            graf->noduri[i]->trasee[graf->noduri[i]->numarTrasee] = *traseu;
            graf->noduri[i]->numarTrasee++;
            free(traseu);
            return;
        }
    }
}

// Funcții de afișare și eliberare
void afisareGraf(Graf* graf) {
    for (int i = 0; i < graf->numarNoduri; i++) {
        if (graf->noduri[i]) {
            printf("Nod: %s\n", graf->noduri[i]->denumire);
            for (int j = 0; j < graf->noduri[i]->numarTrasee; j++) {
                printf("  Traseu catre %s, Distanta: %.2f\n", graf->noduri[i]->trasee[j].denumireStop, graf->noduri[i]->trasee[j].distanta);
            }
        }
    }
}

void eliberareGraf(Graf* graf) {
    for (int i = 0; i < graf->numarNoduri; i++) {
        if (graf->noduri[i]) {
            for (int j = 0; j < graf->noduri[i]->numarTrasee; j++) {
                free(graf->noduri[i]->trasee[j].denumireStart);
                free(graf->noduri[i]->trasee[j].denumireStop);
            }
            free(graf->noduri[i]->trasee);
            free(graf->noduri[i]->denumire);
            free(graf->noduri[i]);
        }
    }
    free(graf->noduri);
    free(graf);
}

// Funcție de citire din fișier
Graf* citireGrafDinFisier(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("Nu s-a putut deschide fișierul");
        exit(1);
    }

    int numarNoduri;
    fscanf(f, "%d\n", &numarNoduri);

    Graf* graf = creareGraf(numarNoduri);

    for (int i = 0; i < numarNoduri; i++) {
        char buffer[256];
        fgets(buffer, 256, f);
        buffer[strcspn(buffer, "\n")] = 0;
        adaugaNod(graf, buffer);
    }

    int numarTrasee;
    fscanf(f, "%d\n", &numarTrasee);

    for (int i = 0; i < numarTrasee; i++) {
        unsigned int id;
        char start[256], stop[256];
        float distanta;
        fscanf(f, "%u\n", &id);
        fgets(start, 256, f);
        start[strcspn(start, "\n")] = 0;
        fgets(stop, 256, f);
        stop[strcspn(stop, "\n")] = 0;
        fscanf(f, "%f\n", &distanta);
        adaugaTraseu(graf, id, start, stop, distanta);
    }

    fclose(f);
    return graf;
}

// Funcția 2: Determină numărul de trasee peste o distanță
int numarTraseePesteDistanta(Graf* graf, float distanta) {
    int count = 0;
    for (int i = 0; i < graf->numarNoduri; i++) {
        if (graf->noduri[i]) {
            for (int j = 0; j < graf->noduri[i]->numarTrasee; j++) {
                if (graf->noduri[i]->trasee[j].distanta > distanta) {
                    count++;
                }
            }
        }
    }
    return count;
}

// Funcția 3: Modifică distanța unui traseu specificat
void modificaDistantaTraseu(Graf* graf, char* start, char* stop, float nouaDistanta) {
    for (int i = 0; i < graf->numarNoduri; i++) {
        if (graf->noduri[i] && strcmp(graf->noduri[i]->denumire, start) == 0) {
            for (int j = 0; j < graf->noduri[i]->numarTrasee; j++) {
                if (strcmp(graf->noduri[i]->trasee[j].denumireStop, stop) == 0) {
                    graf->noduri[i]->trasee[j].distanta = nouaDistanta;
                    return;
                }
            }
        }
    }
}

// Funcția 4: Elimină un nod și traseele asociate
void eliminaNod(Graf* graf, char* denumire) {
    for (int i = 0; i < graf->numarNoduri; i++) {
        if (graf->noduri[i] && strcmp(graf->noduri[i]->denumire, denumire) == 0) {
            for (int j = 0; j < graf->noduri[i]->numarTrasee; j++) {
                free(graf->noduri[i]->trasee[j].denumireStart);
                free(graf->noduri[i]->trasee[j].denumireStop);
            }
            free(graf->noduri[i]->trasee);
            free(graf->noduri[i]->denumire);
            free(graf->noduri[i]);
            graf->noduri[i] = NULL;
        }
    }

    // Elimină traseele către nodul șters
    for (int i = 0; i < graf->numarNoduri; i++) {
        if (graf->noduri[i]) {
            for (int j = 0; j < graf->noduri[i]->numarTrasee; j++) {
                if (strcmp(graf->noduri[i]->trasee[j].denumireStop, denumire) == 0) {
                    for (int k = j; k < graf->noduri[i]->numarTrasee - 1; k++) {
                        graf->noduri[i]->trasee[k] = graf->noduri[i]->trasee[k + 1];
                    }
                    graf->noduri[i]->numarTrasee--;
                    graf->noduri[i]->trasee = (Traseu*)realloc(graf->noduri[i]->trasee, graf->noduri[i]->numarTrasee * sizeof(Traseu));
                    j--;
                }
            }
        }
    }
}

// Funcția 5: Salvează traseele sub o anumită distanță într-un graf nou
Graf* salvareTraseeSubDistanta(Graf* graf, float distanta) {
    Graf* grafNou = creareGraf(graf->numarNoduri);

    for (int i = 0; i < graf->numarNoduri; i++) {
        if (graf->noduri[i]) {
            adaugaNod(grafNou, graf->noduri[i]->denumire);
            for (int j = 0; j < graf->noduri[i]->numarTrasee; j++) {
                if (graf->noduri[i]->trasee[j].distanta < distanta) {
                    adaugaTraseu(grafNou, graf->noduri[i]->trasee[j].id, graf->noduri[i]->trasee[j].denumireStart, graf->noduri[i]->trasee[j].denumireStop, graf->noduri[i]->trasee[j].distanta);
                }
            }
        }
    }

    return grafNou;
}

int main() {
    Graf* graf = citireGrafDinFisier("trasee.txt");

    printf("Traseele din graf:\n");
    afisareGraf(graf);
    printf("\n\n");

    float distantaPrag = 10.0;
    int nrTrasee = numarTraseePesteDistanta(graf, distantaPrag);
    printf("Numar de trasee cu distanta mai mare de %.2f: %d\n\n", distantaPrag, nrTrasee);

    char* start = "A";
    char* stop = "B";
    float nouaDistanta = 8.5;
    modificaDistantaTraseu(graf, start, stop, nouaDistanta);
    printf("Traseele dupa modificarea distantei intre %s si %s:\n", start, stop);
    afisareGraf(graf);
    printf("\n\n");

    char* nodDeEliminat = "C";
    eliminaNod(graf, nodDeEliminat);
    printf("Traseele dupa eliminarea nodului %s:\n", nodDeEliminat);
    afisareGraf(graf);
    printf("\n\n");

    float distantaMaxima = 5.0;
    Graf* grafNou = salvareTraseeSubDistanta(graf, distantaMaxima);
    printf("Traseele cu distanta mai mica de %.2f:\n", distantaMaxima);
    afisareGraf(grafNou);
    printf("\n\n");

    eliberareGraf(graf);
    eliberareGraf(grafNou);

    return 0;
}
