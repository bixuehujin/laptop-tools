/*
 * lt_instance.h
 *
 *  Created on: 2012-8-26
 *      Author: hujin
 */

#ifndef LT_INSTANCE_H_
#define LT_INSTANCE_H_

#include "lt_event.h"
#include "lt_monitor.h"
#include "lt_settings.h"

typedef struct lt_instance {
	lt_settings_t * lts;
	lt_monitor_t * ltm;
	lt_event_t * lte;
}lt_instance_t;


void lt_instance_init(int argc, char *argv[]);

void lt_instance_main();

void lt_instance_destroy();


#endif /* LT_INSTANCE_H_ */
