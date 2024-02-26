//Seminar 1 - gr. 1058

//examen 60%
//seminar 40% 
		//20% (test dupa strcturile liniare(vectori, liste, hash poate si stack si queue)
		//20% activitate seminar + acasa (task-uri NEPUNCTATE + un repo public cu laboratoarele + exercitiile de practice) + test grila

#include <stdio.h>
#include <malloc.h>


struct Masina {
	char* marca;
	int an;
	float capacitateCilindrica;

};

void afisareMasina(const struct Masina* m) {
	if (m->marca != NULL) {
		printf("Marca: %s\n", m->marca);
	}
	printf("Capacitate cilindric: %5.2f\n", m->capacitateCilindrica);
	printf("An fabricatie: %d\n\n", m->an);
}

struct Masina initializareMasina(const char* marca, int an, float capaacitateCilindrica) {
	struct Masina m;
	m.an = an;
	m.capacitateCilindrica = capaacitateCilindrica;
	if (marca != NULL) {
		m.marca = malloc(strlen(marca) + 1);
		strcpy(m.marca, marca);
	}
	else {
		m.marca = malloc(strlen("N/A") + 1);
		strcpy(m.marca, "N/A");
	}
	return m;
}

void dezalocareMasina(struct Masina* masina) {
	free(masina->marca);
	masina->marca = NULL;
}


void main() {
	struct Masina masina1;
	//Dacia
	masina1.marca = (char*)malloc(sizeof(char) * (strlen("Dacia") + 1));
	strcpy(masina1.marca, "Dacia");
	masina1.capacitateCilindrica = 1.9;
	masina1.an = 2023;

	afisareMasina(&masina1);
	dezalocareMasina(&masina1);

	struct Masina masina2 = initializareMasina("Ford", 2022, 0.8);
	afisareMasina(&masina2);
	dezalocareMasina(&masina2);

//	afisareMasina(&masina2);
}