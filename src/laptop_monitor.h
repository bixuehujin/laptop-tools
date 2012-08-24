/*
 * laptop_monitor.h
 *
 *  Created on: 2012-8-22
 *      Author: hujin
 */



#ifndef LAPTOP_MONITOR_H_
#define LAPTOP_MONITOR_H_
#include "laptop_event.h"

#define LM_GROUP_POWER 1
#define LM_GROUP_BRIGHT 2

typedef struct _laptop_monitor {
	laptop_event_t * le;
	struct udev * udev;
	struct udev_monitor * monitor;
	struct udev_enumerate * enumerate;
	struct udev_device * _device;
}laptop_monitor_t;



laptop_monitor_t * laptop_monitor_new();
char * laptop_monitor_get_attr(laptop_monitor_t * lm,int group ,const char * name);
int laptop_monitor_get_udev_fd(laptop_monitor_t * lm);
void laptop_monitor_run(laptop_monitor_t * lm);
#endif /* LAPTOP_MONITOR_H_ */
