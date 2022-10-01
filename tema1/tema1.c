/*Toma Mihai Dragos - 312CB*/

#include <stdio.h>
#include "tlg.h"
#include "thash.h"
#include <string.h>

typedef struct
{
	int number_letters;
	TLG head_list;
} TLetters;

typedef struct
{
	char *name;
	int frequency;
} TFrequency;

int codHash(void *element)
{
	char *name = (char *)element;
	if ('A' <= name[0] && name[0] <= 'Z')
		return *name - 'A';
	return *name - 'a';
}
//doar da cast si afiseaza numarul de litere din celula din prima lista
//se apeleaza pentru print la toata lista
void afisare_cel_number_letters(void *element)
{
	TLetters *el = (TLetters *)element;
	printf("(%d:", el->number_letters);
}
//functie de afisare pt afisarea mare si
//printeaza elmentele din sulbista
void afisare_element_second_list(void *element)
{
	TLetters *el = (TLetters *)element;
	TLG pars = el->head_list;
	TLG next = pars; //de la p
	for (; pars != NULL; pars = pars->urm)
	{
		TFrequency *info1 = (TFrequency *)(pars->info);
		next = pars->urm;
		if (next == NULL)
		{
			printf("%s/%d", info1->name, info1->frequency);
		}
		else
			printf("%s/%d, ", info1->name, info1->frequency);
	}
	printf(")");
}
//aloca celula pt sublista de info de tip TFrequency
TLG aloc_cel_second_list(void *name)
{
	TLG pars = NULL;
	pars = (TLG)malloc(sizeof(TCelulaG));
	if (pars == NULL)
		return NULL;
	TFrequency *crt_info = (TFrequency *)malloc(sizeof(TFrequency));
	if (crt_info == NULL)
	{
		free(pars);
		return NULL;
	}
	crt_info->frequency = 1;
	crt_info->name = (char *)malloc(30 * sizeof(char));
	if (crt_info->name == NULL)
	{
		free(crt_info);
		free(pars);
		return NULL;
	}
	strcpy(crt_info->name, (char *)name);
	pars->info = crt_info;
	pars->urm = NULL;
	return pars;
}

//fucntia o apele in insert_second_list si are rolul sa ferifice frecventele
//daca sunt egale adun frecvent si daca nu gasesc nici una egala
// atunci intorc 0 si aloca una noua in sublista
int compare_two_cels_second(void *el1, void *el2)
{
	TFrequency *element1 = (TFrequency *)el1;
	char *element2 = (char *)el2;
	if (strcmp(element1->name, element2) == 0)
	{
		element1->frequency++;
		return 1;
	}
	return 0;
}
//functie care imi compara strlen(cuv) cu info din celula din prima lista
//daca sunt egale stiu ca deja exita celula, asa cu nu mai aloca alta celula
int cmp_equal_first(void *el1, void *el2)
{
	TLetters *element1 = (TLetters *)el1;
	int number_let = strlen(el2);
	if (element1->number_letters == number_let)
	{
		return 1;
	}
	else
		return 0;
}
//functie de inserare orodnata in sublista
//sau reinserare pt cazul in care numele exista deja
void ord_frequence(TLG *al, TLG element_search)
{
	TLG l = *al;
	TLG ant = NULL;
	TFrequency *el_search = (TFrequency *)(element_search->info);
	for (; l != NULL; ant = l, l = l->urm)
	{
		TFrequency *el_crt = (TFrequency *)(l->info);
		int val_cmp = strcmp(el_search->name, el_crt->name);
		//verficia in cazul in care frecventa e egala si intoarce dupa strcmp
		if (el_search->frequency == el_crt->frequency && val_cmp < 0)
		{
			break;
		}
		//cazul in care frecventa e mai mare decat urmatoare
		//in caz in care strcmp nu intoarce nimic o sa insereze
		//la prima valoare mai mica intalnita
		else if (el_search->frequency > el_crt->frequency)
		{
			break;
		}
	}
	//dau break pt a insera la pozitia dorita

	//aici e inserarea normala
	if (ant == NULL)
	{
		*al = element_search;
		element_search->urm = l;
	}
	else
	{
		ant->urm = element_search;
		element_search->urm = l;
	}
}
//functia de insertia in sublista
//aici inserez si in caz daca celula din prima lista este nula
//si daca vreau sa inserez dupa o anumita ordine
int insert_second_list(TLG el, void *name)
{
	TLetters *info = (TLetters *)(el->info);
	int ok = 0;
	//cazul in care celula TLetters nu este initializata cu o sublista
	if (info->head_list == NULL)
	{
		TLG first_cel = aloc_cel_second_list(name);
		if (first_cel == NULL)
			return 0;
		info->head_list = first_cel;
	}
	//cazul in care este deja capul listei
	else
	{
		TLG aux = info->head_list;
		TLG ant = NULL;
		for (; aux != NULL && ok == 0; ant = aux, aux = aux->urm)
		{
			//aici compar in subliste sa vad daca cuvantul inserat exista deja
			//daca da atunci o sa imi elimin elemetul e tip TFrequence
			//si o sa il introduc din nou in lista prin ord_freq...
			if (compare_two_cels_second((aux)->info, name) == 1)
			{
				ok = 1;
				if (ant != NULL)
				{
					ant->urm = aux->urm;
					aux->urm = NULL;
				}
				else
				{
					info->head_list = aux->urm;
					aux->urm = NULL;
				}
				//functia de insare in ordinea dorita in sublista
				ord_frequence(&info->head_list, aux);
			}
		}
		if (ok == 0)
		{
			//cazul in care cunvatul nu exita in lista
			//add are rolul sa fie celula adaugata in caz ca nu exista in sublista
			TLG add = aloc_cel_second_list(name);
			if (add == NULL)
				return 0;
			ord_frequence(&info->head_list, add);
		}
	}
	return 1;
}
//are rolul de a compara doua celul din prima lista
//ma ajuta la inserarea crescatoare
int cmp_cresc_first(void *el1, void *el2)
{
	TLetters *element1 = (TLetters *)el1;
	TLetters *element2 = (TLetters *)el2;
	if (element1->number_letters < element2->number_letters)
		return 1;
	return 0;
}
//tot la cazul print frecventa
//aici verific daca fiecare sublista are frecventa dorita
//ma ajuta la afisare pentru a nu afisa poz: si ceva gol
int verif_cel(void *el_crt, int frequence)
{
	TLetters *element = (TLetters *)el_crt;
	TLG parcurgere = element->head_list;
	int contor = 0;
	for (; parcurgere != NULL; parcurgere = parcurgere->urm)
	{
		TFrequency *info = (TFrequency *)(parcurgere->info);
		if (info->frequency <= frequence)
			contor++;
	}
	if (contor == 0)
		return 0;
	return 1;
}

//funcite de afisare pentru print frecventa
void afisare_first_task(void *el_crt, int frequnece)
{
	TLetters *element = (TLetters *)el_crt;
	TLG parcurgere = element->head_list;
	TLG next = parcurgere;
	printf("(%d: ", element->number_letters);
	for (; parcurgere != NULL; parcurgere = parcurgere->urm)
	{
		TFrequency *info = (TFrequency *)(parcurgere->info);
		next = parcurgere->urm;
		if (info->frequency <= frequnece && next == NULL)
		{
			printf("%s/%d", info->name, info->frequency);
		}
		else if (info->frequency <= frequnece && next != NULL)
		{
			printf("%s/%d, ", info->name, info->frequency);
		}
	}
	printf(")");
}
//verificare pentru afisare 1 print frecventa
//am ferificat daca exist in lista mea frecventa dorita
//aceasta funcite ma ajuta la afisare
int list_verif(TLG first_list, int frequence)
{
	TLG l = first_list;
	int contor = 0;
	for (; l != NULL; l = l->urm)
	{
		TLetters *info1 = (TLetters *)(l->info);
		TLG par_sublist = info1->head_list;
		for (; par_sublist != NULL; par_sublist = par_sublist->urm)
		{
			TFrequency *info2 = (TFrequency *)(par_sublist->info);
			if (info2->frequency <= frequence)
				contor++;
		}
	}
	if (contor == 0)
		return 0;
	return 1;
}
//afisare pentru print litera frecventa
//l-am luat ca si caz 2
void afisare2(TLG el)
{
	TLetters *info = (TLetters *)(el->info);
	TLG l = info->head_list;
	TLG next = l;
	for (; l != NULL; l = l->urm)
	{
		TFrequency *info2 = (TFrequency *)(l->info);
		next = l->urm;
		if (next == NULL)
		{
			printf("%s/%d", info2->name, info2->frequency);
		}
		else
		{
			printf("%s/%d, ", info2->name, info2->frequency);
		}
	}
	printf(")");
}
//functie de comparere pentru a vedea daca doua celule din prima lista coincid
//daca da o sa apelez o functie de inserare in functii-tabHash afinsert
TLG verif_case_2(TLG list, int nr_let)
{
	TLG l = list;
	for (; l != NULL; l = l->urm)
	{
		TLetters *info = (TLetters *)(l->info);
		if (info->number_letters == nr_let)
			return l;
	}
	return NULL;
}

//fucntie de dezalocare pentru subliste
void eliberare_second_list(TLG element)
{
	TLetters *info1 = (TLetters *)((element)->info);
	TLG l = info1->head_list;
	TLG aux = NULL;
	for (; l != NULL;)
	{
		aux = l;
		l = l->urm;
		TFrequency *info2 = (TFrequency *)(aux->info);
		free(info2->name);
		free(aux->info);
		free(aux);
	}
	info1->head_list = NULL;
}

//functie de alocare pentru o celula din prima lista
TLG aloc1(void *elem)
{
	char *name = (char *)elem;
	TLG element = (TLG)malloc(sizeof(TCelulaG));
	if (element == NULL)
		return NULL;
	TLetters *info = (TLetters *)malloc(sizeof(TLetters));
	if (info == NULL)
	{
		free(element);
		return NULL;
	}
	info->number_letters = strlen(name);
	info->head_list = NULL;
	element->info = info;
	return element;
}

//functia unde initializez th si citesc toate informatiile
TH *GenerareHash(FILE *f)
{
	TH *h = NULL;
	size_t M = ('z' - 'a') + 1;
	int rez;
	h = (TH *)InitTH(M, codHash);
	if (h == NULL)
		return NULL;
	char *line = (char *)malloc(sizeof(char) * 1000);
	while (fgets(line, 1000, f))
	{
		char *word = strtok(line, " ,.\n\r");
		if (strcmp(word, "insert") == 0)
		{
			word = strtok(NULL, " ,.\n\r");
			while (word != NULL)
			{
				if (strlen(word) >= 3 && (word[0] > '9' || word[0] < '0'))
				{
					//functie de inserare al primului tip de lista si cel de al doilea
					rez = InsTH(h, cmp_equal_first, word, insert_second_list, cmp_cresc_first, aloc1);
					//in caz ca nu am memorie destula
					if (rez == 0)
					{
						DistrTH(&h, eliberare_second_list);
						return NULL;
					}
				}
				word = strtok(NULL, " ,.\n\r");
			}
		}
		else if (strcmp(word, "print") == 0)
		{
			//aici sa afla toate tipurile de afisari
			char *first_info;
			char *second_info;
			first_info = strtok(NULL, " ,.\n\r");
			second_info = strtok(NULL, " ,.\n\r");
			if (first_info == NULL && second_info == NULL)
			{
				//print normal pt intreaga lista
				AfiTH(h, afisare_cel_number_letters, afisare_element_second_list);
			}
			else if (first_info != NULL && second_info == NULL)
			{
				int frequnce = atoi(first_info);
				//print frequence
				AfiTH_case1(h, afisare_first_task, frequnce, verif_cel, list_verif);
			}
			else if (first_info != NULL && second_info != NULL)
			{
				//print letter frequence
				int nr_let = atoi(second_info);
				AfiTH_case2(h, first_info, nr_let, verif_case_2, afisare2);
			}
		}
	}
	free(line);
	fclose(f);
	return h;
}
int main(int argc, char *argv[])
{
	TH *h = NULL;
	FILE *input;
	input = fopen(argv[1], "r");
	h = GenerareHash(input);
	if (h == NULL)
	{
		printf("Tabela hash nu a putut fi generata\n");
		return 0;
	}
	//functie de dezalocare
	DistrTH(&h, eliberare_second_list);
	return 0;
}
