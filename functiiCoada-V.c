//Preda Ioan Alexandru-313CB
#include "TCoada.h"
#include "TStiva.h"
/* FUNCTII COADA */


/* initiaza coada goala -> coada goala */
void *InitQ(size_t d,...) 
{
	AQ new = (AQ)malloc(sizeof(TCoada));
	if (!new) return NULL;
	new->ic=NULL;
	new->sc=NULL;
	new->dime=d;
	return (void *)new;
}


/* adauga elemtul x la sfarsitul cozii */
void IntrQ(void *c, void *x)
{
	ACel new = (ACel)malloc(sizeof(TCel));
	if(!new) return;
	new->info = (void *)malloc(sizeof(Task));
	if(!(new->info)) return; 
	memcpy(new->info, x, sizeof(Task));
	Task *y = (Task *)new->info;
	
	new->urm = NULL;
	if(VidaQ(c))
		IC(c) = SC(c) = new;
	else
	{
		((AQ)c)->sc->urm = new;
		((AQ)c)->sc = new;
	}
}
/* scoate un element din varful cozii*/
void ExtrQ(void *a, void **ae)
{
	if(VidaQ(a)) return;
	ACel aux = ((AQ)a)->ic;
	((AQ)a)->ic =aux->urm;
    Task *x = (Task *)aux->info;
	memcpy(*ae, aux->info, DIMEQ(a));
	free(aux->info);
	free(aux); 

	if(((AQ)a)->ic == NULL) ((AQ)a)->sc = NULL;
}
/* functie de elberare a spatiului ocupat de lista*/
void DistrugeLGQ(ACel *L, void(*freec)(void *))
{
	ACel aux=NULL, p;
	p=*L;
	while(p)
	{
		aux=p;
		p=p->urm;
		freec(aux->info);
		free(aux);
	}
	*L=NULL;
}

void ResetQ(void *c, void(*dis)(ACel *, void(*freec)(void *) ), void(*freec)(void *))
{
	ACel aux = (ACel)IC(c);
	((AQ)c)->ic = NULL;
	((AQ)c)->sc = NULL;
	DistrugeLGQ(&aux, freec);
}

void DistrQ(void *c, void(*dis)(ACel *, void(*freec)(void *) ), void(*freec)(void *))
{
	ResetQ(c, dis, freec);
	free(c);
}

/*afiseaza elementele cozii*/
void AfisQ(void *c, void (*afiEl)(void *, FILE *), FILE *out)
{
	void *aux = InitQ(DIMEQ(c));
	void *el = malloc(DIMEQ(c));

	while(!VidaQ(c)) 
	{
		ExtrQ(c, el);
		afiEl(el, out);
		IntrQ(aux, el);
	}
	while(!VidaQ(aux))
	{
		ExtrQ(aux, el);
		IntrQ(c, el);
	}
	free(el);
	free(aux);
}
void ConcatQ(void *dest, void *sursa) 
{
    if(!IC(dest) && !SC(dest)) 
    {
        ((TCoada *)dest)->ic = ((TCoada *)sursa)->ic;
        ((TCoada *)dest)->sc = ((TCoada *)sursa)->sc;
        ((TCoada *)sursa)->ic = ((TCoada *)sursa)->sc = NULL;
    }
    if(!IC(sursa) && !SC(sursa)) return;
    if(IC(sursa) && SC(sursa) && 
        IC(dest) && SC(dest)) 
    {
        ((TCoada *)dest)->sc->urm = ((TCoada *)sursa)->ic;
        ((TCoada *)dest)->sc = ((TCoada *)sursa)->ic;

        ((TCoada *)sursa)->ic = ((TCoada *)sursa)->sc = NULL;
    }
}