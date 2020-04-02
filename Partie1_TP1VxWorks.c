#include <stdio.h>
#include <taskLib.h>
#include <semLib.h>
SEM_ID MonSem;


void fonction1 (void);
void fonction2 (void);

void init (void)
{
	int tache1ID, tache2ID, priorite;
	//creation semaphore vide
	MonSem=semBCreate(SEM_Q_FIFO,SEM_EMPTY);
	printf("Hello\n\n");
	
	taskPriorityGet (0,&priorite);
	printf("Creation tache 2\n");
	tache2ID=taskSpawn("tache2",priorite+1,0,1000,(FUNCPTR)&fonction2,0,0,0,0,0,0,0,0,0,0);
	
	printf("Creation tache 1\n");
	tache1ID=taskSpawn("tache1",priorite+2,0,1000,(FUNCPTR)&fonction1,0,0,0,0,0,0,0,0,0,0);	
	
	printf("Tache INIT x_x\n");
}


//fonction1 reveil fonction2
void fonction1 (void)
{
	int i=0;
	
	printf("T1: BONJOUR\n");
	
	while (1)
	{
		
		printf("T1: Je fais un traaaaaaaaaaitement super Lonnnnnnnnnnnnnnnnnnng\n");
		//remplir semaphore plein
		printf("T1: Je remplis le semaphore\n");	
		semGive(MonSem);
		
		printf("T1: J'ai (re)pris la main, je m'endore\n");
		taskDelay(100);
		printf ("T1: reveille %d\n", i++);
	}
}

//fonction2 est reveille par fonction1
void fonction2 (void)
{
	int i=0;
	printf("T2: BONJOUR\n");
	
	while (1)
	{
		//prendre
		printf("T2: Je prend le semaphore\n");
		semTake(MonSem, WAIT_FOREVER);
		printf("T2, J'ai la main, ReCoucou %d\n", i++);
		printf("T2, Je fais mon traitement (synchronise)\n");
	}
}
