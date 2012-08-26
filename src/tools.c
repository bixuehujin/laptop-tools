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
#include "lt_event.h"
#include "lt_monitor.h"
#include "lt_settings.h"
#include "handlers.h"








int main(int argc, char * argv[]) {
	/*
	if(daemon(0, 0) < 0) {
		printf("daemon() failed\n");
	}
	*/


	lt_instance_init(argc, argv);
	lt_instance_main();
	lt_instance_destroy();


	return 0;
}
