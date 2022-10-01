/* Toma Mihai Dragos - 312CB */
/*-- TStiva.h --- tipul stiva ---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>


void* InitS(size_t d);    /* initializeaza stiva cu elem.de dimensiune d;
				  					anumite implementari pot necesita si alti parametri */
int Push(void* a, void* ae);  /* pune element in varful stivei */
int Pop (void* a, void* ae);  /* extrage elementul din varful stivei la adresa ae */
int Top (void* a, void* ae);  /* copiaza elementul din varful stivei la adresa ae */
int VidaS(void* a);           /* test stiva vida */
void ResetS (void* a);        /* transforma stiva in stiva vida */
void DistrS (void** aa);      /* elibereaza intregul spatiu ocupat de stiva */
int Suprapunere(void* ad, void* as); /* muta stiva sursa peste destinatie */
int Rastoarna(void* ad, void* as); /* suprapune inversul sursei peste destinatie */