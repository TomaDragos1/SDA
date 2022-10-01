/* Toma Mihai Dragos - 312CB */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "TCoada.h"
#include "TStiva.h"

/* numar maxim elemente coada / stiva */
#define MAX 32678

typedef struct
{
	unsigned int ID;
} Thread;

typedef struct
{
	unsigned short ID;
	Thread curent_thread;
	char priority;
	int execution_time;
	int exec_time_run;
} Task;

//program de inserare sortata in waiting si running

void sort_coada_wait(void **c, Task *crt_task)
{
	void *aux = InitQ(sizeof(Task));
	int rez;
	if (aux == NULL)
		return;
	Task *pars = malloc(sizeof(Task));
	if (pars == NULL)
	{
		DistrQ(&aux);
		return;
	}
	int ok = 0;
	//condtii pt inserare sortata
	while (!VidaQ(*c))
	{
		ExtrQ(*c, pars);
		if (pars->priority < crt_task->priority)
		{
			rez = IntrQ(aux, crt_task);
			rez = IntrQ(aux, pars);
			ok = 1;
			break;
		}
		else if (crt_task->priority == pars->priority 
		&& crt_task->exec_time_run < pars->exec_time_run)
		{
			rez = IntrQ(aux, crt_task);
			rez = IntrQ(aux, pars);
			ok = 1;
			break;
		}
		else if (crt_task->priority == pars->priority &&
				 crt_task->exec_time_run == pars->exec_time_run && crt_task->ID < pars->ID)
		{
			rez = IntrQ(aux, crt_task);
			rez = IntrQ(aux, pars);
			ok = 1;
			break;
		}
		rez = IntrQ(aux, pars);
	}
	ConcatQ(aux, *c);
	//in caz ca e elementul de la finalul cozii
	if (ok == 0)
		rez = IntrQ(aux, crt_task);
	if (rez == 0)
	{
		free(pars);
		DistrQ(&aux);
	}
	ConcatQ(*c, aux);
	free(pars);
	DistrQ(&aux);
}

//calculeaza maxim in coada in fucntie de exec_time
int max_run(void *c_run)
{
	Task *pars = malloc(sizeof(Task));
	int rez;
	if (pars == NULL)
		return -1;
	int max = 0;
	void *aux = InitQ(sizeof(Task));
	while (!VidaQ(c_run))
	{
		ExtrQ(c_run, pars);
		if (pars->exec_time_run > max)
			max = pars->exec_time_run;
		rez = IntrQ(aux, pars);
	}
	if (rez == 0)
	{
		free(pars);
		return -1;
	}
	ConcatQ(c_run, aux);
	DistrQ(&aux);
	free(pars);
	return max;
}

//task running
void running(void **c_wait, void **c_run, void **c_fin, 
void **stiva, int time, int **cont_id, int *total_time)
{
	Thread *crt_thread = calloc(1, sizeof(Thread));
	Task *pars = calloc(1, sizeof(Task));
	void *aux = InitQ(sizeof(Task));
	int rez = 1;
	//scot din coada de waiting si asignez thread
	while (!VidaQ(*c_wait) && !VidaS(*stiva))
	{
		ExtrQ(*c_wait, pars);
		Pop(*stiva, crt_thread);
		pars->curent_thread.ID = crt_thread->ID;
		sort_coada_wait(&(*c_run), pars);
	}
	if (!VidaQ(*c_run) || !VidaQ(*c_wait))
	{
		int val = max_run(*c_run);
		if (val == -1)
		{
			free(crt_thread);
			free(pars);
			free(aux);
			DistrQ(&aux);
			return;
		}
		if (val < time)
			(*total_time) += val;
		else
			(*total_time) += time;
	}
	//rulez timpul si scot din run daca e cazul
	while (!VidaQ(*c_run))
	{
		ExtrQ(*c_run, pars);
		int comp = pars->exec_time_run - time;
		if (comp <= 0)
		{
			Thread push_thread;
			push_thread.ID = pars->curent_thread.ID;
			rez = Push(*stiva, &push_thread);
			(*cont_id)[pars->ID] = 0;
			rez = IntrQ(*c_fin, pars);
		}
		else
		{
			pars->exec_time_run -= time;
			rez = IntrQ(aux, pars);
		}
	}
	ConcatQ(*c_run, aux);
	//iar bag in coada de run cata threaduri mai sunt disponibile
	while (!VidaQ(*c_wait) && !VidaS(*stiva))
	{
		ExtrQ(*c_wait, pars);
		Pop(*stiva, crt_thread);
		pars->curent_thread.ID = crt_thread->ID;
		sort_coada_wait(&(*c_run), pars);
	}
	if (rez == 0)
	{
		free(pars);
		free(crt_thread);
		DistrQ(&aux);
		return;
	}
	DistrQ(&aux);
	free(pars);
	free(crt_thread);
}
//functii de afisare in functie de taskul meu
int afi_task_run(Task *crt_task, FILE *out)
{
	fprintf(out, "(%d: priority = %d, remaining_time = %d, running_thread = %d)",
	crt_task->ID, crt_task->priority, crt_task->exec_time_run, crt_task->curent_thread.ID);
	return 1;
}

int afi_task_fin(Task *crt_task, FILE *out)
{
	fprintf(out, "(%d: priority = %d, executed_time = %d)", 
	crt_task->ID, crt_task->priority, crt_task->execution_time);
	return 1;
}

int afi_task(Task *crt_task, FILE *out)
{
	fprintf(out, "(%d: priority = %d, remaining_time = %d)", 
	crt_task->ID, crt_task->priority, crt_task->execution_time);
	return 1;
}
//parsreaza prin coada si afisez
void parsare_queue(void *c, int (*afisare)(Task *, FILE *), FILE *out)
{
	Task *pars = malloc(sizeof(Task));
	int rez;
	if (pars == NULL)
	{
		return;
	}
	void *aux = InitQ(sizeof(Task));
	if (aux == NULL)
	{
		free(pars);
		return;
	}
	while (!VidaQ(c))
	{
		ExtrQ(c, pars);
		afisare(pars, out);
		if (!VidaQ(c))
			fprintf(out, ",\n");
		rez = IntrQ(aux, pars);
		if (rez == 0)
		{
			free(pars);
			free(aux);
			DistrQ(&aux);
			return;
		}
	}
	ConcatQ(c, aux);
	DistrQ(&aux);
	free(pars);
}
//fucntia get task
void get_task(void *c_run, void *c_wait, void *c_fin, unsigned short ID, FILE *out)
{
	Task pars;
	int rez;
	void *aux = InitQ(sizeof(Task));
	if (aux == NULL)
	{
		return;
	}
	//pt coada run
	while (!VidaQ(c_run))
	{
		ExtrQ(c_run, &pars);
		if (pars.ID == ID)
		{
			fprintf(out, "Task %d is running (remaining_time = %d).\n", 
			pars.ID, pars.exec_time_run);
			rez = IntrQ(aux, &pars);
			ConcatQ(aux, c_run);
			ConcatQ(c_run, aux);
			DistrQ(&aux);
			return;
		}
		rez = IntrQ(aux, &pars);
		if (rez == 0)
		{
			DistrQ(&aux);
			return;
		}
	}
	ConcatQ(c_run, aux);
	//pt coada wait
	while (!VidaQ(c_wait))
	{
		ExtrQ(c_wait, &pars);
		if (pars.ID == ID)
		{
			fprintf(out, "Task %d is waiting (remaining_time = %d).\n", 
			pars.ID, pars.execution_time);
			rez = IntrQ(aux, &pars);
			ConcatQ(aux, c_wait);
			ConcatQ(c_wait, aux);
			DistrQ(&aux);
			return;
		}
		rez = IntrQ(aux, &pars);
		if (rez == 0)
		{
			DistrQ(&aux);
			return;
		}
	}
	ConcatQ(c_wait, aux);
	//pt coada fin
	while (!VidaQ(c_fin))
	{
		ExtrQ(c_fin, &pars);
		if (pars.ID == ID)
		{
			fprintf(out, "Task %d is finished (executed_time = %d).\n", 
			pars.ID, pars.execution_time);
			rez = IntrQ(aux, &pars);
			ConcatQ(aux, c_fin);
			ConcatQ(c_fin, aux);
			DistrQ(&aux);
			return;
		}
		rez = IntrQ(aux, &pars);
		if (rez == 0)
		{
			DistrQ(&aux);
			return;
		}
	}
	ConcatQ(c_fin, aux);
	fprintf(out, "Task %d not found.\n", ID);
	DistrQ(&aux);
}
//get thread
void get_thread(void *c_run, void *stiva, unsigned int thread, FILE *out)
{
	Task *pars = malloc(sizeof(Task));
	if (pars == NULL)
	{
		return;
	}
	void *aux = InitQ(sizeof(Task));
	if (aux == NULL)
	{
		free(pars);
		return;
	}
	Thread *pars_thread = malloc(sizeof(Thread));
	if (pars_thread == NULL)
	{
		free(pars);
		DistrQ(&aux);
		return;
	}
	void *stiva_aux = InitS(sizeof(Thread));
	if (stiva_aux == NULL)
	{
		free(pars);
		DistrQ(&aux);
		free(pars_thread);
		return;
	}
	int ok = 0;
	int rez;
	//parcurgere stiva
	while (!VidaS(stiva))
	{
		Pop(stiva, pars_thread);
		if (pars_thread->ID == thread)
		{
			fprintf(out, "Thread %d is idle.\n", thread);
			ok = 1;
		}
		rez = Push(stiva_aux, pars_thread);
		if (rez == 0)
		{
			free(pars);
			DistrQ(&aux);
			DistrS(&stiva_aux);
			free(pars_thread);
			return;
		}
	}
	Rastoarna(stiva, stiva_aux);
	//parcurgere coada run
	while (!VidaQ(c_run) && ok == 0)
	{
		ExtrQ(c_run, pars);
		if (pars->curent_thread.ID == thread)
		{
			fprintf(out, "Thread %d is running task %d (remaining_time = %d).\n", 
			thread, pars->ID, pars->exec_time_run);
		}
		rez = IntrQ(aux, pars);
		if (rez == 0)
		{
			free(pars);
			DistrQ(&aux);
			DistrS(&stiva_aux);
			free(pars_thread);
			return;
		}
	}
	ConcatQ(c_run, aux);
	free(pars);
	free(pars_thread);
	DistrQ(&aux);
	DistrS(&stiva_aux);
}
//functia de finish care e exact ca run
void finish(void **c_wait, void **c_run, void **c_fin,
 void **stiva, int Q, int total_time, FILE *out, int **cont_id)
{
	Thread *crt_thread = malloc(sizeof(Thread));
	int rez;
	if (crt_thread == NULL)
	{
		return;
	}
	Task *pars = malloc(sizeof(Task));
	if (pars == NULL)
	{
		free(crt_thread);
		return;
	}
	void *aux = InitQ(sizeof(Task));
	if (aux == NULL)
	{
		free(pars);
		free(crt_thread);
		return;
	}
	while (1)
	{
		if (VidaQ(*c_run) && VidaQ(*c_wait))
			break;
		ResetQ(aux);
		while (!VidaQ(*c_wait) && !VidaS(*stiva))
		{
			ExtrQ(*c_wait, pars);
			Pop(*stiva, crt_thread);
			pars->curent_thread.ID = crt_thread->ID;
			sort_coada_wait(&(*c_run), pars);
		}
		//calculez maximul la fiecare iteratie de timp sa vad
		//cat e timpul pt rulat
		int crt_time = max_run(*c_run);
		if (crt_time == -1)
		{
			free(pars);
			DistrQ(&aux);
			free(crt_thread);
			return;
		}
		if (crt_time > Q)
			crt_time = Q;
		int time = crt_time;
		total_time += time;
		while (!VidaQ(*c_run))
		{
			ExtrQ(*c_run, pars);
			int comp = pars->exec_time_run - time;
			if (comp <= 0)
			{
				Thread push_thread;
				push_thread.ID = pars->curent_thread.ID;
				rez = Push(*stiva, &push_thread);
				(*cont_id)[pars->ID] = 0;
				rez = IntrQ(*c_fin, pars);
			}
			else
			{
				pars->exec_time_run -= time;
				rez = IntrQ(aux, pars);
			}
			if (rez == 0)
			{
				free(pars);
				DistrQ(&aux);
				free(crt_thread);
				return;
			}
		}
		ConcatQ(*c_run, aux);
		while (!VidaQ(*c_wait) && !VidaS(*stiva))
		{
			ExtrQ(*c_wait, pars);
			Pop(*stiva, crt_thread);
			pars->curent_thread.ID = crt_thread->ID;
			sort_coada_wait(&(*c_run), pars);
		}
	}
	fprintf(out, "Total time: %d", total_time);
	DistrQ(&aux);
	free(pars);
	free(crt_thread);
}

int main(int argc, char *argv[])
{
	int Q;
	int C;
	FILE *in, *out;
	in = fopen(argv[argc - 2], "r");
	out = fopen(argv[argc - 1], "w");
	fscanf(in, "%d", &Q);
	fscanf(in, "%d\n", &C);
	char *line = malloc(256);
	int n = 2 * C;
	//initializari stive si cozi
	void *c_wait = NULL, *c_run = NULL, *c_finish = NULL, *stiva = NULL;
	c_wait = InitQ(sizeof(Task));
	if (c_wait == NULL)
		return -1;
	c_run = InitQ(sizeof(Task));
	if (c_run == NULL)
		return -1;
	c_finish = InitQ(sizeof(Task));
	if (c_finish == NULL)
		return -1;
	int *cont_ID = (int *)calloc(MAX + 2, sizeof(unsigned short));
	if (cont_ID == NULL)
		return -1;
	stiva = InitS(sizeof(Thread));
	if (stiva == NULL)
		return -1;
	int total_time = 0;
	for (int i = n - 1; i >= 0; i--)
	{
		Thread *new_thread = malloc(sizeof(Thread));
		if (new_thread == NULL)
		{
			DistrS(&stiva);
			return -1;
		}
		new_thread->ID = i;
		Push(stiva, new_thread);
		free(new_thread);
	}
	//citire
	while (fgets(line, 256, in))
	{
		char *word = strtok(line, " \n");
		if (strcmp(word, "add_tasks") == 0)
		{
			word = strtok(NULL, " \n");
			int number_task = atoi(word);
			word = strtok(NULL, " \n");
			int exec_time = atoi(word);
			word = strtok(NULL, " \n");
			char priority = atoi(word);
			while (number_task > 0)
			{
				//creare task
				Task *crt_task = malloc(sizeof(Task));
				if (crt_task == NULL)
				{
					return -1;
				}
				int ok = 0;
				//alocare id
				for (int i = 1; i <= MAX && ok == 0; i++)
				{
					if (cont_ID[i] == 0)
					{
						crt_task->ID = i;
						cont_ID[i] = 1;
						ok = 1;
					}
				}
				fprintf(out, "Task created successfully : ID %d.\n", crt_task->ID);
				crt_task->exec_time_run = exec_time;
				crt_task->execution_time = exec_time;
				crt_task->priority = priority;
				//sortare
				sort_coada_wait(&c_wait, crt_task);
				number_task--;
				free(crt_task);
			}
		}
		if (strcmp(word, "print") == 0)
		{
			word = strtok(NULL, " \n");
			//toate printurile cozilor
			if (strcmp(word, "waiting") == 0)
			{
				fprintf(out, "====== Waiting queue =======\n");
				fprintf(out, "[");
				parsare_queue(c_wait, afi_task, out);
				fprintf(out, "]");
				fprintf(out, "\n");
			}
			if (strcmp(word, "running") == 0)
			{
				fprintf(out, "====== Running in parallel =======\n");
				fprintf(out, "[");
				parsare_queue(c_run, afi_task_run, out);
				fprintf(out, "]");
				fprintf(out, "\n");
			}
			if (strcmp(word, "finished") == 0)
			{
				fprintf(out, "====== Finished queue =======\n");
				fprintf(out, "[");
				parsare_queue(c_finish, afi_task_fin, out);
				fprintf(out, "]");
				fprintf(out, "\n");
			}
		}
		if (strcmp(word, "run") == 0)
		{
			word = strtok(NULL, " \n");
			int time = atoi(word);
			fprintf(out, "Running tasks for %d ms...\n", time);
			//determinare cuanta si rulat in functie de min dintre q si timo
			//si scad din timpul meu pana ajunge la 0
			if (time > Q)
			{
				while (time >= Q)
				{
					running(&c_wait, &c_run, &c_finish, &stiva, Q, &cont_ID, &total_time);
					time = time - Q;
				}
			}
			if (time >= 0)
			{
				running(&c_wait, &c_run, &c_finish, &stiva, time, &cont_ID, &total_time);
			}
		}
		if (strcmp(word, "get_task") == 0)
		{
			int ID;
			word = strtok(NULL, " \n");
			ID = atoi(word);
			get_task(c_run, c_wait, c_finish, ID, out);
		}
		if (strcmp(word, "get_thread") == 0)
		{
			unsigned int thread;
			word = strtok(NULL, " \n");
			thread = atoi(word);
			get_thread(c_run, stiva, thread, out);
		}
		if (strcmp(word, "finish") == 0)
		{
			finish(&c_wait, &c_run, &c_finish, &stiva, Q, total_time, out, &cont_ID);
		}
	}
	//dezalocare
	DistrQ(&c_wait);
	DistrQ(&c_run);
	DistrQ(&c_finish);
	DistrS(&stiva);
	free(cont_ID);
	free(line);
	fclose(in);
	fclose(out);
	return 0;
}
