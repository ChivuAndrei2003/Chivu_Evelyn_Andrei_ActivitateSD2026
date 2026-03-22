#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date
typedef struct StructuraAvion Masina;

struct StructuraAvion
{
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};


void afisareMasina (Masina masina)
{
    printf ("\n Id masina %d:",masina.id);
    printf ("\n Nr. usi masina %d:",masina.nrUsi);
    printf ("\n Pret %.2F:",masina.pret);
    printf ("\n Model %s:",masina.model);
    printf ("\n Nume sofer %s:",masina.numeSofer);
    printf ("\n Serie %c:",masina.serie);
}

void afisareVectorMasini (Masina* vectorMasini,int nrMasini)
{
    //afiseaza toate elemente de tip masina din vector
    //prin apelarea functiei afisareMasina()
    for(int i = 0; i < nrMasini; i++)
    {
        afisareMasina (*(vectorMasini + i));
    }

}

void adaugaMasinaInVector (Masina** masini,int* nrMasini,Masina masinaNoua)
{
    //adauga in vectorul primit o noua masina pe care o primim ca parametru
    //ATENTIE - se modifica numarul de masini din vector;
    Masina* m = (Masina*)malloc(sizeof (Masina) * ((*nrMasini) + 1));

    for(int i = 0; i < *nrMasini; i++)
    {
        m[i] = (*masini)[i];
    }
    m[(*nrMasini)] = masinaNoua;
    (*nrMasini)++;
    free(*masini);
    *masini = m;

}

Masina citireMasinaFisier (FILE* file)
{
    //functia citeste o masina dintr-un strceam deja deschis
    //masina citita este returnata;

    char buffer[100];
    char separator[3] = ",\n";
    fgets(buffer,100,file);
    char* aux;
    Masina masina;
    aux = strtok(buffer,separator);
    masina.id = atoi(aux);
    masina.nrUsi = atoi(strtok(NULL,separator));
    masina.pret = atof(strtok(NULL,separator));
    aux = strtok(NULL,separator);
    masina.model = malloc(strlen(aux) + 1);
    strcpy(masina.model,aux);

    aux = strtok(NULL,separator);
    masina.numeSofer = malloc(strlen(aux) + 1);
    strcpy(masina.numeSofer,aux);

    masina.serie = *strtok(NULL,separator);

    return masina;
}

Masina* citireVectorMasiniFisier (const char* numeFisier,int* nrMasiniCitite)
{
    //functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
    //prin apelul repetat al functiei citireMasinaFisier()
    //numarul de masini este determinat prin numarul de citiri din fisier
    //ATENTIE - la final inchidem fisierul/stream-ul

    FILE* file = fopen(numeFisier,"r");
    Masina* masini = NULL;

    do
    {
        adaugaMasinaInVector(&masini,nrMasiniCitite,citireMasinaFisier(file));
    }
    while(!feof(file));

    fclose(file);

    return masini;
}

void dezalocareVectorMasini (Masina** vector,int* nrMasini)
{
    //este dezalocat intreg vectorul de masini

    for(int i = 0;i < *nrMasini;i++)
    {
        free((*vector)[i].model);
        free((*vector)[i].numeSofer);
    }

    free(*vector);
    *vector = NULL;
    *nrMasini = 0;
}

int main ( )
{
    int nrMasini = 0;

    // citire vector masini din fisier
    Masina* masini = citireVectorMasiniFisier("masina.txt",&nrMasini);

    //   afisare vector masini
    printf("\nVectorul de masini citit din fisier:\n");
    afisareVectorMasini(masini,nrMasini);

    // dezaloc. memoria
    dezalocareVectorMasini(&masini,&nrMasini);

    printf("\nDupa dezalocare:\n");
    printf("nrMasini = %d\n",nrMasini);
    printf("masini = %p\n",masini);


    return 0;
}