/* Toma Mihai Dragos - 312CB */
/*--- tarb.h ---*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef Binary_tree
#define Binary_tree

struct file;
struct director;

typedef struct file
{
	char *nume;
	struct director *parinte;
	struct file *st, *dr;
} Tfile, *Tarbfile;

typedef struct director
{
	char *nume;
	struct director *parinte;
	struct director *subdir;
	struct file *files;
	struct director *st, *dr;
} Tdir, *Tarbdir;

//file functions
Tarbfile aloc_file(char *, Tarbdir); //functie care imi aloca file
int insert_file(Tarbdir *, char *); //functie de inserare in arbore de file
void afisare_fisier(Tarbfile); // functie folosita la ls ca sa afisesre in inordine arborele
int verif_fis_exist(Tarbfile, char *);//verficia daca exista file in arobre si returnez 1 daca da
void delete_file(Tarbfile *, char *); //sterg un anumit element din arborele de file
void eliberare_file(Tarbfile); //sterg tot arborele de file
Tarbdir caut_file(Tarbdir, char *, Tarbdir);//caut un file in toata ierarhia la find -f

//directory functions

Tarbdir aloc_dir(char *, Tarbdir); //aloc un struct de tip dir
int insert_director(Tarbdir *, char *);//inserez in arborele de directoare
void afisare_dir(Tarbdir);//afisez arborele de directoare in inordine
int verif_dir_exist(Tarbdir, char *);//verific daca exista dir si intorc 1
void eliberare_dir(Tarbdir);//functie care imi sterge un director
void delete_dir(Tarbdir*, char *);//sterge un intreg arbore de dir
Tarbdir caut_dir(Tarbdir, char *, Tarbdir);//caut un dir in tota ierarhia de directoare

//other functions
Tarbdir cautare_dir_cd(Tarbdir, char *);//caut sa vad daca exsita dir in arborele curent si il intorc
Tarbdir cd_dir(Tarbdir, char *);//efectiv dau cd in dir daca exista
void pwd(Tarbdir);//afisez recursiv working directory

#endif