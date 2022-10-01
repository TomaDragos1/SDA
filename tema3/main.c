/* Toma Mihai Dragos - 312CB */
#include "tarb.h"

//caut director in subdir si il intorc
Tarbdir cautare_dir_cd(Tarbdir crt_subdir, char *nume)
{
	Tarbdir new_dir;
	while (crt_subdir != NULL)
	{
		if (strcmp(crt_subdir->nume, nume) == 0)
		{
			new_dir = crt_subdir;
			return new_dir;
		}
		if (strcmp(crt_subdir->nume, nume) > 0)
			crt_subdir = crt_subdir->st;
		else
			crt_subdir = crt_subdir->dr;
	}
	return NULL;
}

//dau efectiv cd in dir dorit si am si cazul pt ..
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

//afisare recursiva
void pwd(Tarbdir crt_dir)
{
	if (crt_dir == NULL)
		return;
	pwd(crt_dir->parinte);
	printf("/%s", crt_dir->nume);
}

int main()
{
	//alocare root
	Tarbdir root = malloc(sizeof(Tdir));
	int rez_verif;
	if (root == NULL)
	{
		return -1;
	}
	Tarbdir crt_director;
	root->nume = malloc(5);
	if (root->nume == NULL)
	{
		free(root);
		return -1;
	}
	strcpy(root->nume, "root");
	root->dr = NULL;
	root->st = NULL;
	root->parinte = NULL;
	root->subdir = NULL;
	root->files = NULL;
	crt_director = root;
	char *line = malloc(256);
	if (line == NULL)
	{
		free(root);
		free(root->nume);
		return -1;
	}
	char *word;
	while (fgets(line, 256, stdin))
	{
		word = strtok(line, " \n");
		if (strcmp(word, "touch") == 0)
		{
			word = strtok(NULL, " \n");
			//verific daca exista deja numele
			int rez = verif_dir_exist(crt_director->subdir, word);
			if (rez == 0)
			{
				rez_verif = insert_file(&crt_director, word);
				if (rez_verif == 0)
				{
					return -1;
				}
			}
			else
				printf("Directory %s already exists!\n", word);
		}
		if (strcmp(word, "mkdir") == 0)
		{
			word = strtok(NULL, " \n");
			//verific daca exista deja numele
			int rez = verif_fis_exist(crt_director->files, word);
			if (rez == 0)
			{
				rez_verif = insert_director(&crt_director, word);
				if (rez_verif == 0)
				{
					return -1;
				}
			}
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
			//verific sa vad daca exista file la care vreau sa dau rm
			int rez = verif_fis_exist(crt_director->files, word);
			if (rez == 1)
				delete_file(&(crt_director->files), word);
			else
				printf("File %s doesn't exist!\n", word);
		}
		if (strcmp(word, "rmdir") == 0)
		{
			word = strtok(NULL, " \n");
			//verific sa vad daca exista dir la care vreau sa dau rmdir
			int rez = verif_dir_exist(crt_director->subdir, word);
			if (rez == 1)
			{
				delete_dir(&(crt_director->subdir), word);
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
			//find la file
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
			//find la dir
			{
				Tarbdir temp, aux = NULL;
				temp = caut_dir(root->subdir, name, aux);
				if (temp == NULL)
				{
					//caz in care dau find la root
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
		//functie de eliberare
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
