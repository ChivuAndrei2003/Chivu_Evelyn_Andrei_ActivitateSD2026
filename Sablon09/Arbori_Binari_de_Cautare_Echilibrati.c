#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina
{
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina;
typedef struct Nod Nod;

//creare structura pentru un nod dintr-un arbore binar de cautare
struct Nod
{
    Masina info;
    Nod* stanga;
    Nod* dreapta;
};

Masina citireMasinaDinFisier(FILE* file)
{
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer,100,file);
    char* aux;
    Masina m1;
    aux = strtok(buffer,sep);
    m1.id = atoi(aux);
    m1.nrUsi = atoi(strtok(NULL,sep));
    m1.pret = atof(strtok(NULL,sep));
    aux = strtok(NULL,sep);
    m1.model = malloc(strlen(aux) + 1);
    strcpy(m1.model,aux);

    aux = strtok(NULL,sep);
    m1.numeSofer = malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer,aux);

    m1.serie = *strtok(NULL,sep);
    return m1;
}

void afisareMasina(Masina masina)
{
    printf("Id: %d\n",masina.id);
    printf("Nr. usi : %d\n",masina.nrUsi);
    printf("Pret: %.2f\n",masina.pret);
    printf("Model: %s\n",masina.model);
    printf("Nume sofer: %s\n",masina.numeSofer);
    printf("Serie: %c\n\n",masina.serie);
}

int calculeazaInaltimeArbore(Nod* arbore)
{
    //calculeaza inaltimea arborelui care este data de 
    //lungimea maxima de la radacina pana la cel mai indepartat nod frunza
    if(arbore != NULL)
    {
        int inaltimeStanga = calculeazaInaltimeArbore(arbore->stanga);
        int inaltimeDreapta = calculeazaInaltimeArbore(arbore->dreapta);

        return(inaltimeStanga > inaltimeDreapta ? inaltimeStanga : inaltimeDreapta) + 1;
    }

    return 0;
}

// balance_factor = inaltime stanga - inaltime dreapta 

// BalanceFactor = 0 -> stanga si dreapta au aceeași înaltime
// BalanceFactor = 1 -> stanga este cu 1 nivel mai inalta
// BalanceFactor =-1 -> dreapta este cu 1 nivel mai înalta
// BalanceFactor = 2 -> arbore dezechilibrat spre stanga
// BalanceFactor =-2 -> arbore dezechilibrat spre dreapta
int calculeazaBalanceFactor(Nod* arbore)
{
    return calculeazaInaltimeArbore(arbore->stanga)
        - calculeazaInaltimeArbore(arbore->dreapta);

}

void adaugaMasinaInArboreEchilibrat(Nod** arbore,Masina masinaNoua)
{
    //adauga o noua masina pe care o primim ca parametru in arbore,
    //astfel incat sa respecte principiile de arbore binar de cautare ECHILIBRAT
    //dupa o anumita cheie pe care o decideti - poate fi ID

    if(arbore != NULL)
    {

    }
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier)
{
    //functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
    //prin apelul repetat al functiei citireMasinaDinFisier()
    //ATENTIE - la final inchidem fisierul/stream-ul
}

void afisareMasiniDinArbore(/*arbore de masini*/)
{
    //afiseaza toate elemente de tip masina din arborele creat
    //prin apelarea functiei afisareMasina()
    //parcurgerea arborelui poate fi realizata in TREI moduri
    //folositi toate cele TREI moduri de parcurgere
}

void dezalocareArboreDeMasini(/*arbore de masini*/)
{
    //sunt dezalocate toate masinile si arborele de elemente
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(/*arborele de masini*/int id);

int determinaNumarNoduri(/*arborele de masini*/);

float calculeazaPretTotal(/*arbore de masini*/);

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/ const char* numeSofer);

int main( )
{


    return 0;
}