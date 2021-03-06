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
	if(lt_settings_is_use_separate_backlight(lt_instance.lts)) {
		lt_settings_set_backlight(lt_instance.lts, online, level);
	}else {
		lt_settings_set_backlight(lt_instance.lts, 1, level);
	}

	lt_settings_flush(lt_instance.lts);
}


void on_mouse_state_changed(int count) {
	logger_log(LOGGER_INFO, "mouse state changed, current mouse count %d\n", count);
	int state = lt_settings_get_touchpad_setting(lt_instance.lts);
	if(state == 2) {
		lt_device_control_touchpad(count == 0 ? 1 : 0);
	}
}


void on_init() {
	logger_log(LOGGER_INFO, "laptop-tools init completed");

	//mouse
	int use_separate , online;
	use_separate = lt_settings_is_use_separate_backlight(lt_instance.lts);
	if(use_separate) {
		online = lt_device_is_power_online();
		lt_device_set_backlight(lt_settings_get_backlight(lt_instance.lts, online));
	}else {
		lt_device_set_backlight(lt_settings_get_backlight(lt_instance.lts, 1));
	}

	//touchpad
	int touchpad_status = lt_settings_get_touchpad_setting(lt_instance.lts);
	if(touchpad_status == 2) {
		if(lt_monitor_is_mouse_plugged(lt_instance.ltm)) {
			lt_device_control_touchpad(0);
		}else {
			lt_device_control_touchpad(1);
		}
	}else if(touchpad_status == 3) {
		lt_device_control_touchpad(0);
	}
}


void on_signal_term() {
	logger_debug("Received term signal, Exiting ...");
	lt_instance_destroy();
	exit(0);
}

