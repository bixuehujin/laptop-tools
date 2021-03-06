/*
 * lt_setter.h
 *
 *  Created on: 2012-8-26
 *      Author: hujin
 */

#ifndef LT_SETTER_H_
#define LT_SETTER_H_

#include <libudev.h>

int lt_device_is_mouse(struct udev_device * device);

void lt_setter_set_backlight(int backlight);

int lt_device_is_power_online();

int lt_device_control_touchpad(int ctl);

int lt_device_is_mouse_plugged();


#endif /* LT_SETTER_H_ */
