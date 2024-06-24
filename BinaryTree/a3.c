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


void inserareInArbore(Nod** rad, Rezervare r) {
    if (*rad) {
        if ((*rad)->info.id > r.id) {
            inserareInArbore(&((*rad)->st), r);
        }
        else {
            inserareInArbore(&((*rad)->dr), r);
        }
    }
    else {
        Nod* nod = (Nod*)malloc(sizeof(Nod));
        nod->info = r;
        nod->dr = NULL;
        nod->st = NULL;
        *rad = nod;
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

Rezervare citireRezervare(FILE* f) {
    Rezervare r;
    char buffer[256];

    if (fgets(buffer, 256, f) == NULL) exit(1);
    sscanf(buffer, "%u", &r.id);

    if (fgets(buffer, 256, f) == NULL) exit(1);
    buffer[strcspn(buffer, "\n")] = 0;
    r.denumireHotel = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(r.denumireHotel, buffer);

    if (fgets(buffer, 256, f) == NULL) exit(1);
    sscanf(buffer, "%hhu", &r.camere);
    
    if (fgets(buffer, 256, f) == NULL) exit(1);
    buffer[strcspn(buffer, "\n")] = 0;
    r.numeClient = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(r.numeClient, buffer);

    if (fgets(buffer, 256, f) == NULL)exit(1);
    sscanf(buffer, "%f", &r.suma);

    return r;
}

Nod* citireArbore(const char* file) {
    Nod* rad = NULL;
    if (file && strlen(file) != 0) {
        FILE* f = fopen(file, "r");
        if (f) {
            int dim = 0;
            fscanf(f, "%d\n", &dim);
            for (int i = 0; i < dim; i++) {
                Rezervare r = citireRezervare(f);
                inserareInArbore(&rad, r);
            }
            fclose(f);
        }
    }
    return rad;
}

int nrCamereDinFrunze(Nod* rad) {
    if (!rad) return 0;
    if (rad->st == NULL && rad->dr == NULL) {
        return rad->info.camere;
    }
    return nrCamereDinFrunze(rad->st) + nrCamereDinFrunze(rad->dr);
}

int inaltimeArbore(Nod* rad) {
    if (!rad) return 0;
    int inaltimeStanga = inaltimeArbore(rad->st);
    int inaltimeDreapta= inaltimeArbore(rad->dr);
    return 1 + (inaltimeStanga > inaltimeDreapta ? inaltimeStanga  : inaltimeDreapta) ;
}

Nod* cautaMin(Nod* rad) {
    if (!rad)return NULL;
    while (rad && rad->st) {
        rad = rad->st;
    }
    return rad;
}
Nod* stergeRadacina(Nod* rad) {
    if (!rad) return NULL;

    if (!rad->st && !rad->dr) {
        free(rad->info.denumireHotel);
        free(rad->info.numeClient);
        free(rad);
    }

    if (!rad->st) {
        Nod* temp = rad->dr;
        free(rad->info.denumireHotel);
        free(rad->info.numeClient);
        free(rad);
        return temp;
    }
    if (!rad->dr) {
        Nod* temp = rad->st;
        free(rad->info.denumireHotel);
        free(rad->info.numeClient);
        free(rad);
        return temp;
    }

    Nod* temp = cautaMin(rad->dr);
    rad->info = temp->info;
    rad->dr = stergeRadacina(rad->dr);
    return rad;
}

void salvareInVector(Nod* rad, unsigned int* vector, int* index) {
    if (rad) {
        salvareInVector(rad->st, vector, index);
        if (rad->info.suma > 500) {
            vector[(*index)++] = rad->info.id;
        }
        salvareInVector(rad->dr, vector, index);
    }
}

float calculeazaSumaTotalaDePLata(Nod* rad) {
    if (!rad) return 0;
    return rad->info.suma + calculeazaSumaTotalaDePLata(rad->st) + calculeazaSumaTotalaDePLata(rad->dr);
}

int toateNodurileDinArbore(Nod* rad) {
    if (!rad)return 0;
    return 1 + toateNodurileDinArbore(rad->st) + toateNodurileDinArbore(rad->dr);
}

Rezervare cautaSumaCeaMaiMare(Nod* rad) {
    if (!rad) {
        Rezervare r = { 0, NULL,0,NULL,0.0 };
        return r;
    }
    Rezervare st = cautaSumaCeaMaiMare(rad->st);
    Rezervare dr = cautaSumaCeaMaiMare(rad->dr);
    Rezervare max = rad->info;

    if (st.suma > max.suma) max = st;
    if (dr.suma > max.suma) max = dr;

    return max;
}

void cautaRezervare(Nod* rad, const char* hotelCautat) {
    if (!rad) return NULL;
    cautaRezervare(rad->st, hotelCautat);
    if (strcmp(rad->info.denumireHotel, hotelCautat) == 0) {
        afisareRezervare(rad->info);
    }
   cautaRezervare(rad->dr, hotelCautat);
}

int detNrNoduriFrunza(Nod* rad) {
    if (!rad) return 0;
    int count = 0;
    if (!rad->st && !rad->dr) {
        count = 1;
    }
    return count + detNrNoduriFrunza(rad->st) + detNrNoduriFrunza(rad->dr);
}

void afiseazaNodurileFrunza(Nod* rad) {
    if (rad) {
        if (!rad->st && !rad->dr) {
            afisareRezervare(rad->info);
        }
        else {
            afiseazaNodurileFrunza(rad->st);
            afiseazaNodurileFrunza(rad->dr);
        }
    }
}

void schimbaNumeHotel(Nod* rad, const char* numeVechi ,const char* numeNou) {
    if (rad) {
        schimbaNumeHotel(rad->st, numeVechi, numeNou);
        if (strcmp(rad->info.denumireHotel, numeVechi) == 0) {
            free(rad->info.denumireHotel);
            rad->info.denumireHotel = (char*)malloc(sizeof(char) * (strlen(numeNou) + 1));
            strcpy(rad->info.denumireHotel, numeNou);
        }
        schimbaNumeHotel(rad->dr, numeVechi, numeNou);
    }
}

void main() {
    Nod* radacina = citireArbore("rezervari.txt");
    afisareInOrdine(radacina);









    //int nrCamereFrunze = nrCamereDinFrunze(radacina);
    //printf("\n\nNr de camere din nodurile frunze este: %d", nrCamereFrunze);
    //int inaltimeABC = inaltimeArbore(radacina);
    //printf("\n\nInaltimea arboerelui este: %d", inaltimeABC);



    //int nrNoduri = toateNodurileDinArbore(radacina);
    //printf("\n\nNr de noduri din arbore este: %d", nrNoduri);


    //printf("\n\nArborele dupa stergerea radacinii:");
    //radacina = stergeRadacina(radacina);
    //afisareInOrdine(radacina);    //printf("\n\nArborele dupa stergerea radacinii:");
    //radacina = stergeRadacina(radacina);
    //afisareInOrdine(radacina);

    //int index=0;
    //unsigned int* vector = (unsigned int*)malloc(100*sizeof(unsigned int));
    //salvareInVector(radacina, vector, &index);
    //printf("\n\nValorile salvate in vector sunt:\n");
    //for (int i = 0; i < index; i++) {
    //   printf("%u", vector[i]);
    //}

    //float sumaTotala = calculeazaSumaTotalaDePLata(radacina);
    //printf("\n\nSuma totala a sumelor din arbore este: %.2f", sumaTotala);

    ////int nrNoduri = toateNodurileDinArbore(radacina);
    //printf("\n\nNr de noduri din arbore este: %d", nrNoduri);
    //
    //Rezervare r = cautaSumaCeaMaiMare(radacina);
    //printf("\n\nRezervarea cu cea mai mare suma este: %.2f si anume:", r.suma);
    //afisareRezervare(r);
    //printf("\n\n");

    //const char* hotelCautat = "Ramada";
    //printf("\n\nRezervarile la hotelul %s:\n", hotelCautat);
    //cautaRezervare(radacina, hotelCautat);

    //int nrFrunze = detNrNoduriFrunza(radacina);
    //printf("\n\nNr de noduri frunza este:%d\n", nrFrunze);
    //printf("Nodurile frunza sunt urmatoarele:\n");
    //afiseazaNodurileFrunza(radacina);

    //printf("\n\n");
    //const char* numeVechi = "Ramada";
    //const char* numeNou = "SERBAN LOL";
    //schimbaNumeHotel(radacina, numeVechi, numeNou);
    //afisareInOrdine(radacina);

    dezalocareArbore(radacina);
    radacina = NULL;
    //vector = NULL;
}