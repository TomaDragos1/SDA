/* Toma Mihai Dragos - 312CB */
#include "TCoada.h"

typedef struct cel
{
	void *info;
	struct cel *urm;

} TcelQ, *AcelQ;

typedef struct
{
	size_t dime; /* dimensiunea unui element din coada  		*/
	AcelQ ic, sc;
} TCoada, *AQ;

