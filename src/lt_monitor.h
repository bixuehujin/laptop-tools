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

void lt_monitor_init(lt_monitor_t * lm);

void lt_monitor_run(lt_monitor_t * lm);

char * lt_monitor_get_attr(lt_monitor_t * lm,int group ,const char * name);

const char * lt_monitor_get_bright_attr(lt_monitor_t * lm, const char * name);

int lt_monitor_is_power_online(lt_monitor_t * lm);

int lt_monitor_get_backlight(lt_monitor_t * lm, int * max);

lt_event_t * lt_monitor_get_event(lt_monitor_t * lm);

void lt_monitor_unref(lt_monitor_t * lm);

const char * lt_monitor_get_touchpad_name(lt_monitor_t * lm);

#endif /* LT_MONITOR_H_ */
