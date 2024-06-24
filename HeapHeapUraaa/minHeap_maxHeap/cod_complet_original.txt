#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct Platforma
{
    char* nume;
    char* owner;
    int dataCreare;
    int nrUtilizatori;
    int pretLicenta;
} plat;

plat crearePlatforma(const char* nume, const char* owner, int data, int nr, int pret)
{
    plat p;
    p.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
    strcpy(p.nume, nume);

    p.owner = (char*)malloc(sizeof(char) * (strlen(owner) + 1));
    strcpy(p.owner, owner);

    p.dataCreare = data;
    p.nrUtilizatori = nr;
    p.pretLicenta = pret;
    return p;
}

void afisarePlatforma(plat p)
{
    printf("\nPlatforma: %s, prop: %s, data: %d, util: %d, pretLicenta: %d",
        p.nume, p.owner, p.dataCreare, p.nrUtilizatori, p.pretLicenta);
}

typedef struct HEAP {
    int dim;
    plat* vector;
} heap;

heap initHeap(int dim)
{
    heap h;
    h.dim = dim;
    h.vector = (plat*)malloc(sizeof(plat) * dim);
    return h;
}

// functia de filtrare
void filtrare(heap h, int poz)
{
    if (h.vector)
    {
        if (poz <= (h.dim - 1) / 2)
        {
            int fiuS = 2 * poz + 1;
            int fiuD = 2 * poz + 2;
            int pozmax = poz;
            // min-heap
            // in parinte sa avem prioritatea minima

            if (fiuS < h.dim && h.vector[fiuS].pretLicenta < h.vector[pozmax].pretLicenta)
            {
                pozmax = fiuS;
            }

            if (fiuD < h.dim && h.vector[fiuD].pretLicenta < h.vector[pozmax].pretLicenta)
            {
                pozmax = fiuD;
            }
            if (pozmax != poz)
            {
                plat aux = h.vector[poz];
                h.vector[poz] = h.vector[pozmax];
                h.vector[pozmax] = aux;

                if (pozmax < (h.dim - 1) / 2)
                {
                    filtrare(h, pozmax);
                }
            }
        }
    }
}

void afisareHeap(heap h)
{
    if (h.vector)
    {
        for (int i = 0; i < h.dim; i++)
        {
            afisarePlatforma(h.vector[i]);
        }
    }
}

heap inserareHeap(heap h, plat p)
{
    if (h.vector)
    {
        // imi aloc o dimensiune noua si imi aloc memorie pentru noul vector
        int dimnoua = h.dim + 1;
        plat* copie = (plat*)malloc(sizeof(plat) * dimnoua);
        // punem in copie ce aveam in vechiul vector;
        for (int i = 0; i < h.dim; i++)
        {
            copie[i] = h.vector[i];
        }
        // practic pe noua pozitie punem elementul nostru
        copie[dimnoua - 1] = p;

        free(h.vector);
        h = initHeap(dimnoua);
        for (int i = 0; i < dimnoua; i++)
        {
            h.vector[i] = copie[i];
        }
        free(copie);

        // dupa ce am adaugat in heap, trebuie sa si filtram
        // aici filtram de jos in sus
        for (int i = (h.dim - 1) / 2; i >= 0; i--)
        {
            filtrare(h, i);
        }

        return h;
    }
    else
    {
        h = initHeap(1);
        h.vector[0] = p;
        return h;
    }
}

plat citirePlatforma(FILE* f)
{
    plat p;
    char buffer[30];
    fscanf(f, "%s", buffer);
    p.nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(p.nume, buffer);

    fscanf(f, "%s", buffer);
    p.owner = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(p.owner, buffer);

    fscanf(f, "%d", &p.dataCreare);
    fscanf(f, "%d", &p.nrUtilizatori);
    fscanf(f, "%d", &p.pretLicenta);

    return p;
}

heap citireFisier(FILE* f)
{
    int nr = 0;
    fscanf(f, "%d", &nr);
    heap h = initHeap(0);
    for (int i = 0; i < nr; i++)
    {
        h = inserareHeap(h, citirePlatforma(f));
    }
    return h;
}

// cerinta2
void modificaPretMinim(heap h, int pretnou)
{
    if (h.vector)
    {
        int pozMin = 0;
        for (int i = 0; i < h.dim; i++)
        {
            // facem if ul asta ca sa reusim sa comparam pret din i cu pretul din pozMin si astfel aflam care este minimul
            if (h.vector[i].pretLicenta < h.vector[pozMin].pretLicenta)
            {
                pozMin = i;
            }
        }
        // modificam pretul gasit
        h.vector[pozMin].pretLicenta = pretnou;

        // refiltram
        for (int i = (h.dim - 1) / 2; i >= 0; i--)
        {
            filtrare(h, i);
        }
    }
}

void freePlatforma(plat p)
{
    free(p.nume);
    free(p.owner);
}

void freeHeap(heap h)
{
    if (h.vector)
    {
        for (int i = 0; i < h.dim; i++)
        {
            freePlatforma(h.vector[i]);
        }
        free(h.vector);
    }
}

//copiere in abc
typedef struct Nod
{
    plat info;
    struct Nod* dr;
    struct Nod* st;
}nod;

nod* creareNod(plat info, nod* dr, nod* st)
{
    nod* nou = (nod*)malloc(sizeof(nod));
    nou->info = crearePlatforma(info.nume, info.owner, info.dataCreare, info.nrUtilizatori, info.pretLicenta);
    nou->dr = dr;
    nou->st = st;
    return nou;
}

nod* inserareabc(nod* root, plat p)
{
    if (root)
    {
        if (p.pretLicenta < root->info.pretLicenta)
        {
            root->st = inserareabc(root->st, p);
        }
        if (p.pretLicenta > root->info.pretLicenta)
        {
            root->dr = inserareabc(root->dr, p);
        }
        return root;
    }
    else
    {
        creareNod(p, NULL, NULL);
    }
   
}

void copieHeapinAbc(heap h, nod** root, int nr)
{
    if (h.vector)
    {
        for (int i = 0; i < h.dim; i++)
        {
            if (h.vector[i].pretLicenta > nr)
            {
                *root = inserareabc(*root, h.vector[i]);
            }
        }
    }
}

void afisareSRD(nod* root)
{
    if (root)
    {
        afisareSRD(root->st);
        afisarePlatforma(root->info);
        afisareSRD(root->dr);
    }
}

//cerinta4
void cerinta4(nod* root)
{
    if (root)
    {
        if (root->st != NULL && root->dr == NULL || root->st == NULL && root->dr != NULL)
        {
            afisarePlatforma(root->info);
            
        }
        //apelurile recursive trebuie mereu sa le fac in afara if ului
        cerinta4(root->st);
        cerinta4(root->dr);
        
    }
}

//cerinta5
void dezalocareHeap(heap h)
{
    if (h.vector)
    {
        for (int i = 0; i < h.dim; i++)
        {
            free(h.vector[i].nume);
            free(h.vector[i].owner);
        }
        free(h.vector);
        h.dim = 0;
    }
}

void dezalocareABC(nod* root)
{
    if (root)
    {
        //sdr
        dezalocareABC(root->st);
        dezalocareABC(root->dr);
        free(root->info.nume);
        free(root->info.owner);
        free(root);
    }
}


void main()
{
    FILE* f = fopen("platforme.txt", "r");
    if (f)
    {
        heap h = citireFisier(f);
        afisareHeap(h);
        modificaPretMinim(h, 200);
        printf("\n----------------cerina2--------------\n");// exemplu de modificare pret
        afisareHeap(h);
        printf("\n----------------cerina3--------------\n");// exemplu de modificare pret
        nod* root = NULL;
        copieHeapinAbc(h, &root, 25);
        afisareSRD(root);
        printf("\n----------------cerina4--------------\n");
        cerinta4(root);
        printf("\n----------------cerina5--------------\n");
        dezalocareHeap(h);
        dezalocareABC(root);
        //freeHeap(h);
        fclose(f);
    }
    else
    {
        printf("Fisierul nu a putut fi deschis.\n");
    }
}
