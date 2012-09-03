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
#include "logger.h"
#include "errno.h"

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
	char cmd[50] = {0};
	sprintf(cmd, "synclient Touchpadoff=%d", ctl ? 0 : 1);
	int ret = system(cmd);
	return ret == 0 ? 1 : 0;
}
