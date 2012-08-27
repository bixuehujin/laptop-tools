/*
 * handlers.c
 *
 *  Created on: 2012-8-26
 *      Author: hujin
 */

#include "logger.h"
#include "lt_instance.h"
#include "lt_device.h"
#include "lt_settings.h"

extern lt_instance_t lt_instance;

void on_power_state_changed(int online){
	logger_debug("power state changed: %d", online);
	int backlight = lt_settings_get_backlight(lt_instance.lts, online);
	lt_device_set_backlight(backlight);
}


void on_backlight_changed(int level, int max){
	logger_log(LOGGER_INFO, "backlight changed: current level %d, max level %d", level, max);
	int online = lt_device_is_power_online();
	if(online < 0) {
		logger_log(LOGGER_WARNING, "Fetch power state failed.");
		return;
	}
	lt_settings_set_backlight(lt_instance.lts, online, level);
	lt_settings_flush(lt_instance.lts);
}


void on_signal_term() {
	logger_debug("Received term signal, Exiting ...");
	lt_instance_destroy();
	exit(0);
}

