/*
 * lt_monitor.c
 *
 *  Created on: 2012-8-22
 *      Author: hujin
 */

#include "lt_monitor.h"
#include "logger.h"
#include "lt_device.h"
#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>


void lt_monitor_init(lt_monitor_t * lm);
int lt_monitor_is_mouse_plugged(lt_monitor_t * lm);


lt_monitor_t * lt_monitor_new() {
	lt_monitor_t * ret = malloc(sizeof(lt_monitor_t));
	if(!ret) return NULL;
	memset(ret, 0, sizeof(lt_monitor_t));
	ret->le = lt_event_new();
	if(!ret->le) {
		free(ret);
		return NULL;
	}
	lt_monitor_init(ret);
	/*
	struct udev_device * dev;
	struct udev_list_entry * dev_list_entry, * devices;
	ret->enumerate = udev_enumerate_new(ret->udev);
	udev_enumerate_add_match_subsystem(ret->enumerate, "hidraw");
	udev_enumerate_scan_devices(ret->enumerate);
	devices = udev_enumerate_get_list_entry(ret->enumerate);
	udev_list_entry_foreach(dev_list_entry, devices) {
		const char *path;
		path = udev_list_entry_get_name(dev_list_entry);
		dev = udev_device_new_from_syspath(ret->udev, path);
		printf("Device Node Path: %s\n", udev_device_get_devnode(dev));

	}
	*/
	return ret;
}


void lt_monitor_init(lt_monitor_t * lm) {
	lm->udev = udev_new();
	lm->enumerate = udev_enumerate_new(lm->udev);
	lm->monitor = udev_monitor_new_from_netlink(lm->udev, "udev");
	udev_monitor_filter_add_match_subsystem_devtype(lm->monitor, "hidraw", NULL);
	udev_monitor_filter_add_match_subsystem_devtype(lm->monitor, "backlight", NULL);
	udev_monitor_filter_add_match_subsystem_devtype(lm->monitor, "power_supply", NULL);
	udev_monitor_filter_add_match_subsystem_devtype(lm->monitor, "input", NULL);
}


void lt_monitor_run(lt_monitor_t * lm) {

	lt_event_trigger(lm->le, "init");

	udev_monitor_enable_receiving(lm->monitor);
	struct udev_device * dev;
	struct udev_list_entry * entrys, *entry;
	while(1) {
		dev = udev_monitor_receive_device(lm->monitor);
		lm->_device = dev;
		const char * subsystem = udev_device_get_subsystem(dev);

		/*
		if(dev) {
			printf("Got Devices\n");
			printf("   Node: %s\n", udev_device_get_devnode(dev));
			printf("   Subsystem: %s\n", udev_device_get_subsystem(dev));
			printf("   Devtype: %s\n", udev_device_get_devtype(dev));
			printf("   Action: %s\n",udev_device_get_action(dev));
			printf("   Driver: %s\n",udev_device_get_driver(dev));
			printf("   brightness: %s\n",udev_device_get_property_value(dev, "brightness"));
			printf("\n");
			printf("props\n");
			entrys = udev_device_get_properties_list_entry(dev);
			udev_list_entry_foreach(entry, entrys) {
				const char * name = udev_list_entry_get_name(entry);
				const char * value = udev_list_entry_get_value(entry);
				printf("	%s:%s\n", name, value);
			}
			entrys = udev_device_get_sysattr_list_entry(dev);
			printf("sysattr:\n");
			udev_list_entry_foreach(entry, entrys) {
				const char * name = udev_list_entry_get_name(entry);
				printf("	%s:%s\n", name, udev_device_get_sysattr_value(dev, name));
			}

			printf("tags:\n");
			entrys = udev_device_get_tags_list_entry(dev);
			printf("sysattr:\n");
			udev_list_entry_foreach(entry, entrys) {
				const char * name = udev_list_entry_get_name(entry);
				const char * value = udev_list_entry_get_value(entry);
				printf("	%s:%s\n", name, value);
			}

		}else {
			printf("No Devices, An error occured\n");
		}
		*/

		if(dev) {
			if(strcmp(subsystem, "power_supply") == 0) {
				int online = lt_monitor_is_power_online(lm);
				if(online != -1) {
					lt_event_trigger(lm->le, "power_state_changed", online);
				}
			}else if(strcmp(subsystem, "backlight") == 0) {
				int backlight, max_backlight;
				backlight = lt_monitor_get_backlight(lm, &max_backlight);
				if(backlight != -1) {
					lt_event_trigger(lm->le, "backlight_changed", backlight, max_backlight);
				}
			}else if(strcmp(subsystem, "input") ==  0) {
				if(lt_device_is_mouse(dev)) {
					lt_event_trigger(lm->le, "mouse_state_changed", lt_monitor_is_mouse_plugged(lm));
				}
			}
			udev_device_unref(lm->_device);
			lm->_device = NULL;
		}

	}
}


char * lt_monitor_get_attr(lt_monitor_t * lm,int group ,const char * name) {
	switch(group) {
	case LM_GROUP_BRIGHT:

		break;

	case LM_GROUP_POWER:

		break;
	default:

		break;
	}
	return NULL;
}


/**
 * name can be :backlight max_backlight
 */
const char * lt_monitor_get_bright_attr(lt_monitor_t * lm, const char * name) {
	struct udev_device * dev;
	dev = udev_device_new_from_syspath(lm->udev, "/sys/class/backlight/acpi_video0");
	const char * level =  udev_device_get_sysattr_value(dev, name);
	return level;
}


/**
 * 1 for online , 0 for offline  . otherwise -1 returned
 */
int lt_monitor_is_power_online(lt_monitor_t * lm) {
	const char * online = udev_device_get_sysattr_value(lm->_device, "online");
	//printf("----%s----\n", online);
	if(!online) return -1;

	return strcmp(online, "1") == 0 ? 1 : 0;
}


int lt_monitor_get_backlight(lt_monitor_t * lm, int * max) {
	static int olevel = -1;
	const char * _level = udev_device_get_sysattr_value(lm->_device, "brightness");
	int level = atoi(_level);

	if(max) {
		const char * _level_total = udev_device_get_sysattr_value(lm->_device, "max_brightness");
		*max = atoi(_level_total);
	}
	if(level == olevel) {
		return -1;
	}else {
		olevel = level;
		return level;
	}
}


lt_event_t * lt_monitor_get_event(lt_monitor_t * lm) {
	return lm->le;
}


int lt_monitor_is_mouse_plugged(lt_monitor_t * lm) {
	struct udev_enumerate * enumerate = lm->enumerate;

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
		device = udev_device_new_from_syspath(lm->udev, path);

		if(device && lt_device_is_mouse(device)) {
			ret ++;
			udev_device_unref(device);
		}
	}

	return ret;
}


const char * lt_monitor_get_touchpad_name(lt_monitor_t * lm) {

	struct udev_enumerate * enumerate = udev_enumerate_new(lm->udev);
	udev_enumerate_add_match_subsystem(enumerate, "input");
	udev_enumerate_add_match_property(enumerate, "ID_INPUT_TOUCHPAD", "1");
	udev_enumerate_scan_devices(enumerate);

	struct udev_list_entry * entrys, * entry;
	const char * path, * product, *name;
	struct udev_device * device;
	static  char  ret[50] = {0};

	entrys =udev_enumerate_get_list_entry(enumerate);

	udev_list_entry_foreach(entry, entrys) {
		path = udev_list_entry_get_name(entry);
		device = udev_device_new_from_syspath(lm->udev, path);
		name = udev_device_get_sysattr_value(device, "name");

		if(name) {
			strcpy(ret, name);
		}
		udev_device_unref(device);
		if(name) {
			return ret;
		}
	}

	return NULL;
}


void lt_monitor_unref(lt_monitor_t * lm) {
	udev_enumerate_unref(lm->enumerate);
	udev_monitor_unref(lm->monitor);
	udev_unref(lm->udev);
	lt_event_unref(lm->le);
	free(lm);
}
