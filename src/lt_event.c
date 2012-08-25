/*
 * lt_event.c
 *
 *  Created on: 2012-8-22
 *      Author: hujin
 */


#include "lt_event.h"
#include "logger.h"

lt_event_t * lt_event_new() {
	lt_event_t * ret = malloc(sizeof(lt_event_t));
	if(!ret) {
		return NULL;
	}
	memset(ret, 0, sizeof(ret));
	ret->refcount = 1;
	ret->table_size = 0;
	ret->max_size = 20;

	return ret;
}


void lt_event_bind(lt_event_t * le, const char * name, handler_t handler) {
	if(le->table_size == le->max_size ) {
		//TODO do realloc
	}

	struct event_item *item;
	int i;
	for(i = 0; i < le->max_size ; i ++) {
		item = &le->items[i];
		if(!item->used) {
			strcpy(item->name, name);
			item->handler = handler;
			if(i > le->table_size - 1) {
				le->table_size = i + 1;
			}
			item->used = 1;
			return;
		}
	}
	logger_log(LOGGER_ERR, "faild to bind signal for %s", name);
}
/*
void lt_event_trigger(lt_event_t * le, const char * name, ...) {
	int i;
	struct event_item * item;
	//logger_debug_ex("this size of signal table is %d", le->table_size);
	for(i = 0; i < le->table_size; i ++) {
		item = &le->items[i];
		logger_debug_ex("find signal %s %d", name, item->used);
		if(strcmp(name, item->name) == 0 && item->used) {
			handler_t handler = item->handler;
			handler();
		}
	}
}
*/

void lt_event_unbind(lt_event_t * le, const char * name) {
	int i;
	struct event_item *item;
	for(i = 0; i < le->table_size ; i ++) {
		item = &le->items[i];
		if(strcmp(name, item->name) == 0) {
			item->used = 0;
		}
	}
}


void lt_event_unref(lt_event_t * le) {
	if(le->refcount == 1) {
		free(le);
	}else {
		le->refcount --;
	}
}
