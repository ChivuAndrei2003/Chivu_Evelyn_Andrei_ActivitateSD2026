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

//STACK
//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector
struct Nod
{
    Masina info;
    struct Nod* next;
};
typedef struct Nod Nod;

void pushStack(Nod** stiva,Masina masina)
{
    Nod* nou = malloc(sizeof(Nod));
    nou->info = masina;
    nou->next = *stiva;
    *stiva = nou;
}

Masina popStack(Nod** stiva)
{
    if((*stiva) != NULL)
    {
        Masina nou = (*stiva)->info;
        Nod* prev = (*stiva);
        (*stiva) = (*stiva)->next;
        free(prev);
        return nou;
    }

    Masina m;
    m.id = -1;
    return m;
}

int emptyStack(Nod** stiva)
{
    while(*stiva)
    {
        Nod* tmp = *stiva;
        *stiva = tmp->next;
        free(tmp);
    }
    return 0;
}

Nod* citireStackMasiniDinFisier(const char* numeFisier)
{
    FILE* f = fopen(numeFisier,"r");
    Nod* stiva = NULL;

    while(!feof(f))
    {
        Masina m = citireMasinaDinFisier(f);
        if(m.id != -1)
        {
            pushStack(&stiva,m);
        }
    }
    fclose(f);

    return stiva;
}

void dezalocareStivaDeMasini(Nod** stiva)
{
    while(*stiva)
    {
        Masina m = popStack(stiva);
        free(m.model);
        free(m.numeSofer);
    }
}

int size(Nod* stiva)
{
    int size = 0;
    while(stiva)
    {
        size++;
        stiva = stiva->next;
    }
    return size;
}

//QUEUE
//putem reprezenta o coada prin LSI, LDI sau vector

struct NodDublu
{
    Masina info;
    struct NodDublu* next;
    struct NodDublu* prev;
};
typedef struct NodDublu NodDublu;

struct Coada
{
    NodDublu* fata;
    NodDublu* spate;
};
typedef struct Coada Coada;

void enqueue(Coada* coada,Masina masina)
{
    //adauga o masina in coada
    NodDublu* nou = malloc(sizeof(NodDublu));
    nou->info = masina;
    nou->next = NULL;
    nou->prev = coada->spate;

    if(coada->spate)
    {
        coada->spate->next = nou;
    }
    else
    {
        coada->fata = nou;
    }
    coada->spate = nou;
}

Masina dequeue(Coada* coada)
{
    //extrage o masina din coada
    if(coada->fata)
    {
        Masina m = coada->fata->info;
        NodDublu* tmp = coada->fata;
        coada->fata = coada->fata->next;
        free(tmp);

        if(coada->fata == NULL)
        {
            coada->spate = NULL;
        }
        else
        {
            coada->fata->prev = NULL;
        }
        return m;
    }
    else
    {
        Masina m;
        m.id = -1;
        return m;
    }
}

Coada citireCoadaDeMasiniDinFisier(const char* numeFisier)
{
    FILE* f = fopen(numeFisier,"r");
    Coada coada;
    coada.fata = NULL;
    coada.spate = NULL;

    while(!feof(f))
    {
        Masina m = citireMasinaDinFisier(f);
        if(m.id != -1)
        {
            enqueue(&coada,m);
        }
    }
    fclose(f);
    return coada;
}

void dezalocareCoadaDeMasini(Coada* coada)
{
    //sunt dezalocate toate masinile si coada de elemente
    while(coada->fata != NULL)
    {
        Masina m = dequeue(coada);

        free(m.model);
        free(m.numeSofer);
    }
    coada->spate = NULL;
}


//metode de procesare
Masina getMasinaByID(Nod** stiva,int id)
{
    Nod* tmp = NULL;
    Masina masinaCautata;

    while((*stiva) != NULL)
    {
        Masina m = popStack(stiva);

        if(m.id == id)
        {
            masinaCautata = m;
        }

        pushStack(&tmp,m);
    }

    while(tmp != NULL)
    {
        pushStack(stiva,popStack(&tmp));
    }

    return masinaCautata;
}

float calculeazaPretTotal(Nod** stiva)
{
    Nod* tmp = NULL;
    float suma = 0;

    while((*stiva) != NULL)
    {
        Masina m = popStack(stiva);
        suma += m.pret;
        pushStack(&tmp,m);
    }
    while(tmp != NULL)
    {
        pushStack(stiva,popStack(&tmp));
    }
    return suma;

}

int main( )
{
    Nod* capStiva;
    capStiva = citireStackMasiniDinFisier("masini.txt");
    printf("\n+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+\n");

    Coada capCoada = citireCoadaDeMasiniDinFisier("masini.txt");
    Masina primaMasina = dequeue(&capCoada);
    afisareMasina(primaMasina);
    free(primaMasina.model);
    free(primaMasina.numeSofer);

    float pretTotal = calculeazaPretTotal(&capStiva);

    printf("Pretul total este de : %.2F\n",pretTotal);

    Masina masinaGasita = getMasinaByID(&capStiva,4);
    if(masinaGasita.id != -1)
    {
        printf("\nMasina cautata dupa id:\n");
        afisareMasina(masinaGasita);
    }
    else
    {
        printf("\nNu exista masina cu id-ul cautat.\n");
    }
    dezalocareStivaDeMasini(&capStiva);
    dezalocareCoadaDeMasini(&capCoada);

    return 0;
}
