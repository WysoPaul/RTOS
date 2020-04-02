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

typedef struct{
    char c;
    int (* func)(int argc, char ** argv);
    char * description;
} shell_func_t;

int shell_func_list_size = 0;
shell_func_t shell_func_list[_SHELL_FUNC_LIST_MAX_SIZE];

int dataReady = 0;


/******************************************
 * UART READ
 *
 ******************************************/

char uart_read() {
	char c;

//	HAL_UART_Receive_IT(&UART_DEVICE, (uint8_t*)(&c), 1);
//	while (!dataReady);
//	dataReady = 0;

//	while(HAL_UART_Receive(&UART_DEVICE, (uint8_t*)(&c), 1, 0xFFFFFFFF) == HAL_TIMEOUT)
//	{
//		printf("HAL_UART_Receive timeout. This should not cause any issue.\r\n");
//	}

	HAL_UART_Receive_IT(&UART_DEVICE, (uint8_t*)(&c), 1);

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
 * SHELL INIT
 *
 ******************************************/

void shell_init() {
	printf("\r\n\r\n===== Monsieur Shell v0.2 =====\r\n");

	shell_add('h', sh_help, help);

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
char prompt[] = "> ";


/******************************************
 * SHELL RUN
 *
 ******************************************/
int shell_run(){
	int reading = 0;
	int pos = 0;

	while (1)
	{
		  uart_write(prompt, 2);
		  reading = 1;

		  while(reading){
			  xSemaphoreTake(MonSemUART, portMAX_DELAY);
			  char c = uart_read();

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
