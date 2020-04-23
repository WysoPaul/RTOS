/*
 * shell.c
 *
 *  Created on: 7 juin 2019
 *      Author: Laurent Fiack
 */

#include "shell.h"
#include "LibPerso.h"

#include <stdio.h>

#include "usart.h"
#include "gpio.h"


#define ARGC_MAX 8
char help[] = "This is help message";
char ls[] = "Afficher la liste des fichiers";

typedef struct{
    char c;
    int (* func)(int argc, char ** argv);
    char * description;
} shell_func_t;

int shell_func_list_size = 0;
shell_func_t shell_func_list[_SHELL_FUNC_LIST_MAX_SIZE];

int dataReady = 0;


/******************************************
 * UART READ without IT
 *
 ******************************************/

char uart_read() {
	char c;

//	HAL_UART_Receive_IT(&UART_DEVICE, (uint8_t*)(&c), 1);
//	while (!dataReady);
//	dataReady = 0;

	while(HAL_UART_Receive(&UART_DEVICE, (uint8_t*)(&c), 1, 0xFFFFFFFF) == HAL_TIMEOUT)
	{
		printf("HAL_UART_Receive timeout. This should not cause any issue.\r\n");
	}

	return c;
}


/******************************************
 * UART WRITE
 *
 ******************************************/

int uart_write(char * s, uint16_t size) {
	HAL_UART_Transmit(&UART_DEVICE, (uint8_t*)s, size, 0xFFFF);
	return size;
}


/******************************************
 * UART DATA READY
 *
 *... unused
 ******************************************/

void uart_data_ready() {
	dataReady = 1;
}


/******************************************
 * SHELL HELP
 *
 ******************************************/

int sh_help(int argc, char ** argv) {
    int i;
    for(i = 0 ; i < shell_func_list_size ; i++) {
        printf("%c %s\r\n", shell_func_list[i].c, shell_func_list[i].description);
    }

    return 0;
}

/******************************************
 * SHELL list
 *
 ******************************************/

int sh_ls(int argc, char ** argv) {
    printf("Merci d'avoir lance la CMD 'l' pour lister les ficheir\r\nMais en faite il n'y a pas de système de fichier d'installe.\r\nDesole :'-(\r\n");

    return 0;
}





/******************************************
 * SHELL INIT
 *
 ******************************************/

void shell_init() {
	printf("\r\n\r\n===== Monsieur Shell v0.2 =====\r\n");

	shell_add('h', sh_help, help);
	shell_add('l', sh_ls, ls);


	for (int i = 0 ; i < 3 ; i++) {

		HAL_Delay(200);
	}
}

/******************************************
 * SHELL ADD
 *
 ******************************************/

int shell_add(char c, int (* pfunc)(int argc, char ** argv), char * description) {
    if (shell_func_list_size < _SHELL_FUNC_LIST_MAX_SIZE) {
        shell_func_list[shell_func_list_size].c = c;
        shell_func_list[shell_func_list_size].func = pfunc;
        shell_func_list[shell_func_list_size].description = description;
        shell_func_list_size++;
        return 0;
    }

    return -1;
}

/******************************************
 * SHELL EXE
 *
 ******************************************/

int shell_exec(char c, char * buf)
{
    int i;

    int argc;
    char * argv[ARGC_MAX];
    char *p;

    for(i = 0 ; i < shell_func_list_size ; i++) {
        if (shell_func_list[i].c == c) {
            argc = 1;
            argv[0] = buf;

            for(p = buf ; *p != '\0' && argc < ARGC_MAX ; p++){
                if(*p == ' ') {
                    *p = '\0';
                    argv[argc++] = p+1;
                }
            }

            return shell_func_list[i].func(argc, argv);
        }
    }
    printf("%c: no such command\r\n", c);
    return -1;
}

char buf[40];
char backspace[] = "\b \b";


/******************************************
 * SHELL RUN
 *
 ******************************************/
int shell_run(){
	int reading = 0;
	int pos = 0;
	char c = 0;

	printf("Debut Shell_Run\r\n");

	while (1)
	{
		  uart_write(prompt, 2);
		  reading = 1;

		  while(reading){
			  HAL_UART_Receive_IT(&UART_DEVICE, (uint8_t*)&c, 1);	//Amorcage de la lecture UART
			  xSemaphoreTake(MonSemUART, portMAX_DELAY);			//Attente du sémaphore ...

			  switch (c) {
				  //process RETURN key
				  case '\r':
					  //case '\n':
					  printf("\r\n");    //finish line
					  buf[pos++] = 0;     //to use cprintf...
					  printf(":%s\r\n", buf);
					  reading = 0;        //exit read loop
					  pos = 0;            //reset buffer
					  break;
					  //backspace
				  case '\b':
					  if (pos > 0) {      //is there a char to delete?
						  pos--;          //remove it in buffer

						  uart_write(backspace, 3);
					  }
					  break;
					  //other characters
				  default:
					  //only store characters if buffer has space
					  if (pos < sizeof(buf)) {
						  uart_write(&c, 1);
						  buf[pos++] = c; //store
					  }
			  }
		  }
		  shell_exec(buf[0], buf);
	}
	return 0;
}
