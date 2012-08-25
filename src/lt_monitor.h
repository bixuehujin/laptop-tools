/*
 * lt_monitor.h
 *
 *  Created on: 2012-8-22
 *      Author: hujin
 */



#ifndef LT_MONITOR_H_
#define LT_MONITOR_H_
#include "lt_event.h"

#define LM_GROUP_POWER 1
#define LM_GROUP_BRIGHT 2

typedef struct _lt_monitor {
	lt_event_t * le;
	struct udev * udev;
	struct udev_monitor * monitor;
	struct udev_enumerate * enumerate;
	struct udev_device * _device;
}lt_monitor_t;



lt_monitor_t * lt_monitor_new();
char * lt_monitor_get_attr(lt_monitor_t * lm,int group ,const char * name);
int lt_monitor_get_udev_fd(lt_monitor_t * lm);
void lt_monitor_run(lt_monitor_t * lm);
#endif /* LT_MONITOR_H_ */
