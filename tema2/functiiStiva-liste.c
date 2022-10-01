/* Toma Mihai Dragos - 312CB */
#include "TStiva-liste.h"

int VidaS(void *a)
{
	TStiva *s = (TStiva *)a;
	if (s->vf == NULL)
		return 1;
	return 0;
}
void *InitS(size_t d)
{
	ASt s;
	s = calloc(1, sizeof(TStiva));
	if (!s)
		return NULL;
	s->dime = d;
	s->vf = NULL;
	return (void *)s;
}

int Push(void *a, void *ae)
{
	AcelST aux = (AcelST)malloc(sizeof(TcelST));
	if (aux == NULL)
		return 0;
	TStiva *s = (TStiva *)a;
	aux->info = malloc((s->dime));
	if(aux->info == NULL)
	{
		free(aux);
		return 0;
	}
	memcpy(aux->info, ae, s->dime);
	aux->urm = s->vf;
	s->vf = aux;
	return 1;
}

int Pop(void *a, void *ae)
{
	TStiva *s = (TStiva *)a;
	if (VidaS(s) == 1)
		return 0;
	memcpy(ae, s->vf->info, s->dime);
	AcelST aux = s->vf;
	s->vf = aux->urm;
	free(aux->info);
	free(aux);
	return 1;
}

int Top(void *a, void *ae)
{
	TStiva *s = (TStiva *)a;
	if (s->vf == NULL)
		return 0;
	memcpy(ae, s->vf->info, s->dime);
	return 1;
}

void ResetS(void *a)
{
	TStiva *s = (TStiva *)a;
	AcelST l = s->vf;
	while (l != NULL)
	{
		AcelST aux = l;
		free(aux->info);
		l = aux->urm;
		free(aux);
	}
	s->vf = NULL;
}
void DistrS(void **aa)
{
	ResetS(*aa);
	free(*aa);
}

int DimeS(void *c)
{
	TStiva *s = (TStiva *)c;
	AcelST l = s->vf;
	int contor = 0;
	while (l != NULL)
	{
		contor++;
		l = l->urm;
	}
	return contor;
}

int Rastoarna(void *d, void *s)
{
	TStiva *dest = (TStiva *)d;
	TStiva *surs = (TStiva *)s;
	if (s == NULL)
		return 0;
	if (dest->dime != surs->dime)
		return 0;
	AcelST ld = dest->vf;
	AcelST ls = surs->vf;
	AcelST aux;
	while (ls)
	{
		aux = ls;
		ls = ls->urm;
		aux->urm = ld;
		ld = aux;
	}
	surs->vf = NULL;
	dest->vf = ld;
	return 1;
}

int Suprapunere(void *ad, void *as)
{
	TStiva *dest = (TStiva *)ad;
	TStiva *surs = (TStiva *)as;
	if (dest->dime != surs->dime)
	{
		return 0;
	}
	if (!VidaS(surs) && VidaS(dest))
	{
		dest->vf = surs->vf;
		surs->vf = NULL;
	}
	else if (!VidaS(surs))
	{
		AcelST u = surs->vf;
		while (u->urm != NULL)
			u = u->urm;

		u->urm = dest->vf;
		dest->vf = surs->vf;
		surs->vf = NULL;
	}
	return 1;
}