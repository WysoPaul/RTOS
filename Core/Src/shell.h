/*
 * shell.h
 *
 *  Created on: 7 juin 2019
 *      Author: laurent
 */

#ifndef INC_LIB_SHELL_SHELL_H_
#define INC_LIB_SHELL_SHELL_H_

#include <stdint.h>

#define UART_DEVICE huart1

#define _SHELL_FUNC_LIST_MAX_SIZE 64

char uart_read();
//	Lit les caractères reçu sur l'UART_DEVICE; caractère par caractrère

int uart_write(char * s, uint16_t size);
// Echo de caractères tappé
// Printf du prompt, retour des commandes, ...

void uart_data_ready();
// Surveille l'UART ???

//void shell_char_received();
//inexistant
//Regroupe un ensemble de caractères constituant une commande (et terminé par un CR/LF)

void shell_init();
// Affiche message welcom
// initialise le tableau des commandes supportés

int shell_add(char c, int (* pfunc)(int argc, char ** argv), char * description);
// Fonction pour ajouter une commande à la liste de commandes

int shell_exec(char c, char * buf);
//

int shell_run();
//Regroupe un ensemble de caractères constituant une commande (et terminé par un CR/LF)

#endif /* INC_LIB_SHELL_SHELL_H_ */
