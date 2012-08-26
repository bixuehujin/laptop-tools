/*
 * lt_settings.h
 *
 *  Created on: 2012-8-25
 *      Author: hujin
 */

#ifndef LT_SETTINGS_H_
#define LT_SETTINGS_H_

#include <glib.h>

typedef struct _lt_settings {
	char * file;
	GKeyFile * ins;
	int dirty;
}lt_settings_t;

lt_settings_t * lt_settings_new(const char * filename);

int lt_settings_get_backlight(lt_settings_t * lts, int ac_online);

int lt_settings_get_touchpad_setting(lt_settings_t * lts);

void lt_settings_set_backlight(lt_settings_t * lts, int ac_online, int backlight);

void lt_settings_set_touchpad_setting(lt_settings_t * lts, int state);

int lt_settings_is_use_separate_backlight(lt_settings_t * lts);

void lt_settings_set_use_separate_backlight(lt_settings_t * lts);

void lt_settings_flush(lt_settings_t * lts);

void lt_settings_destroy(lt_settings_t * lts);

#endif /* LT_SETTINGS_H_ */
