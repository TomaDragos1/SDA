#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "TCoada.h"
#include "TStiva.h"

// /* numar maxim elemente coada / stiva */
#define MAX 32678

typedef struct
{
	int ID;
} Thread;

typedef struct
{
	int ID;
	Thread curent_thread;
	int priority;
	int execution_time;
	int exec_time_run;
} Task;

void sort_coada_wait(void **c, Task *crt_task)
{
	void *aux = InitQ(sizeof(Task));
	Task *pars = malloc(sizeof(Task));
	int ok = 0;
	while (!VidaQ(*c))
	{
		ExtrQ(*c, pars);
		if (pars->priority < crt_task->priority)
		{
			IntrQ(aux, crt_task);
			IntrQ(aux, pars);
			ok = 1;
			break;
		}
		else if (crt_task->priority == pars->priority && crt_task->exec_time_run < pars->exec_time_run)
		{
			IntrQ(aux, crt_task);
			IntrQ(aux, pars);
			ok = 1;
			break;
		}
		else if (crt_task->priority == pars->priority && crt_task->exec_time_run == pars->exec_time_run && crt_task->ID < pars->ID)
		{
			IntrQ(aux, crt_task);
			IntrQ(aux, pars);
			ok = 1;
			break;
		}
		IntrQ(aux, pars);
	}
	while (!VidaQ(*c))
	{
		ExtrQ(*c, pars);
		IntrQ(aux, pars);
	}
	if (ok == 0)
		IntrQ(aux, crt_task);
	ConcatQ(*c, aux);
	free(pars);
	DistrQ(&aux);
}
int max_run(void *c_run)
{
	Task *pars = malloc(sizeof(Task));
	int max = 0;
	void *aux = InitQ(sizeof(Task));
	while (!VidaQ(c_run))
	{
		ExtrQ(c_run, pars);
		if (pars->exec_time_run > max)
			max = pars->exec_time_run;
		IntrQ(aux, pars);
	}
	ConcatQ(c_run, aux);
	DistrQ(&aux);
	free(pars);
	return max;
}

void inserare_stiva(void **stiva, int id)
{
	void *stiva_aux = InitS(sizeof(int));
	int ok = 0;
	while (!VidaS(*stiva))
	{
		int ID_pars;
		Pop(*stiva, &ID_pars);
		if(ID_pars > id && ok == 0)
		{
			Push(stiva_aux, &id);
			Push(stiva_aux, &ID_pars);
			ok = 1;
		}
		else Push(stiva_aux, &ID_pars);

	}
	if(ok == 0)
	Push(stiva_aux, &id);
	Rastoarna(*stiva, stiva_aux);
	DistrS(&stiva_aux);
}

void running(void **c_wait, void **c_run, void **c_fin, void **stiva, int time, void **stiva_id, int *total_time, FILE *out)
{
	Thread *crt_thread = calloc(1, sizeof(Thread));
	Task *pars = calloc(1, sizeof(Task));
	void *aux = InitQ(sizeof(Task));
	int contor = 0;
	int ok = 0;
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
		if (val < time)
			(*total_time) += val;
		else
			(*total_time) += time;
	}
	while (!VidaQ(*c_run))
	{
		ExtrQ(*c_run, pars);
		int comp = pars->exec_time_run - time;
		if (comp <= 0)
		{
			Thread push_thread;
			//push_thread = pars->curent_thread;
			push_thread.ID = pars->curent_thread.ID;
			//fprintf(out, "%d astea sunt id-r \n", pars->ID);
			inserare_stiva(&(*stiva_id), pars->ID);
			Push(*stiva, &push_thread);
			//(*cont_id)[pars->ID] = 0;
			IntrQ(*c_fin, pars);
			contor++;
		}
		else
		{
			pars->exec_time_run -= time;
			IntrQ(aux, pars);
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
	DistrQ(&aux);
	free(pars);
	free(crt_thread);
}
int afi_task_run(Task *crt_task, FILE *out)
{
	fprintf(out, "(%d: priority = %d, remaining_time = %d, running_thread = %d)", crt_task->ID,
			crt_task->priority, crt_task->exec_time_run, crt_task->curent_thread.ID);
	return 1;
}

int afi_task_fin(Task *crt_task, FILE *out)
{
	fprintf(out, "(%d: priority = %d, executed_time = %d)", crt_task->ID, crt_task->priority, crt_task->execution_time);
	return 1;
}

int afi_task(Task *crt_task, FILE *out)
{
	fprintf(out, "(%d: priority = %d, remaining_time = %d)", crt_task->ID, crt_task->priority, crt_task->execution_time);
	return 1;
}

void parsare_queue(void *c, int (*afisare)(Task *, FILE *), FILE *out)
{
	Task *pars = malloc(sizeof(Task));
	void *aux = InitQ(sizeof(Task));
	while (!VidaQ(c))
	{
		Task *verif;
		ExtrQ(c, pars);
		afisare(pars, out);
		if (PrimQ(c, verif))
			fprintf(out, ",\n");
		IntrQ(aux, pars);
		//free(verif);
	}
	ConcatQ(c, aux);
	DistrQ(&aux);
	free(pars);
}

void get_task(void *c_run, void *c_wait, void *c_fin, int ID, FILE *out)
{
	int ok = 0;
	Task pars;
	void *aux = InitQ(sizeof(Task));
	while (!VidaQ(c_run))
	{
		ExtrQ(c_run, &pars);
		if (pars.ID == ID)
		{
			fprintf(out, "Task %d is running (remaining_time = %d).\n", pars.ID, pars.exec_time_run);
			IntrQ(aux, &pars);
			while (!VidaQ(c_run))
			{
				ExtrQ(c_run, &pars);
				IntrQ(aux, &pars);
			}
			ConcatQ(c_run, aux);
			DistrQ(&aux);
			return;
		}
		IntrQ(aux, &pars);
	}
	ConcatQ(c_run, aux);
	while (!VidaQ(c_wait))
	{
		ExtrQ(c_wait, &pars);
		if (pars.ID == ID)
		{
			fprintf(out, "Task %d is waiting (remaining_time = %d).\n", pars.ID, pars.execution_time);
			IntrQ(aux, &pars);
			while (!VidaQ(c_wait))
			{
				ExtrQ(c_wait, &pars);
				IntrQ(aux, &pars);
			}
			ConcatQ(c_wait, aux);
			DistrQ(&aux);
			return;
		}
		IntrQ(aux, &pars);
	}
	ConcatQ(c_wait, aux);
	while (!VidaQ(c_fin))
	{
		ExtrQ(c_fin, &pars);
		if (pars.ID == ID)
		{
			fprintf(out, "Task %d is finished (executed_time = %d).\n", pars.ID, pars.execution_time);
			IntrQ(aux, &pars);
			while (!VidaQ(c_fin))
			{
				ExtrQ(c_fin, &pars);
				IntrQ(aux, &pars);
			}
			ConcatQ(c_fin, aux);
			DistrQ(&aux);
			return;
		}
		IntrQ(aux, &pars);
	}
	ConcatQ(c_fin, aux);
	fprintf(out, "Task %d not found.\n", ID);
	DistrQ(&aux);
}

void get_thread(void *c_run, void *stiva, int thread, FILE *out)
{
	Task *pars = malloc(sizeof(Task));
	void *aux = InitQ(sizeof(Task));
	Thread *pars_thread = malloc(sizeof(Thread));
	void *stiva_aux = InitS(sizeof(Thread));
	int ok = 0;
	while (!VidaS(stiva))
	{
		Pop(stiva, pars_thread);
		if (pars_thread->ID == thread)
		{
			fprintf(out, "Thread %d is idle.\n", thread);
			ok = 1;
		}
		Push(stiva_aux, pars_thread);
	}
	Rastoarna(stiva, stiva_aux);
	while (!VidaQ(c_run) && ok == 0)
	{
		ExtrQ(c_run, pars);
		if (pars->curent_thread.ID == thread)
		{
			fprintf(out, "Thread %d is running task %d (remaining_time = %d).\n", thread, pars->ID, pars->exec_time_run);
		}
		IntrQ(aux, pars);
	}
	ConcatQ(c_run, aux);
	free(pars);
	free(pars_thread);
	DistrQ(&aux);
	DistrS(&stiva_aux);
}

void finish(void **c_wait, void **c_run, void **c_fin, void **stiva, int Q, int total_time, FILE *out, void **stiva_id)
{
	Thread *crt_thread = malloc(sizeof(Thread));
	Task *pars = malloc(sizeof(Task));
	void *aux = InitQ(sizeof(Task));
	int contor = 0;
	int ok = 0;
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
		int crt_time = max_run(*c_run);
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
				inserare_stiva(&(*stiva_id), push_thread.ID);
				Push(*stiva, &push_thread);
				IntrQ(*c_fin, pars);
				contor++;
			}
			else
			{
				pars->exec_time_run -= time;
				IntrQ(aux, pars);
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
	in = fopen(argv[1], "r");
	out = fopen(argv[2], "w");
	fscanf(in, "%d", &Q);
	fscanf(in, "%d\n", &C);
	char *line = malloc(1000);
	int n = 2 * C;
	void *c_wait = NULL, *c_run = NULL, *c_finish = NULL, *stiva = NULL;
	c_wait = InitQ(sizeof(Task));
	c_run = InitQ(sizeof(Task));
	c_finish = InitQ(sizeof(Task));
	int *cont_ID = (int *)calloc(MAX + 2, sizeof(int));
	stiva = InitS(sizeof(Thread));
	void *stiva_id = InitS(sizeof(int));
	int total_time = 0;
	//printf("%d\n", n);
	for (int i = n - 1; i >= 0; i--)
	{
		Thread *new_thread = malloc(sizeof(Thread));
		new_thread->ID = i;
		Push(stiva, new_thread);
		free(new_thread);
	}
	for (int i = MAX; i >= 1; i--)
	{
		Push(stiva_id, &i);
	}
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
			int priority = atoi(word);
			while (number_task > 0)
			{
				Task *crt_task = malloc(sizeof(Task));
				int ok = 0;
				// for (int i = 1; i <= MAX && ok == 0; i++)
				// {
				// 	if (cont_ID[i] == 0)
				// 	{
				// 		crt_task->ID = i;
				// 		cont_ID[i] = 1;
				// 		ok = 1;
				// 	}
				// }
				int id;
				Pop(stiva_id, &id);
				crt_task->ID = id;
				fprintf(out, "Task created successfully : ID %d.\n", crt_task->ID);
				crt_task->exec_time_run = exec_time;
				crt_task->execution_time = exec_time;
				crt_task->priority = priority;
				sort_coada_wait(&c_wait, crt_task);
				number_task--;
				free(crt_task);
			}
		}
		if (strcmp(word, "print") == 0)
		{
			word = strtok(NULL, " \n");
			if (strcmp(word, "waiting") == 0)
			{
				//printf("\n");
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
			if (time > Q)
			{
				while (time >= Q)
				{
					running(&c_wait, &c_run, &c_finish, &stiva, Q, &stiva_id, &total_time, out);
					time = time - Q;
				}
			}
			if (time >= 0)
			{
				running(&c_wait, &c_run, &c_finish, &stiva, time, &stiva_id, &total_time, out);
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
			int thread;
			word = strtok(NULL, " \n");
			thread = atoi(word);
			if (thread < n)
				get_thread(c_run, stiva, thread, out);
		}
		if (strcmp(word, "finish") == 0)
		{
			finish(&c_wait, &c_run, &c_finish, &stiva, Q, total_time, out, &stiva_id);
		}
	}
	DistrQ(&c_wait);
	DistrQ(&c_run);
	DistrQ(&c_finish);
	DistrS(&stiva);
	DistrS(&stiva_id);
	free(cont_ID);
	free(line);
	fclose(in);
	fclose(out);
	return 0;
}
