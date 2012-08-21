/*
 ============================================================================
 Name        : tools.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "logger.h"

int main(void) {
	/*
	if(daemon(0, 0) < 0) {
		printf("daemon() failed\n");
	}
	*/
	logger_init("laptop-tools");
	logger_log(LOGGER_ERR, "code:%d connect refused", 4);
	logger_debug("debug message there %d %s\n", 56, "haha");
	//sleep(14);
	puts("Hello Worsld"); /* prints Hello World */
	logger_close();
	return 0;
}
