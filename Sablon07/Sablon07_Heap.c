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

//creare structura pentru Heap
//un vector de elemente, lungimea vectorului si numarul de elemente din vector
struct Heap
{
	int lungime;
	Masina* vector;
	int nrMasini;
};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime)
{
	Heap heap;
	heap.lungime = lungime;
	heap.nrMasini = 0;
	heap.vector = (Masina*)malloc(sizeof(Masina) * lungime);

	return heap;
}

void filtreazaHeap(Heap heap,int pozitieNod)
{
	//filtreaza heap-ul pentru nodul a carei pozitie o primeste ca parametru

	int poz_fiu_stanga = 2 * pozitieNod + 1;
	int poz_fiu_dreapta = 2 * pozitieNod + 2;
	int poz_max = pozitieNod;

	if(poz_fiu_stanga < heap.nrMasini && heap.vector[poz_max].pret < heap.vector[poz_fiu_stanga].pret)
	{
		poz_max = poz_fiu_stanga;
	}

	if(poz_fiu_dreapta < heap.nrMasini && heap.vector[poz_max].pret < heap.vector[poz_fiu_dreapta].pret)
	{
		poz_max = poz_fiu_dreapta;
	}

	else if ( poz_max!=pozitieNod )
	{
		Masina aux = heap.vector[poz_max];
		heap.vector[poz_max] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		if(poz_max <= (heap.nrMasini - 2) / 2)
		{
			filtreazaHeap(heap,poz_max);
	 }
	}
	
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier)
{
	//citim toate masinile din fisier si le stocam intr-un heap 
	// pe care trebuie sa il filtram astfel incat sa respecte 
	// principiul de MAX-HEAP sau MIN-HEAP dupa un anumit criteriu
	// sunt citite toate elementele si abia apoi este filtrat vectorul

	FILE* f = fopen(numeFisier,"r");
	Heap heap = initializareHeap(10);
	char buffer[100];
	while(fgets(buffer,sizeof(buffer),stdin))
	{
		heap.vector[heap.nrMasini++] = citireMasinaDinFisier(f);
	}
	fclose(f);

	for(int i = (heap.nrMasini - 2) / 2;i >= 0;i++)
	{
		filtreazaHeap(heap,i);
	}
	return heap;
}

void afisareHeap(Heap heap)
{
	//afiseaza elementele vizibile din heap
	for(int i = 0;i < heap.nrMasini;i++)
	{
		afisareMasina(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap)
{
	//afiseaza elementele ascunse din heap
	for(int i = heap.nrMasini;i < heap.lungime;i++)
	{
		afisareMasina(heap.vector[i]);
	}
}

Masina extrageMasina(Heap* heap)
{
	//extrage si returneaza masina de pe prima pozitie
	//elementul extras nu il stergem...doar il ascundem

	Masina masinaInvalida = { 0, 0, 0, NULL, NULL, 0 };

	if(heap->nrMasini > 0)
	{
		Masina masina = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrMasini - 1];
		heap->vector[heap->nrMasini - 1] = masina;
		heap->nrMasini--;

		for(int i = (heap->nrMasini - 2) / 2;i >= 0;i--)
		{
			filtreazaHeap(*heap, i);
		}
		return masina;
	}

	return masinaInvalida;
}


void dezalocareHeap(Heap* heap)
{
	for(int i = 0;i < heap->lungime;i++)
	{
		free(heap->vector[i].numeSofer);
		free(heap->vector[i].model);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrMasini = 0;
}

int main( )
{
	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(heap);

	printf("masini extrase: \n");
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));
	

	printf("Heap-ul ascuns este :\n");
	afiseazaHeapAscuns(heap);

	dezalocareHeap(&heap);
	return 0;
}
