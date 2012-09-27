/*
 * lt_setter.c
 *
 *  Created on: 2012-8-26
 *      Author: hujin
 */


#include "lt_device.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libudev.h>
#include "logger.h"
#include "errno.h"
#include "lt_monitor.h"
#include "lt_instance.h"


extern lt_instance_t lt_instance;

static char * backlist[] = {
		"11/2/a/0",
		"11/2/5/7326",
		NULL
};

int lt_device_is_mouse(struct udev_device * device) {
	const char * product;
	if(!(product = udev_device_get_property_value(device, "PRODUCT"))) {
		return 0;
	}

	int i;
	for(i = 0; backlist[i]; i ++) {
		if(strcmp(backlist[i], product) == 0) {
			return 0;
		}
	}

	return 1;
}


void lt_device_set_backlight(int backlight) {
	int fd = open("/sys/class/backlight/acpi_video0/brightness", O_WRONLY);
	if(fd == -1) {
		logger_log(LOGGER_WARNING, strerror(errno));
		return;
	}
	char str[3] = {0};
	snprintf(str, 3, "%d", backlight);
	if(write(fd, str, strlen(str)) == -1) {
		logger_log(LOGGER_WARNING, strerror(errno));
	}
	close(fd);
}


int lt_device_is_power_online() {
	int fd = open("/sys/class/power_supply/AC/online", O_RDONLY);
	if(fd == -1) {
		logger_log(LOGGER_WARNING, strerror(errno));
		return -1;
	}
	char str[3] = {0};
	read(fd, &str, sizeof(str));
	return atoi(str);
}


/**
 * ctrl : 1 for enable touchpad , 0 disable touchpad
 */
int lt_device_control_touchpad(int ctl) {

	char * display = getenv("DISPLAY");
	logger_log(LOGGER_INFO, "using display %s", display);
	setenv("DISPLAY", ":0.0", 1);
	char cmd[100] = {0};
	sprintf(cmd, "/usr/bin/synclient TouchpadOff=%d >> /home/hujin/synclient.log", ctl ? 0 : 1);
	int ret = system(cmd);
	logger_log(LOGGER_INFO, "execute command synclient( %s ) with exit status %d\n", cmd, ret);
	return ret == 0 ? 1 : 0;
}


int lt_device_is_mouse_plugged() {
	struct udev * udev = udev_new();
	struct udev_enumerate * enumerate = udev_enumerate_new(udev);

	udev_enumerate_add_match_subsystem(enumerate, "input");
	udev_enumerate_add_match_property(enumerate, "ID_INPUT_MOUSE", "1");
	udev_enumerate_scan_devices(enumerate);

	struct udev_list_entry * entrys, * entry;
	const char * path, * product;
	struct udev_device * device;
	int ret = 0;

	entrys =udev_enumerate_get_list_entry(enumerate);

	udev_list_entry_foreach(entry, entrys) {
		path = udev_list_entry_get_name(entry);
		device = udev_device_new_from_syspath(udev, path);

		if(device && lt_device_is_mouse(device)) {
			ret ++;
			udev_device_unref(device);
		}
	}
	udev_enumerate_unref(enumerate);
	udev_unref(udev);

	return ret;
}
