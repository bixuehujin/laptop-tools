/*
 * mouse.c
 *
 *  Created on: 2012-9-3
 *      Author: hujin
 */

#include <libudev.h>
#include <stdio.h>

static char * backlist[] = {
		"11/2/a/0",
		"11/2/5/7326",
		NULL
};

int device_is_mouse(struct udev_device * device) {
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

void device_print_sysattr(struct udev_device * device) {
	struct udev_list_entry * entrys, * entry;
	entrys = udev_device_get_sysattr_list_entry(device);
	printf("Sysattrs:\n");
	udev_list_entry_foreach(entry, entrys) {
		const char * name = udev_list_entry_get_name(entry);
		const char * value = udev_device_get_sysattr_value(device, name);
		printf("	%s: %s\n", name, value);
	}
	printf("\n");
}


void device_print_props(struct udev_device * device) {
	struct udev_list_entry * entrys, * entry;
	entrys = udev_device_get_properties_list_entry(device);
	printf("Props:\n");
	udev_list_entry_foreach(entry, entrys) {
		const char * name = udev_list_entry_get_name(entry);
		const char * value = udev_device_get_property_value(device, name);
		printf("	%s: %s\n", name, value);
	}
	printf("\n");
}


int list_touchpad_devices() {
	struct udev *udev = udev_new();
		struct udev_enumerate * enumerate = udev_enumerate_new(udev);
		udev_enumerate_add_match_subsystem(enumerate, "input");
		//udev_enumerate_add_match_sysattr(enumerate, "ID_INPUT_MOUSE", "1");
		udev_enumerate_add_match_property(enumerate, "ID_INPUT_TOUCHPAD", "1");
		udev_enumerate_scan_devices(enumerate);

		struct udev_list_entry * entrys  =udev_enumerate_get_list_entry(enumerate);
		struct udev_list_entry * entry;
		const char * path;
		struct udev_device * device;
		udev_list_entry_foreach(entry, entrys) {
			path = udev_list_entry_get_name(entry);
			device = udev_device_new_from_syspath(udev, path);
			const char * product;
			//printf("find device: %s\n", path);
			//if(device && device_is_mouse(device)) {
				device_print_sysattr(device);
				device_print_props(device);
			//}
		}

		return 1;
}


int list_mouse_devices() {
	struct udev *udev = udev_new();
	struct udev_enumerate * enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(enumerate, "input");
	//udev_enumerate_add_match_sysattr(enumerate, "ID_INPUT_MOUSE", "1");
	udev_enumerate_add_match_property(enumerate, "ID_INPUT_MOUSE", "1");
	udev_enumerate_scan_devices(enumerate);

	struct udev_list_entry * entrys  =udev_enumerate_get_list_entry(enumerate);
	struct udev_list_entry * entry;
	const char * path;
	struct udev_device * device;
	udev_list_entry_foreach(entry, entrys) {
		path = udev_list_entry_get_name(entry);
		device = udev_device_new_from_syspath(udev, path);
		const char * product;
		//printf("find device: %s\n", path);
		if(device && device_is_mouse(device)) {
			device_print_sysattr(device);
			device_print_props(device);
		}
	}

	return 1;
}


int main(int argc, char **argv) {
	list_mouse_devices();
	printf("\n\n----------------touchpad------------------\n");
	list_touchpad_devices();
	int i;
	for(i = 0; backlist[i]; i ++) {
		printf("%s\n", backlist[i]);
	}
	return 0;
}



