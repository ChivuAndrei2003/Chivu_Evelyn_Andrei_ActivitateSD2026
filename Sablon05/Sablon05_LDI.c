#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod
{
    Masina info;
    struct Nod* next;
    struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla
{
    Nod* first;
    Nod* last;
    
};
typedef struct ListaDubla ListaDubla;


Masina citireMasinaDinFisier(FILE* file)
{
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(ListaDubla* lista) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
    //prin apelarea functiei afisareMasina()
    Nod* p = lista->first;
    while(p != NULL)
    {
        afisareMasina(p->info);
        p=p->next;
    }
}
 

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
    //adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
    Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->info = masinaNoua;
    nodNou->prev = NULL;
    nodNou->next = lista->first;
    if(lista->first != NULL)
    {
        lista->first->prev = nodNou;
    }
    else
    {
        lista->last = nodNou;
    }
    lista->first = nodNou;

}

 /* ADAUGARE LA SFARSIT DE LISTA   */
void adaugaLaSfarsitInLista(ListaDubla* lista, Masina masinaNoua) {
    //adauga la final in lista primita o noua masina pe care o primim ca parametru

    Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->info = masinaNoua;
    nodNou->next = NULL;
    nodNou->prev = lista->last;
    if(lista->last != NULL)
    {
        lista->last->next = nodNou;
    }
    else
    {
        lista->first = nodNou;
    }
    lista->last = nodNou;

}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
    //ATENTIE - la final inchidem fisierul/stream-ul

    FILE* f= fopen(numeFisier,"r");

    ListaDubla lista;

    lista.first = NULL;
    lista.last = NULL;
    char buffer[200];
    
    while(fgets(buffer,100,f) != NULL)
    {
        Masina m = citireMasinaDinFisier(f);
        adaugaLaInceputInLista(&lista, m );
    }
    fclose(f);

    return lista;

}

void dezalocareLDMasini(ListaDubla* lista) {
    //sunt dezalocate toate masinile si lista dublu inlantuita de elemente
    while(lista->first != NULL)
    {
        Nod* aux = lista->first;
        free(aux->info.numeSofer);
        free(aux->info.model);
        lista->first = lista->first->next;
        free(aux);
    }
    lista->last = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
    //calculeaza pretul mediu al masinilor din lista.
    float sum = 0;
    int k = 0;
    
    while(lista.first != NULL)
    {
        sum += lista.first->info.pret;
        k++;
        lista.first = lista.first->next;

    }
    return (sum > 0) ? sum / k : 0;
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
	//sterge masina cu id-ul primit.
    //tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie

    Nod* p = lista->first;

    while(p != NULL && p->info.id != id)
    {
        p = p->next;
    }
    if(p != NULL)
    {
        if(p->prev != NULL)
        {
            p->prev->next = p->next;
        }
        else
        {
            lista->first = p->next;
        }
        if(p->next != NULL)
        {
            p->next->prev = p->prev;
        }
        else
        {
            lista->last = p->prev;
        }
        free(p->info.model);
        free(p->info.numeSofer);
        free(p);
    }


    
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	//cauta masina cea mai scumpa si 
    //returneaza numele soferului acestei maasini.

    if(lista.first == NULL)
    {
        return NULL;
    }

    Nod* maxim = lista.first;
    lista.first = lista.first->next;

    while(lista.first != NULL)
    {
        if(lista.first->info.pret > maxim->info.pret)
        {
            maxim = lista.first;
            
        }
        lista.first = lista.first->next;


        
    }
    char* numeSofer = malloc(strlen(maxim->info.numeSofer) + 1);
    strcpy(numeSofer,maxim->info.numeSofer);
    return numeSofer;

}

int main() {


    ListaDubla LISTA_DUBLA = citireLDMasiniDinFisier("masini.txt");
    afisareListaMasini(&LISTA_DUBLA);

    printf("\n/**======================================**/");

    float pret_mediu = calculeazaPretMediu(LISTA_DUBLA);
    printf("\n Pretul mediu al masinilor este : %.2F",pret_mediu);

    char* nume = getNumeSoferMasinaScumpa(LISTA_DUBLA);
    printf("Numele soferului cu cea mai scumpa masina este : %s",nume);
    if( nume != NULL )
    {
        free(nume);
    }
    dezalocareLDMasini(&LISTA_DUBLA);
    return 0;
}