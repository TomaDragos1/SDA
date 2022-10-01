/**-- test-ab.c --  prelucreaza arbori binari cu chei intregi --*/
#include "tarb.h"

/*-- se completeaza cu definitiile functiilor implementate --*/

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
	while (n)
	{
		p = n;
		if (strcmp(nume, n->nume) == 0)
		{
			printf("File %s already exists!\n", nume);
			return 0;
		}
		if (strcmp(nume, n->nume) < 0)
			n = n->st;
		else
			n = n->dr;
	}
	aux = aloc_file(nume, *crt_dir);
	if (!aux)
		return 0;
	if (strcmp(nume, p->nume) < 0)
		p->st = aux;
	else
		p->dr = aux;

	return 1;
}

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

int insert_director(Tarbdir *crt_dir, char *nume)
{
	Tarbdir *dir_rad, n, p, aux;
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
	while (n)
	{
		p = n;
		if (strcmp(nume, n->nume) == 0)
		{
			printf("Directory %s already exists!\n", nume);
			return 0;
		}
		if (strcmp(nume, n->nume) < 0)
			n = n->st;
		else
			n = n->dr;
	}
	aux = aloc_dir(nume, *crt_dir);
	if (!aux)
		return 0;
	if (strcmp(nume, p->nume) < 0)
		p->st = aux;
	else
		p->dr = aux;
	return 1;
}

void afisare_fisier(Tarbfile crt_file_rad)
{
	if (crt_file_rad == NULL)
		return;
	afisare_fisier(crt_file_rad->st);
	printf("%s ", crt_file_rad->nume);
	afisare_fisier(crt_file_rad->dr);
}

void afisare_dir(Tarbdir crt_dir_rad)
{
	if (crt_dir_rad == NULL)
		return;
	afisare_dir(crt_dir_rad->st);
	printf("%s ", crt_dir_rad->nume);
	afisare_dir(crt_dir_rad->dr);
}

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

Tarbfile delete (Tarbfile crt_file, char *nume)
{
	if (crt_file == NULL)
		return crt_file;
	else if (strcmp(nume, crt_file->nume) < 0)
		crt_file->st = delete (crt_file->st, nume);
	else if (strcmp(nume, crt_file->nume) > 0)
		crt_file->dr = delete (crt_file->dr, nume);
	else
	{
		if (crt_file->st == NULL && crt_file->dr == NULL)
		{
			free(crt_file->nume);
			free(crt_file);
			crt_file = NULL;
		}
		else if (crt_file->st == NULL)
		{
			Tarbfile temp = crt_file;
			crt_file = crt_file->dr;
			free(temp->nume);
			free(temp);
		}
		else if (crt_file->dr == NULL)
		{
			Tarbfile temp = crt_file;
			crt_file = crt_file->st;
			free(temp->nume);
			free(temp);
		}
		else
		{
			Tarbfile temp = crt_file;
			temp = temp->st;
			if (temp->dr == NULL)
			{
				Tarbfile temp2 = temp->st;
				strcpy(crt_file->nume, temp->nume);
				crt_file->parinte = temp->parinte;
				free(temp->nume);
				free(temp);
				crt_file->st = temp2;
			}
			else
			{
				Tarbfile parinte_aux;
				while (temp != NULL)
				{
					parinte_aux = temp;
					temp = temp->dr;
				}
				Tarbfile temp2 = temp->st;
				strcpy(crt_file->nume, temp->nume);
				crt_file->parinte = temp->parinte;
				free(temp->nume);
				free(temp);
				parinte_aux->dr = temp2;
			}
		}
	}
	return crt_file;
}

void eliberare_file(Tarbfile delete_file)
{
	if (delete_file == NULL)
		return;
	eliberare_file(delete_file->st);
	eliberare_file(delete_file->dr);
	free(delete_file->nume);
	free(delete_file);
}

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

Tarbdir delete_dir(Tarbdir crt_dir, char *nume)
{
	if (crt_dir == NULL)
		return crt_dir;
	else if (strcmp(nume, crt_dir->nume) < 0)
		crt_dir->st = delete_dir(crt_dir->st, nume);
	else if (strcmp(nume, crt_dir->nume) > 0)
		crt_dir->dr = delete_dir(crt_dir->dr, nume);
	else
	{
		if (crt_dir->st == NULL && crt_dir->dr == NULL)
		{
			eliberare_dir(crt_dir->subdir);
			eliberare_file(crt_dir->files);
			free(crt_dir->nume);
			free(crt_dir);
			crt_dir = NULL;
		}
		else if (crt_dir->st == NULL)
		{
			Tarbdir temp = crt_dir;
			crt_dir = crt_dir->dr;
			eliberare_dir(temp->subdir);
			eliberare_file(temp->files);
			free(temp->nume);
			free(temp);
			temp = NULL;
		}
		else if (crt_dir->dr == NULL)
		{
			Tarbdir temp = crt_dir;
			crt_dir = crt_dir->st;
			eliberare_dir(temp->subdir);
			eliberare_file(temp->files);
			free(temp->nume);
			free(temp);
			temp = NULL;
		}
		else
		{
			Tarbdir temp = crt_dir;
			temp = temp->st;
			if (temp->dr == NULL)
			{
				Tarbdir temp2 = temp->st;
				strcpy(crt_dir->nume, temp->nume);
				crt_dir->parinte = temp->parinte;
				crt_dir->files = temp->files;
				crt_dir->subdir = temp->subdir;
				eliberare_dir(temp->subdir);
				eliberare_file(temp->files);
				free(temp->nume);
				free(temp);
				crt_dir->st = temp2;
			}
			else
			{
				Tarbdir parinte = temp;
				while (temp->dr != NULL)
				{
					parinte = temp;
					temp = temp->dr;
				}
				Tarbdir temp2 = temp->st;
				strcpy(crt_dir->nume, temp->nume);
				crt_dir->parinte = temp->parinte;
				crt_dir->files = temp->files;
				crt_dir->subdir = temp->subdir;
				eliberare_dir(temp->subdir);
				eliberare_file(temp->files);
				free(temp->nume);
				free(temp);
				parinte->dr = temp2;
			}
		}
	}
	return crt_dir;
}

Tarbdir cautare_dir_cd(Tarbdir crt_subdir, char *nume)
{
	Tarbdir new_dir;
	while (crt_subdir != NULL)
	{
		if (strcmp(crt_subdir->nume, nume) == 0)
		{
			new_dir = crt_subdir;
			//printf("aici trb sa fie %s\n", new_dir->nume);
			return new_dir;
		}
		if (strcmp(crt_subdir->nume, nume) > 0)
			crt_subdir = crt_subdir->st;
		else
			crt_subdir = crt_subdir->dr;
	}
	return NULL;
}

Tarbdir cd_dir(Tarbdir crt_dir, char *new_dir_name)
{
	Tarbdir new_dir;
	if (strcmp(new_dir_name, "..") == 0)
	{
		if (strcmp("root", crt_dir->nume) == 0)
			return crt_dir;
		else
		{
			new_dir = crt_dir->parinte;
			return new_dir;
		}
	}
	else
	{
		new_dir = cautare_dir_cd(crt_dir->subdir, new_dir_name);
		return new_dir;
	}
}

void pwd(Tarbdir crt_dir)
{
	if (crt_dir == NULL)
		return;
	pwd(crt_dir->parinte);
	printf("/%s", crt_dir->nume);
}

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
	caut_file(crt_dir->subdir, nume, aux);
}

Tarbdir caut_dir(Tarbdir crt_dir, char *nume, Tarbdir aux)
{
	if (crt_dir == NULL)
		return aux;
	aux = caut_dir(crt_dir->st, nume, aux);
	aux = caut_dir(crt_dir->dr, nume, aux);

	if (strcmp(crt_dir->nume, nume) == 0)
		return crt_dir;

	aux = caut_dir(crt_dir->subdir, nume, aux);
}

int main()
{
	Tarbdir root = malloc(sizeof(Tdir));
	Tarbdir crt_director;
	root->nume = malloc(5);
	strcpy(root->nume, "root");
	root->dr = NULL;
	root->st = NULL;
	root->parinte = NULL;
	root->subdir = NULL;
	root->files = NULL;
	crt_director = root;
	char *line = malloc(256);
	char *word;
	while (fgets(line, 256, stdin))
	{
		word = strtok(line, " \n");
		if (strcmp(word, "touch") == 0)
		{
			word = strtok(NULL, " \n");
			int rez = verif_dir_exist(crt_director->subdir, word);
			if (rez == 0)
				insert_file(&crt_director, word);
			else
				printf("Directory %s already exists!\n", word);
		}
		if (strcmp(word, "mkdir") == 0)
		{
			word = strtok(NULL, " \n");
			int rez = verif_fis_exist(crt_director->files, word);
			if (rez == 0)
				insert_director(&crt_director, word);
			else
				printf("File %s already exists!\n", word);
		}
		if (strcmp(word, "ls") == 0)
		{
			afisare_dir(crt_director->subdir);
			afisare_fisier(crt_director->files);
			printf("\n");
		}
		if (strcmp(word, "rm") == 0)
		{
			word = strtok(NULL, " \n");
			int rez = verif_fis_exist(crt_director->files, word);
			if (rez == 1)
				crt_director->files = delete (crt_director->files, word);
			else
				printf("File %s doesn't exist!\n", word);
		}
		if (strcmp(word, "rmdir") == 0)
		{
			word = strtok(NULL, " \n");
			int rez = verif_dir_exist(crt_director->subdir, word);
			if (rez == 1)
			{
				crt_director->subdir = delete_dir(crt_director->subdir, word);
			}
			else
			{
				printf("Directory %s doesn't exist!\n", word);
			}
		}
		if (strcmp(word, "cd") == 0)
		{
			word = strtok(NULL, " \n");
			Tarbdir temp;
			temp = cd_dir(crt_director, word);
			if (temp == NULL)
			{
				printf("Directory not found!\n");
			}
			else
				crt_director = temp;
		}
		if (strcmp(word, "pwd") == 0)
		{
			pwd(crt_director);
			printf("\n");
		}
		if (strcmp(word, "find") == 0)
		{
			word = strtok(NULL, " \n");
			char *type = word;
			word = strtok(NULL, " \n");
			char *name = word;
			if (strcmp(type, "-f") == 0)
			{
				Tarbdir temp, aux = NULL;
				temp = caut_file(root, name, aux);
				if (temp == NULL)
					printf("File %s not found!\n", name);
				else
				{
					printf("File %s found!\n", name);
					pwd(temp);
					printf("\n");
				}
			}
			else
			{
				Tarbdir temp, aux = NULL;
				temp = caut_dir(root->subdir, name, aux);
				if (temp == NULL)
				{
					if (strcmp(name, "root") == 0)
					{
						printf("/root\n");
					}
					else
						printf("Directory %s not found!\n", name);
				}
				else
				{
					printf("Directory %s found!\n", name);
					pwd(temp);
					printf("\n");
				}
			}
		}
		if (strcmp(word, "quit") == 0)
		{
			eliberare_dir(root->subdir);
			eliberare_file(root->files);
			root->subdir = NULL;
			free(root->nume);
			free(root);
			free(line);
		}
	}

	return 0;
}
