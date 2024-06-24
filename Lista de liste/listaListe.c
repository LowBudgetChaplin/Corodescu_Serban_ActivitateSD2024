#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Platforma {
    char* nume;
    char* owner;
    int dataCreare;
    int nrUtilizatori;
    int pretLicenta;
} Platforma;

typedef struct Nod {
    Platforma info;
    struct Nod* next;
} Nod;

typedef struct ListaDeListe {
    Nod** liste;
    int numarListe;
} ListaDeListe;

void afisarePlatforma(Platforma p) {
    printf("Platforma: %s, prop: %s, data: %d, util: %d, pretLicenta: %d\n",
        p.nume, p.owner, p.dataCreare, p.nrUtilizatori, p.pretLicenta);
}

ListaDeListe initListaDeListe(int numarListe) {
    ListaDeListe ldl;
    ldl.liste = (Nod**)malloc(sizeof(Nod*) * numarListe);
    for (int i = 0; i < numarListe; i++) {
        ldl.liste[i] = NULL;
    }
    ldl.numarListe = numarListe;
    return ldl;
}

void inserareInLista(Nod** cap, Platforma p) {
    Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->info.nume = (char*)malloc(strlen(p.nume) + 1);
    strcpy(nodNou->info.nume, p.nume);
    nodNou->info.owner = (char*)malloc(strlen(p.owner) + 1);
    strcpy(nodNou->info.owner, p.owner);
    nodNou->info.dataCreare = p.dataCreare;
    nodNou->info.nrUtilizatori = p.nrUtilizatori;
    nodNou->info.pretLicenta = p.pretLicenta;
    nodNou->next = *cap;
    *cap = nodNou;
}

void inserareInListaDeListe(ListaDeListe* ldl, Platforma p) {
    int index = p.pretLicenta / 500;
    if (index >= ldl->numarListe) {
        index = ldl->numarListe - 1;
    }
    inserareInLista(&ldl->liste[index], p);
}

void afisareLista(Nod* cap) {
    Nod* temp = cap;
    while (temp) {
        afisarePlatforma(temp->info);
        temp = temp->next;
    }
}

void afisareListaDeListe(ListaDeListe ldl) {
    for (int i = 0; i < ldl.numarListe; i++) {
        printf("Lista %d:\n", i);
        afisareLista(ldl.liste[i]);
        printf("\n");
    }
}

void dezalocareLista(Nod* cap) {
    while (cap) {
        Nod* temp = cap;
        cap = cap->next;
        free(temp->info.nume);
        free(temp->info.owner);
        free(temp);
    }
}

void dezalocareListaDeListe(ListaDeListe* ldl) {
    for (int i = 0; i < ldl->numarListe; i++) {
        dezalocareLista(ldl->liste[i]);
    }
    free(ldl->liste);
    ldl->liste = NULL;
    ldl->numarListe = 0;
}

Platforma citirePlatforma(FILE* f) {
    Platforma p;
    char buffer[256];

    if (fgets(buffer, 256, f) == NULL) {
        printf("Error reading nume\n");
        exit(1);
    }
    buffer[strcspn(buffer, "\n")] = 0;
    p.nume = (char*)malloc(strlen(buffer) + 1);
    if (p.nume == NULL) {
        printf("Error allocating memory for nume\n");
        exit(1);
    }
    strcpy(p.nume, buffer);

    if (fgets(buffer, 256, f) == NULL) {
        printf("Error reading owner\n");
        exit(1);
    }
    buffer[strcspn(buffer, "\n")] = 0;
    p.owner = (char*)malloc(strlen(buffer) + 1);
    if (p.owner == NULL) {
        printf("Error allocating memory for owner\n");
        exit(1);
    }
    strcpy(p.owner, buffer);

    if (fgets(buffer, 256, f) == NULL) {
        printf("Error reading dataCreare\n");
        exit(1);
    }
    sscanf(buffer, "%d", &p.dataCreare);

    if (fgets(buffer, 256, f) == NULL) {
        printf("Error reading nrUtilizatori\n");
        exit(1);
    }
    sscanf(buffer, "%d", &p.nrUtilizatori);

    if (fgets(buffer, 256, f) == NULL) {
        printf("Error reading pretLicenta\n");
        exit(1);
    }
    sscanf(buffer, "%d", &p.pretLicenta);

    return p;
}

ListaDeListe citireListaDeListe(const char* file, int numarListe) {
    ListaDeListe ldl = initListaDeListe(numarListe);

    if (file && strlen(file) != 0) {
        FILE* f = fopen(file, "r");
        if (f != NULL) {
            int dim;
            fscanf(f, "%d\n", &dim);
            for (int i = 0; i < dim; i++) {
                Platforma p = citirePlatforma(f);
                inserareInListaDeListe(&ldl, p);
            }
            fclose(f);
        }
        else {
            printf("Error opening file\n");
            exit(1);
        }
    }
    return ldl;
}

void afisarePlatformeIntervalPret(ListaDeListe ldl, int minPret, int maxPret) {
    for (int i = 0; i < ldl.numarListe; i++) {
        Nod* temp = ldl.liste[i];
        while (temp) {
            if (temp->info.pretLicenta >= minPret && temp->info.pretLicenta <= maxPret) {
                afisarePlatforma(temp->info);
            }
            temp = temp->next;
        }
    }
}

void modificaPretLicenta(ListaDeListe* ldl, char* nume, int noulPret) {
    for (int i = 0; i < ldl->numarListe; i++) {
        Nod* temp = ldl->liste[i];
        Nod* prev = NULL;
        while (temp) {
            if (strcmp(temp->info.nume, nume) == 0) {
                if (prev) {
                    prev->next = temp->next;
                }
                else {
                    ldl->liste[i] = temp->next;
                }
                temp->info.pretLicenta = noulPret;
                inserareInListaDeListe(ldl, temp->info);
                free(temp->info.nume);
                free(temp->info.owner);
                free(temp);
                return;
            }
            prev = temp;
            temp = temp->next;
        }
    }
    printf("Platforma cu numele %s nu a fost gasita\n", nume);
}

void sortarePlatforme(ListaDeListe* ldl) {
    for (int i = 0; i < ldl->numarListe; i++) {
        Nod* sorted = NULL;
        Nod* current = ldl->liste[i];
        while (current) {
            Nod* next = current->next;
            if (!sorted || sorted->info.pretLicenta > current->info.pretLicenta) {
                current->next = sorted;
                sorted = current;
            }
            else {
                Nod* temp = sorted;
                while (temp->next && temp->next->info.pretLicenta < current->info.pretLicenta) {
                    temp = temp->next;
                }
                current->next = temp->next;
                temp->next = current;
            }
            current = next;
        }
        ldl->liste[i] = sorted;
    }
}

int main() {
    ListaDeListe ldl = citireListaDeListe("platforme.txt", 5);

    printf("Platformele din Lista de Liste:\n");
    afisareListaDeListe(ldl);
    printf("\n\n");

    Platforma platformaNoua = { "Platforma Noua", "Proprietar Nou", 2023, 1000, 700 };
    inserareInListaDeListe(&ldl, platformaNoua);
    printf("Lista de Liste dupa inserarea unei noi platforme:\n");
    afisareListaDeListe(ldl);
    printf("\n\n");

    printf("Platformele din intervalul de pret 600-1000:\n");
    afisarePlatformeIntervalPret(ldl, 600, 1000);
    printf("\n\n");

    modificaPretLicenta(&ldl, "Platforma 2", 800);
    printf("Lista de Liste dupa modificarea pretului de licenta pentru Platforma 2:\n");
    afisareListaDeListe(ldl);
    printf("\n\n");

    sortarePlatforme(&ldl);
    printf("Platformele sortate in ordinea crescatoare a pretului de licenta:\n");
    afisareListaDeListe(ldl);
    printf("\n\n");

    dezalocareListaDeListe(&ldl);

    return 0;
}
