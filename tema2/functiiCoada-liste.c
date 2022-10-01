/* Toma Mihai Dragos - 312CB */
#include "TCoada-liste.h"

int VidaQ(void *a)
{
	TCoada *c = (TCoada *)a;
	if (c->ic == NULL && c->sc == NULL)
		return 1;
	return 0;
}

void *InitQ(size_t d)
{
	AQ elemet = calloc(1, sizeof(TCoada));
	if (elemet == NULL)
		return NULL;
	elemet->dime = d;
	elemet->ic = NULL;
	elemet->sc = NULL;
	return (void *)elemet;
}

int ExtrQ(void *a, void *ae)
{
	AcelQ new_element = NULL;
	TCoada *c = (TCoada *)a;
	if (c->ic == c->sc)
	{
		c->sc = NULL;
	}
	if (VidaQ(a))
		return 0;
	else
	{
		new_element = c->ic;
		c->ic = new_element->urm;
		memmove(ae, new_element->info, c->dime);
		free(new_element->info);
		free(new_element);
	}
	return 1;
}

int IntrQ(void *a, void *ae)
{
	AcelQ new_element = calloc(1, sizeof(TcelQ));
	if (new_element == NULL)
		return 0;
	TCoada *c = (TCoada *)a;
	new_element->info = calloc(1, c->dime);
	if (new_element->info == NULL)
	{
		free(new_element);
		return 0;
	}
	memmove(new_element->info, ae, c->dime);
	new_element->urm = NULL;
	if (c->ic == NULL && c->sc == NULL)
	{
		c->ic = new_element;
		c->sc = new_element;
	}
	else
	{
		c->sc->urm = new_element;
		c->sc = new_element;
	}
	return 1;
}

void ResetQ(void *a)
{
	AcelQ p;
	TCoada *c = (TCoada *)a;
	p = c->ic;
	while (p != NULL)
	{
		AcelQ aux = p;
		p = aux->urm;
		free(aux->info);
		free(aux);
	}
	c->ic = NULL;
	c->sc = c->ic;
}

void DistrQ(void **aa)
{
	ResetQ(*aa);
	free(*aa);
}

int ConcatQ(void *d, void *s)
{
	TCoada *dest = (TCoada *)d;
	TCoada *surs = (TCoada *)s;
	if (VidaQ(s))
		return 0;
	if (VidaQ(d))
	{
		dest->ic = surs->ic;
		dest->sc = surs->sc;
		surs->ic = NULL;
		surs->sc = NULL;
	}
	else
	{
		dest->sc->urm = surs->ic;
		dest->sc = surs->sc;
		surs->ic = NULL;
		surs->sc = NULL;
	}
	return 1;
}