/*
 * lt_instance.c
 *
 *  Created on: 2012-8-26
 *      Author: hujin
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "logger.h"
#include "lt_instance.h"
#include "lt_monitor.h"
#include "handlers.h"



lt_instance_t lt_instance;
void do_print_help();
void do_print_version();
void do_run(int as_daemon);


void lt_parse_arguments(int argc, char * argv[]) {
	int c, arg_count = 0, invalid = 0;
	while(1) {
		int option_index = 0;
		static struct option long_options[] = {
			{"help", 	no_argument, 	0, 'h'},
			{"daemon", 	no_argument, 	0, 'd'},
			{"version", no_argument, 	0, 'v'},
			{0, 		0, 				0,  0 }
		};
		c = getopt_long(argc, argv, "hdv", long_options, &option_index);
		if(c == -1) {
			break;
		}
		switch(c) {
			case 'h':
				do_print_help();
				break;

			case 'd':{
				do_run(1);
				break;

			case 'v':
				do_print_version();
				break;
			case '?':
				invalid = 1;
				break;
			default:
				printf("No arguments");
				break;
			}
		}
		arg_count ++;

	}

	if(arg_count == 0) { // run in terminal
		do_run(0);
	}
	if(invalid) {
		do_print_help();
	}
}


void lt_instance_init(int argc, char *argv[]) {
	lt_parse_arguments(argc, argv);

	lt_instance.lts = lt_settings_new("/cproject/lt/src/lt.conf");
	lt_instance.ltm = lt_monitor_new();
	lt_instance.lte = lt_monitor_get_event(lt_instance.ltm);
	signal(SIGTERM, on_signal_term);
}


void lt_instance_main() {
	lt_event_bind(lt_instance.lte, "power_state_changed", on_power_state_changed);
	lt_event_bind(lt_instance.lte, "backlight_changed", on_backlight_changed);
	lt_event_bind(lt_instance.lte, "mouse_state_changed", on_mouse_state_changed);

	lt_monitor_run(lt_instance.ltm);
}


void lt_instance_destroy() {
	lt_monitor_unref(lt_instance.ltm);
	lt_settings_destroy(lt_instance.lts);
	logger_close();
}


void do_print_help() {
	printf("Usage: lt [--help|--daemon|--version]\n");
	printf("    -h, --help        display this help and exit.\n");
	printf("    -d, --daemon      run the program as system daemon.\n");
	printf("    -v, --version     output version information and exit.\n");
	exit(0);
}


void do_print_version() {
	printf("ls 0.01\n");
	exit(0);
}


void do_run(int as_daemon) {
	if(geteuid() != 0) {
		printf("root permission needed to run the program.\n");
		exit(1);
	}
	if(as_daemon) {
		if(daemon(0, 0) < 0) {
			printf("daemon() failed\n");
		}
	}
	logger_init("laptop-tools", as_daemon);
}

