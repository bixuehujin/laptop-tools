/*
 * lt_settings.c
 *
 *  Created on: 2012-8-25
 *      Author: hujin
 */


#include "lt_settings.h"
#include "logger.h"

lt_settings_t * lt_settings_new(const char * filename) {
	lt_settings_t * ret = malloc(sizeof(lt_settings_t));
	if(!ret) return NULL;
	GError *error = NULL;

	ret->file = strdup(filename);
	ret->ins = g_key_file_new();
	ret->dirty = 0;
	if(!g_key_file_load_from_file(ret->ins, filename, G_KEY_FILE_KEEP_COMMENTS, &error)) {
		logger_log(LOGGER_WARNING, "Cannot load config file '%s', %s", filename, error->message);
	}

	return ret;
}


int _lt_settings_get_integer(lt_settings_t * lts, char * group, char *name) {
	int ret;
	GError * error = NULL;
	ret = g_key_file_get_integer(lts->ins, group, name, &error);
	if(error) {
		logger_log(LOGGER_WARNING, "Error on parse config, %s", error->message);
		g_error_free(error);
	}
	return ret;
}


int lt_settings_get_backlight(lt_settings_t * lts, int ac_online) {
	int backlight = 2;
	if(ac_online) {
		backlight = _lt_settings_get_integer(lts, "backlight", "power_backlight");
	}else {
		backlight = _lt_settings_get_integer(lts, "backlight", "batt_backlight");
	}
	return backlight;
}


int lt_settings_get_touchpad_setting(lt_settings_t * lts) {
	return _lt_settings_get_integer(lts, "touchpad", "disabled");
}


int lt_settings_is_use_separate_backlight(lt_settings_t * lts) {
	return _lt_settings_get_integer(lts, "backlight", "use_separate_backlight");
}


void lt_settings_set_backlight(lt_settings_t * lts, int ac_online, int backlight) {
	if(ac_online) {
		g_key_file_set_integer(lts->ins, "backlight", "power_backlight", backlight);
	}else {
		g_key_file_set_integer(lts->ins, "backlight", "batt_backlight", backlight);
	}
	lts->dirty = 1;
}


void lt_settings_set_touchpad_setting(lt_settings_t * lts, int state) {
	lts->dirty = 1;
	g_key_file_set_integer(lts->ins, "touchpad", "disabled", state);
}


void lt_settings_set_use_separate_backlight(lt_settings_t * lts) {
	g_key_file_set_boolean(lts->ins, "backlight", "use_separate_backlight", TRUE);
	lts->dirty = 1;
}


void lt_settings_flush(lt_settings_t * lts) {
	char * data = NULL;
	size_t len;
	if(lts->dirty) {
		data = g_key_file_to_data(lts->ins, &len, NULL);

		FILE * fp = fopen(lts->file, "w");
		fwrite(data, len, 1, fp);
		fclose(fp);

		g_free(data);
		lts->dirty = 0;
	}
}


void lt_settings_destroy(lt_settings_t * lts) {
	lt_settings_flush(lts);
	g_key_file_free(lts->ins);
	free(lts->file);
	free(lts);
}
