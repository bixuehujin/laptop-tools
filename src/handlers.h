/*
 * handlers.h
 *
 *  Created on: 2012-8-26
 *      Author: hujin
 */

#ifndef HANDLERS_H_
#define HANDLERS_H_


void on_power_state_changed(int online);

void on_backlight_changed(int level, int max);

void on_signal_term();

void on_signal_tstp();

#endif /* HANDLERS_H_ */
