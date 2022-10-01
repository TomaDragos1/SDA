/*Toma Mihai Dragos - 312CB*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "tlg.h"

#ifndef _TAB_HASH_
#define _TAB_HASH_

typedef int (*TFElem)(void *);        /* functie prelucrare element */
typedef int (*TFCmp)(void *, void *); /* functie de comparare doua elemente
(dupa numarul de litere din prima lista) si vede daca sunt egale */
typedef void (*TF)(void *);           /* functie afisare/eliberare un element */
typedef int (*TFHash)(void *);
typedef void(TFsecond)(void *);             /*afisare a doua lista*/
typedef int (*TFinsertsec)(TLG, void *);    /*functie de inserare ordonata conform cerintei in cea de a doua lista*/
typedef int (*TFcompare)(void *, void *);   /*o functie care ma ajuta sa compar 
numarul de litere din fiecare cel din prima lista pt a aranaj crescator prima lista*/
typedef void (*TF_first_task)(void *, int); /*afisare pentru print nr pt fiecare cel*/
typedef int (*TFverif)(void *, int);        /*veriica daca sublista pentru frecventa mea exista */
typedef int (*TFlist_ver)(TLG, int);        /*verifica daca intreaga lista are frecventa mea*/
typedef void (*TAfis2)(TLG);                /*functie de afisare pt print litera frec*/
typedef TLG (*Tverif2)(TLG, int);           /*asta imi verifica doar o sublista pt ce un caz mai mic*/
typedef void (*TFdezaloc)(TLG);             /*functie dezalocare sublista*/
typedef TLG (*TFaloc1)(void *);             /*aloca o celula din prima lista pt (void *) */

typedef struct
{
    size_t M;
    TFHash fh;
    TLG *v; // lista de hash in care pui pui prima litera
} TH;

/* functii tabela hash */
TH *InitTH(size_t M, TFHash fh);
void DistrTH(TH **aa, TFdezaloc elib_el);
void AfiTH(TH *a, TF afiEl, TFsecond afiEl2);
int InsTH(TH *a, TFCmp f, void *name, TFinsertsec afinsert, TFcompare irs_list_cmp, TFaloc1 aloc1);
void AfiTH_case1(TH *a, TF_first_task afisare_first_task, int frequence, TFverif verif_cel, TFlist_ver list_ver);
void AfiTH_case2(TH *a, char *frst_let, int nmbr_let, Tverif2 verif2, TAfis2 afisare2);

#endif