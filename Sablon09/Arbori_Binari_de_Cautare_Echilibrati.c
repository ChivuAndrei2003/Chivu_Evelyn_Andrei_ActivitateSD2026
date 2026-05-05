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
    if(arbore == NULL)
    {
        return 0;
    }

    return calculeazaInaltimeArbore(arbore->stanga)
        - calculeazaInaltimeArbore(arbore->dreapta);

}
// dif. dintre inaltimea subarborelui stang si drept poate fi doar -1, 0 sau 1.
void rotireStanga(Nod** radacinaSubarbore)
{
    Nod* vecheaRadacina = *radacinaSubarbore;
    Nod* nouaRadacina = vecheaRadacina->dreapta;

    vecheaRadacina->dreapta = nouaRadacina->stanga;
    nouaRadacina->stanga = vecheaRadacina;

    *radacinaSubarbore = nouaRadacina;
}

void rotieDreapta(Nod** radacinaSubarbore)
{
    Nod* vecheaRadacina = *radacinaSubarbore;
    Nod* nouaRadacina = vecheaRadacina->stanga;

    vecheaRadacina->stanga = nouaRadacina->dreapta;
    nouaRadacina->dreapta = vecheaRadacina;

    *radacinaSubarbore = nouaRadacina;
}

void adaugaMasinaInArboreEchilibrat(Nod** arbore,Masina masinaNoua)
{
    //adauga o noua masina pe care o primim ca parametru in arbore,
    //astfel incat sa respecte principiile de arbore binar de cautare ECHILIBRAT
    //dupa o anumita cheie pe care o decideti - poate fi ID

    if(*arbore != NULL)
    {
        if((*arbore)->info.id < masinaNoua.id)
        {
            adaugaMasinaInArboreEchilibrat(&(*arbore)->dreapta,masinaNoua);
        }
        else
        {
            adaugaMasinaInArboreEchilibrat(&(*arbore)->stanga,masinaNoua);
        }

        int balanceFactor = calculeazaBalanceFactor(*arbore);
        if(balanceFactor == 2)
        {
            if(calculeazaBalanceFactor((*arbore)->stanga) >= 0)
            {
                rotieDreapta(arbore);
            }
            else
            {
                rotireStanga(&(*arbore)->stanga);
                rotieDreapta(arbore);
            }
        }
        else if(balanceFactor == -2)
        {
            if(calculeazaBalanceFactor((*arbore)->dreapta) > 0)
            {
                rotieDreapta(&(*arbore)->dreapta);
            }
            rotireStanga(arbore);
        }
    }
    else
    {
        (*arbore) = (Nod*)malloc(sizeof(Nod));
        (*arbore)->info = masinaNoua;
        (*arbore)->stanga = NULL;
        (*arbore)->dreapta = NULL;
    }
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier)
{
    //functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
    //prin apelul repetat al functiei citireMasinaDinFisier()
    //ATENTIE - la final inchidem fisierul/stream-ul
    FILE* file = fopen(numeFisier,"r");
    Nod* arbore = NULL;

    if(file != NULL)
    {
        char buffer[100];
        int nrLinii = 0;
        while(fgets(buffer,100,file) != NULL)
        {
            nrLinii++;
        }

        rewind(file);
        for(int i = 0; i < nrLinii; i++)
        {
            Masina masina = citireMasinaDinFisier(file);
            adaugaMasinaInArboreEchilibrat(&arbore,masina);
        }

        fclose(file);
    }
    else
    {
        printf("Fisierul %s nu a putut fi deschis.\n",numeFisier);
    }

    return arbore;
}

void afisareMasiniPreordine(Nod* arbore)
{
    if(arbore != NULL)
    {
        afisareMasina(arbore->info);
        afisareMasiniPreordine(arbore->stanga);
        afisareMasiniPreordine(arbore->dreapta);
    }
}

void afisareMasiniInordine(Nod* arbore)
{
    if(arbore != NULL)
    {
        afisareMasiniInordine(arbore->stanga);
        afisareMasina(arbore->info);
        afisareMasiniInordine(arbore->dreapta);
    }
}

void afisareMasiniPostordine(Nod* arbore)
{
    if(arbore != NULL)
    {
        afisareMasiniPostordine(arbore->stanga);
        afisareMasiniPostordine(arbore->dreapta);
        afisareMasina(arbore->info);
    }
}

void afisareMasiniDinArbore(Nod* arbore)
{
    //afiseaza toate elemente de tip masina din arborele creat
    //prin apelarea functiei afisareMasina()
    //parcurgerea arborelui poate fi realizata in TREI moduri
    //folositi toate cele TREI moduri de parcurgere
    printf("Parcurgere preordine:\n");
    afisareMasiniPreordine(arbore);

    printf("Parcurgere inordine:\n");
    afisareMasiniInordine(arbore);

    printf("Parcurgere postordine:\n");
    afisareMasiniPostordine(arbore);
}

void dezalocareArboreDeMasini(Nod** root)
{
    //sunt dezalocate toate masinile si arborele de elemente
    if((*root) != NULL)
    {
        dezalocareArboreDeMasini(&(*root)->stanga);
        dezalocareArboreDeMasini(&(*root)->dreapta);
        free((*root)->info.model);
        free((*root)->info.numeSofer);
        free(*root);
        *root = NULL;
    }
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.
Masina getMasinaByID(Nod* root,int id)
{
    Masina m;
    m.id = -1;
    if(root == NULL) return m;

    if(id > root->info.id)
    {
        return getMasinaByID(root->dreapta,id);
    }
    else if(id < root->info.id)
    {
        return getMasinaByID(root->stanga,id);
    }
    else
    {
        m = root->info;
        m.numeSofer = malloc(strlen(root->info.numeSofer) + 1);
        strcpy(m.numeSofer,root->info.numeSofer);

        m.model = malloc(strlen(root->info.model) + 1);
        strcpy(m.model,root->info.model);

        return m;
    }


    return m;
}

int determinaNumarNoduri(Nod* root)
{
    //calculeaza numarul total de noduri din arborele binar de cautare
    if(root == NULL) return 0;

    int s = determinaNumarNoduri(root->stanga);
    int d = determinaNumarNoduri(root->dreapta);

    return 1 + s + d;
}
float calculeazaPretTotal(Nod* radacina)
{
    if(radacina == NULL)
    {
        return 0;
    }
    else
    {
        float sumaStanga = calculeazaPretTotal(radacina->stanga);
        float sumaDreapta = calculeazaPretTotal(radacina->dreapta);

        return sumaStanga + sumaDreapta + radacina->info.pret;
    }
}

float calculeazaPretulMasinilorUnuiSofer(Nod* root,const char* numeSofer)
{
    if(root == NULL)return 0;

    float pret = 0;
    if(strcmp(root->info.numeSofer,numeSofer) == 0)
    {
        pret += root->info.pret;
    }
    pret += calculeazaPretulMasinilorUnuiSofer(root->stanga,numeSofer);
    pret += calculeazaPretulMasinilorUnuiSofer(root->dreapta,numeSofer);

    return pret;
}

int main(void)
{
    Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");

    afisareMasiniDinArbore(arbore);

    printf("Numar noduri: %d\n",determinaNumarNoduri(arbore));
    printf("Inaltime arbore: %d\n",calculeazaInaltimeArbore(arbore));
    printf("Balance factor radacina: %d\n",calculeazaBalanceFactor(arbore));
    printf("Pret total: %.2f\n",calculeazaPretTotal(arbore));
    printf("Pret total masini sofer Ionescu: %.2f\n",calculeazaPretulMasinilorUnuiSofer(arbore,"Ionescu"));

    Masina masinaCautata = getMasinaByID(arbore,5);
    if(masinaCautata.id != -1)
    {
        printf("\nMasina cu id-ul 5:\n");
        afisareMasina(masinaCautata);
        free(masinaCautata.model);
        free(masinaCautata.numeSofer);
    }
    else
    {
        printf("\nNu exista masina cu id-ul 5.\n");
    }

    dezalocareArboreDeMasini(&arbore);

    return 0;
}
