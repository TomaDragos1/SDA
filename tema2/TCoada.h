/* Toma Mihai Dragos - 312CB */
/*-- TCoada.h --- tipul coada ---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

void *InitQ(size_t a);         /* creeaza coada vida cu elemente de dimensiune d;*/
int IntrQ(void *a, void *ae);  /* adauga element la sfarsitul cozii */
int ExtrQ(void *a, void *ae);  /* extrage primul element din coada la adresa ae */
int VidaQ(void *a);            /* test coada vida */
void ResetQ(void *a);          /* transforma coada in coada vida */
void DistrQ(void **aa);        /* elibereaza intregul spatiu ocupat de coada */
int ConcatQ(void *d, void *s); /* concatenare (muta toate elem.din sursa) */
