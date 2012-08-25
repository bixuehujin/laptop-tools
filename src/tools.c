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

void on_power_state_changed(int online){
	logger_debug("power state changed: %d", online);


}
void on_backlight_changed(int level, int max){
	logger_log(LOGGER_INFO, "backlight changed: current level %d, max level %d", level, max);
}
int main(void) {
	/*
	if(daemon(0, 0) < 0) {
		printf("daemon() failed\n");
	}
	*/
	logger_init("laptop-tools");


	lt_monitor_t * monitor = lt_monitor_new();
	lt_event_t * le = lt_monitor_get_event(monitor);
	lt_event_bind(le, "power_state_changed", on_power_state_changed);
	lt_event_bind(le, "backlight_changed", on_backlight_changed);
	printf("the current backlight level is %s\n", lt_monitor_get_bright_attr(monitor, "brightness"));
	printf("the max backlight level is %s\n", lt_monitor_get_bright_attr(monitor, "max_brightness"));
	lt_monitor_run(monitor);

	lt_monitor_unref(monitor);
	logger_close();
	return 0;
}
