/*
 * lt_instance.c
 *
 *  Created on: 2012-8-26
 *      Author: hujin
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "logger.h"
#include "lt_instance.h"
#include "lt_monitor.h"
#include "handlers.h"



lt_instance_t lt_instance;


void lt_instance_init(int argc, char *argv[]) {
	logger_init("laptop-tools");
	lt_instance.lts = lt_settings_new("/cproject/lt/src/lt.conf");
	lt_instance.ltm = lt_monitor_new();
	lt_instance.lte = lt_monitor_get_event(lt_instance.ltm);
}


void lt_instance_main() {
	lt_event_bind(lt_instance.lte, "power_state_changed", on_power_state_changed);
	lt_event_bind(lt_instance.lte, "backlight_changed", on_backlight_changed);
	lt_monitor_run(lt_instance.ltm);
}


void lt_instance_destroy() {

	lt_monitor_unref(lt_instance.ltm);
	lt_settings_destroy(lt_instance.lts);
	logger_close();
}
