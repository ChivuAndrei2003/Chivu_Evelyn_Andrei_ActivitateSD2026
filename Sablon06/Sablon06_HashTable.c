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

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod
{
    Masina info;
    Nod* next;

};
typedef struct Nod Nod;

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste
struct HashTable
{
    int dim;
    Nod** table;
};
typedef struct HashTable HashTable;

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

void afisareListaMasini(Nod* masina)
{
    //afiseaza toate elemente de tip masina din lista dublu inlantuita
    //prin apelarea functiei afisareMasina()
    while(masina)
    {
        afisareMasina(masina->info);
        masina = masina->next;
    }
}

void adaugaMasinaInLista(Nod* head,Masina masinaNoua)
{
    //adauga la final in lista primita o noua masina pe care o primim ca parametru
    Nod* cap = head;
    while(cap->next)
    {
        cap = cap->next;
    }

    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = NULL;

    cap->next = nou;
}


HashTable initializareHashTable(int dimensiune)
{
    
    HashTable hash_table;
    hash_table.dim = dimensiune;
    hash_table.table =(Nod**)calloc(dimensiune,sizeof(Nod*));

    return hash_table;
}

int calculeazaHash(char* key,int dimensiune)
{
    // este calculat hash-ul in functie de dimensiunea tabelei si un atribut al masinii
    int sum = 0;
    for(int i = 0; key[i] ;i++)
    {
        sum += key[i];
    }
    return sum % dimensiune;
}

void inserareMasinaInTabela(HashTable hash,Masina galerie)
{
    //este folosit mecanismul CHAINING
    //este determinata pozitia si se realizeaza inserarea pe pozitia respectiva
    int poz = calculeazaHash(galerie.model,sizeof(hash.dim));

    if(hash.table[poz] == NULL)
    {
        hash.table[poz] = (Nod*)malloc(sizeof(Nod));
        hash.table[poz]->info = galerie;
        hash.table[poz]->next = NULL;
    }
    else
    {
        adaugaMasinaInLista(hash.table[poz], galerie);
        
    }
}

HashTable citireMasiniDinFisier(const char* numeFisier,int dimensiune)
{
    //functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
    //prin apelul repetat al functiei citireMasinaDinFisier()
    // aceste masini sunt inserate intr-o tabela de dispersie initializata aici
    //ATENTIE - la final inchidem fisierul/stream-ul

    HashTable hash=initializareHashTable(dimensiune);
    FILE* f = fopen(numeFisier,"r");

    char buffer[100];
    while(!feof(f))
    {
        Masina masina = citireMasinaDinFisier(f);
        inserareMasinaInTabela(masina);
    }
    fclose(f);

    return hash;

}

void afisareTabelaDeMasini(HashTable ht)
{
    //sunt afisate toate masinile cu evidentierea clusterelor realizate
    for(int i = 0;i < ht.dim;i++)
    {
        if(ht.table[i] != NULL)
        {
            printf("Pe pozitia %d avem urmatoarele masin:\n",i);
            afisareListaMasini(ht.table[i]);
        }
        else
        {
            printf("Nu exista masini pe pozitia %d", i);
        }
    }
}

void dezalocareListaMasini(Nod** cap){
    
    Nod* head = *cap;
    while(head){
        Nod* temp =head;
        head= head->next;
        if(temp->masina.model != NULL ){
            free(temp->masina.model);
        }
        if(temp->masina.numeSofer != NULL ){
            free(temp->masina.numeSofer);
        }
        free(temp);
    }
    *cap=NULL;
}
void dezalocareTabelaDeMasini(HashTable* ht)
{
    //sunt dezalocate toate masinile din tabela de dispersie
    
    for(int i=0;i<ht.dim;i++){
        if(ht.table[i] != NULL){
            free(ht.)
        }
    }
}

float* calculeazaPreturiMediiPerClustere(HashTable ht,int* nrClustere)
{
    //calculeaza pretul mediu al masinilor din fiecare cluster.
    //trebuie sa returnam un vector cu valorile medii per cluster.
    //lungimea vectorului este data de numarul de clustere care contin masini
    return NULL;
}

Masina getMasinaDupaCheie(HashTable ht /*valoarea pentru masina cautata*/)
{
    Masina m;
    //cauta masina dupa valoarea atributului cheie folosit in calcularea hash-ului
    //trebuie sa modificam numele functiei 
    return m;
}

int main( )
{


    return 0;
}