/*
 * laptop_event.h
 *
 *  Created on: 2012-8-22
 *      Author: hujin
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef LAPTOP_EVENT_H_
#define LAPTOP_EVENT_H_

typedef void (*power_state_changed_handler_t)(int ac_online);
typedef void (*mouse_pluged_handler_t)();
typedef void (*mouse_unpluged_handler_t)();
typedef void (*backlight_changed_handler_t)();
typedef void (*handler_t)();

struct event_item{
	char name[50];
	handler_t handler;
	int used;
};



typedef struct _laptop_event{
	int refcount;
	int table_size;
	int max_size;
	struct event_item  items[20];
}laptop_event_t;

laptop_event_t * laptop_event_new();
void laptop_event_bind(laptop_event_t * le, const char * name, handler_t handler);
void laptop_event_unbind(laptop_event_t * le, const char * name);
void laptop_event_unref(laptop_event_t * le);

#define laptop_event_trigger(le, _name, ...) {				\
	int i;														\
	struct event_item * item;									\
	for(i = 0; i < le->table_size; i ++) {						\
		item = &le->items[i];									\
		logger_debug("find signal %s %d", _name);	\
		if(strcmp(_name, item->name) == 0 && item->used) {		\
			handler_t handler = item->handler;					\
			handler(__VA_ARGS__);								\
		}														\
	}															\
}




#endif /* LAPTOP_EVENT_H_ */
