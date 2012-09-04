/*
 * lt_event.h
 *
 *  Created on: 2012-8-22
 *      Author: hujin
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef LT_EVENT_H_
#define LT_EVENT_H_

typedef void (*power_state_changed_handler_t)(int ac_online);
typedef void (*mouse_state_changed_handler_t)(int mouse_count);
typedef void (*backlight_changed_handler_t)();
typedef void (*init_handler_t)();
typedef void (*handler_t)();

struct event_item{
	char name[50];
	handler_t handler;
	int used;
};



typedef struct _lt_event{
	int refcount;
	int table_size;
	int max_size;
	struct event_item  items[20];
}lt_event_t;

lt_event_t * lt_event_new();
void lt_event_bind(lt_event_t * le, const char * name, handler_t handler);
void lt_event_unbind(lt_event_t * le, const char * name);
void lt_event_unref(lt_event_t * le);

#define lt_event_trigger(le, _name, ...) {				\
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




#endif /* LT_EVENT_H_ */
