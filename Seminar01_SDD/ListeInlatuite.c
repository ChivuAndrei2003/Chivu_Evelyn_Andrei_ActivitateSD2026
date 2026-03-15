#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};

typedef struct StructuraMasina Masina;
typedef struct Nod Nod;

struct Nod {
    Masina info;
    Nod* next;
};

int citireMasinaDinFisier(FILE* file, Masina* m1) {
    char buffer[100];
    char sep[] = ",\n";

    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        return 0;
    }

    char* aux = strtok(buffer, sep);
    if (aux == NULL) return 0;
    m1->id = atoi(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) return 0;
    m1->nrUsi = atoi(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) return 0;
    m1->pret = (float)atof(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) return 0;
    m1->model = malloc(strlen(aux) + 1);
    if (m1->model == NULL) return 0;
    strcpy(m1->model, aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        free(m1->model);
        return 0;
    }
    m1->numeSofer = malloc(strlen(aux) + 1);
    if (m1->numeSofer == NULL) {
        free(m1->model);
        return 0;
    }
    strcpy(m1->numeSofer, aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        free(m1->model);
        free(m1->numeSofer);
        return 0;
    }
    m1->serie = aux[0];

    return 1;
}

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi: %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* lista) {
    while (lista) {
        afisareMasina(lista->info);
        lista = lista->next;
    }
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
    Nod* nou = malloc(sizeof(Nod));
    if (nou == NULL) return;

    nou->info = masinaNoua;
    nou->next = NULL;

    if (*lista == NULL) {
        *lista = nou;
    } else {
        Nod* p = *lista;
        while (p->next) {
            p = p->next;
        }
        p->next = nou;
    }
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (f == NULL) {
        printf("Nu s-a putut deschide fisierul.\n");
        return NULL;
    }

    Nod* lista = NULL;
    Masina m;

    while (citireMasinaDinFisier(f, &m)) {
        adaugaMasinaInLista(&lista, m);
    }

    fclose(f);
    return lista;
}

void dezalocareListaMasini(Nod** lista) {
    while (*lista) {
        free((*lista)->info.model);
        free((*lista)->info.numeSofer);
        Nod* p = *lista;
        *lista = (*lista)->next;
        free(p);
    }
}

int main() {
    Nod* lista = citireListaMasiniDinFisier("masini2.txt");
    afisareListaMasini(lista);
    dezalocareListaMasini(&lista);
    return 0;
}