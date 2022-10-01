/* Toma Mihai Dragos - 312CB */
#include "tarb.h"

//alocare
Tarbfile aloc_file(char *nume, Tarbdir crt_dir)
{
    Tarbfile new = calloc(1, sizeof(Tfile));
    if (new == NULL)
        return NULL;
    new->nume = malloc(strlen(nume) + 1);
    if (new->nume == NULL)
    {
        free(new);
        return NULL;
    }
    strcpy(new->nume, nume);
    new->parinte = crt_dir;
    return new;
}

int insert_file(Tarbdir *crt_dir, char *nume)
{
    Tarbfile *file_rad, n, p, aux;
    //aici tin minte file
    file_rad = &((*crt_dir)->files);
    if (*file_rad == NULL)
    {
        *file_rad = aloc_file(nume, *crt_dir);
        if (*file_rad == NULL)
        {
            return 0;
        }
        return 1;
    }
    n = *file_rad;
    //caut unde sa inserez
    while (n)
    {
        p = n;
        if (strcmp(nume, n->nume) == 0)
        {
            printf("File %s already exists!\n", nume);
            return 2;
        }
        if (strcmp(nume, n->nume) < 0)
            n = n->st;
        else
            n = n->dr;
    }
    //inserez
    aux = aloc_file(nume, *crt_dir);
    if (aux == NULL)
        return 0;
    if (strcmp(nume, p->nume) < 0)
        p->st = aux;
    else
        p->dr = aux;

    return 1;
}

//afisare inordine
void afisare_fisier(Tarbfile crt_file_rad)
{
    if (crt_file_rad == NULL)
        return;
    afisare_fisier(crt_file_rad->st);
    printf("%s ", crt_file_rad->nume);
    afisare_fisier(crt_file_rad->dr);
}

//verific sa vad daca exista
int verif_fis_exist(Tarbfile crt_file, char *nume)
{
    if (crt_file == NULL)
        return 0;
    if (strcmp(crt_file->nume, nume) == 0)
        return 1;
    if (strcmp(crt_file->nume, nume) > 0)
        return verif_fis_exist(crt_file->st, nume);
    return verif_fis_exist(crt_file->dr, nume);
}

//dau delete la un element
void delete_file(Tarbfile *crt_file, char *nume)
{
    if (*crt_file == NULL)
        return;
    else if (strcmp(nume, (*crt_file)->nume) < 0)
        delete_file(&(*crt_file)->st, nume);
    else if (strcmp(nume, (*crt_file)->nume) > 0)
        delete_file(&(*crt_file)->dr, nume);
    else
    {
        //frunza
        if (!(*crt_file)->st && !(*crt_file)->dr)
        {
            free((*crt_file)->nume);
            free((*crt_file));
            (*crt_file) = NULL;
        }
          //copil stanga
        else if (!(*crt_file)->dr)
        {
            Tarbfile temp = (*crt_file);
            (*crt_file) = (*crt_file)->st;
            free(temp->nume);
            free(temp);
        }
        //copil dreapta
        else if (!(*crt_file)->st)
        {
            Tarbfile temp = (*crt_file);
            (*crt_file) = (*crt_file)->dr;
            free(temp->nume);
            free(temp);
        }
        //ordine 2
        else
        {
            Tarbfile temp = (*crt_file);
            temp = temp->st;
            //caz particular ma duc in stanga si in dreapta nu e nimic
            if (temp->dr == NULL)
            {
                Tarbfile temp2 = temp->st;
                strcpy((*crt_file)->nume, temp->nume);
                (*crt_file)->parinte = temp->parinte;
                free(temp->nume);
                free(temp);
                (*crt_file)->st = temp2;
            }
            //cazul general din curs
            else
            {
                Tarbfile parinte_aux;
                while (temp != NULL)
                {
                    parinte_aux = temp;
                    temp = temp->dr;
                }
                Tarbfile temp2 = temp->st;
                strcpy((*crt_file)->nume, temp->nume);
                (*crt_file)->parinte = temp->parinte;
                free(temp->nume);
                free(temp);
                parinte_aux->dr = temp2;
            }
        }
    }
}

//delete recusiv la arbore file
void eliberare_file(Tarbfile delete_file)
{
    if (delete_file == NULL)
        return;
    eliberare_file(delete_file->st);
    eliberare_file(delete_file->dr);
    free(delete_file->nume);
    free(delete_file);
}

//cautare recursiva de file
Tarbdir caut_file(Tarbdir crt_dir, char *nume, Tarbdir aux)
{
    if (crt_dir == NULL)
        return aux;
    aux = caut_file(crt_dir->st, nume, aux);
    aux = caut_file(crt_dir->dr, nume, aux);

    int rez = verif_fis_exist(crt_dir->files, nume);
    if (rez == 1)
    {
        return crt_dir;
    }
    aux = caut_file(crt_dir->subdir, nume, aux);
    return aux;
}