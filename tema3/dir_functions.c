/* Toma Mihai Dragos - 312CB */
#include "tarb.h"

//alocare dir
Tarbdir aloc_dir(char *nume, Tarbdir crt_dir)
{
    Tarbdir new = calloc(1, sizeof(Tdir));
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

//inserez in dir(seamana cu file)
int insert_director(Tarbdir *crt_dir, char *nume)
{
    Tarbdir *dir_rad, n, p, aux;
    //pointerul magic
    dir_rad = &((*crt_dir)->subdir);
    if (*dir_rad == NULL)
    {
        *dir_rad = aloc_dir(nume, *crt_dir);
        if (*dir_rad == NULL)
        {
            return 0;
        }
        return 1;
    }
    n = *dir_rad;
    //caut loc inserare
    while (n)
    {
        p = n;
        if (strcmp(nume, n->nume) == 0)
        {
            printf("Directory %s already exists!\n", nume);
            return 2;
        }
        if (strcmp(nume, n->nume) < 0)
            n = n->st;
        else
            n = n->dr;
    }
    //inserez
    aux = aloc_dir(nume, *crt_dir);
    if (!aux)
        return 0;
    if (strcmp(nume, p->nume) < 0)
        p->st = aux;
    else
        p->dr = aux;
    return 1;
}

//afisez dir in inordine
void afisare_dir(Tarbdir crt_dir_rad)
{
    if (crt_dir_rad == NULL)
        return;
    afisare_dir(crt_dir_rad->st);
    printf("%s ", crt_dir_rad->nume);
    afisare_dir(crt_dir_rad->dr);
}

//verific daca dir exista in director
int verif_dir_exist(Tarbdir crt_subdir, char *nume)
{
    if (crt_subdir == NULL)
        return 0;
    if (strcmp(crt_subdir->nume, nume) == 0)
        return 1;
    if (strcmp(crt_subdir->nume, nume) > 0)
        return verif_dir_exist(crt_subdir->st, nume);
    return verif_dir_exist(crt_subdir->dr, nume);
}

//sterg recursiv toata ierarhia de directoare si file-uri
//parcurg si fiecare director recursiv
void eliberare_dir(Tarbdir delete_dir)
{
    if (delete_dir == NULL)
        return;
    eliberare_dir(delete_dir->st);
    eliberare_dir(delete_dir->dr);
    eliberare_file(delete_dir->files);
    eliberare_dir(delete_dir->subdir);
    free(delete_dir->nume);
    free(delete_dir);
}

//dau delete la un anumit director rmdir
void delete_dir(Tarbdir *crt_dir, char *nume)
{
    if ((*crt_dir) == NULL)
        return;
    else if (strcmp(nume, (*crt_dir)->nume) < 0)
        delete_dir(&(*crt_dir)->st, nume);
    else if (strcmp(nume, (*crt_dir)->nume) > 0)
        delete_dir(&(*crt_dir)->dr, nume);
    else
    {
        //frunza
        if (!(*crt_dir)->st && !(*crt_dir)->dr)
        {
            eliberare_dir((*crt_dir)->subdir);
            eliberare_file((*crt_dir)->files);
            free((*crt_dir)->nume);
            free((*crt_dir));
            (*crt_dir) = NULL;
        }
        //copil stanga
        else if (!(*crt_dir)->dr)
        {
            Tarbdir temp = (*crt_dir);
            (*crt_dir) = (*crt_dir)->st;
            eliberare_dir(temp->subdir);
            eliberare_file(temp->files);
            free(temp->nume);
            free(temp);
            temp = NULL;
        }
        //copil dreapta
        else if (!(*crt_dir)->st)
        {
            Tarbdir temp = (*crt_dir);
            (*crt_dir) = (*crt_dir)->dr;
            eliberare_dir(temp->subdir);
            eliberare_file(temp->files);
            free(temp->nume);
            free(temp);
            temp = NULL;
        }
        //doi copii
        else
        {
            Tarbdir temp = (*crt_dir);
            temp = temp->st;
            //caz particular ma duc stanga si dupa nu are in dreapta copil
            if (temp->dr == NULL)
            {
                Tarbdir temp2 = temp->st;
                strcpy((*crt_dir)->nume, temp->nume);
                (*crt_dir)->parinte = temp->parinte;
                (*crt_dir)->files = temp->files;
                (*crt_dir)->subdir = temp->subdir;
                eliberare_dir(temp->subdir);
                eliberare_file(temp->files);
                free(temp->nume);
                free(temp);
                (*crt_dir)->st = temp2;
            }
            else
            //caz genera;
            {
                Tarbdir parinte = temp;
                //ma duc cat pot in dreapta
                //si tin minte parinte la ultimu element
                while (temp->dr != NULL)
                {
                    parinte = temp;
                    temp = temp->dr;
                }
                //salvez ce e in stanga ultimului element dr in temp2
                Tarbdir temp2 = temp->st;
                //fac schimb intre cele doua elemente
                strcpy((*crt_dir)->nume, temp->nume);
                (*crt_dir)->parinte = temp->parinte;
                (*crt_dir)->files = temp->files;
                (*crt_dir)->subdir = temp->subdir;
                eliberare_dir(temp->subdir);
                eliberare_file(temp->files);
                free(temp->nume);
                free(temp);
                //reunesc subarborele dupa ce am schimbat cele doua noduri
                //m-am luat dupa algoritmul din curs
                parinte->dr = temp2;
            }
        }
    }
}

//caut dir recursiv si il intorc in main pt a da pwd
Tarbdir caut_dir(Tarbdir crt_dir, char *nume, Tarbdir aux)
{
    if (crt_dir == NULL)
        return aux;
    aux = caut_dir(crt_dir->st, nume, aux);
    aux = caut_dir(crt_dir->dr, nume, aux);

    if (strcmp(crt_dir->nume, nume) == 0)
        return crt_dir;

    aux = caut_dir(crt_dir->subdir, nume, aux);
    return aux;
}
