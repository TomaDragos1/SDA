/*Toma Mihai Dragos - 312CB*/

/*--- functiiLG.c -- operatii de baza pentru lista simplu inlantuita generica---*/
#include "tlg.h"

TLG Ins_IncLG(TLG* aL, void* name, TFcompare firs_list_cmp, TFaloc1 aloc1)
{
	TLG aux = aloc1(name);
	if(aux == NULL)
	    return NULL;
	// aux->urm = *aL;
	// *aL = aux;
	TLG l = *aL;
	TLG ant = NULL;
	for(;l != NULL ; ant = l , l = l->urm)
	{
		if(firs_list_cmp(aux->info, l->info) == 1) //aicea trb vazut
		{
			break;
		}
	}
	if(ant == NULL)
	{
		*aL = aux;
		aux->urm = l;
	}
	else
	{
		ant->urm= aux;
		aux->urm = l;
	}
	return aux;
}

void DistrugeLG(TLG* aL, TF free_elem) /* distruge lista */
{
	while(*aL != NULL)
    {
        TLG aux = *aL;     /* adresa celulei eliminate */
        if (!aux)
            return;

        free_elem(aux->info);  /* elib.spatiul ocupat de element*/
        *aL = aux->urm;    /* deconecteaza celula din lista */
        free(aux);   /* elibereaza spatiul ocupat de celula */
    }
}

size_t LungimeLG(TLG* a)      /* numarul de elemente din lista */
{
	size_t lg = 0;
	TLG p = *a;

     /* parcurge lista, numarand celulele */
	for (; p != NULL; p = p->urm)
        lg++;

	return lg;
}

void Afisare(TLG* aL, TF afiEL)
{
	if(!*aL) {
        printf("Lista vida\n");
        return;
    }

	printf("[\n");
	for(; *aL; aL = &(*aL)->urm) {
		afiEL((*aL)->info);
    }
	printf("]\n");
}
