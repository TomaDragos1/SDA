/* Toma Mihai Dragos - 312CB */
#include "TStiva.h"

typedef struct cel
{
	void *info;
	struct cel *urm;

} TcelST, *AcelST;

typedef struct stiva
{ 
	size_t 	dime; 		/* dimensiunea unui element al stivei 			*/
	AcelST vf;
} TStiva, *ASt;

