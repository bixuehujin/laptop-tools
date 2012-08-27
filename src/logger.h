/*
 * logger.h
 *
 *  Created on: 2012-8-21
 *      Author: hujin
 */

#include <syslog.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifndef LOGGER_H_
#define LOGGER_H_


#define	LOGGER_EMERG 	LOG_EMERG		/* system is unusable */
#define	LOGGER_ALERT 	LOG_ALERT		/* action must be taken immediately */
#define	LOGGER_CRIT  	LOG_CRIT		/* critical conditions */
#define	LOGGER_ERR   	LOG_ERR			/* error conditions */
#define	LOGGER_WARNING 	LOG_WARNING		/* warning conditions */
#define	LOGGER_NOTICE 	LOG_NOTICE		/* normal but significant condition */
#define	LOGGER_INFO 	LOG_INFO		/* informational */
#define	LOGGER_DEBUG 	LOG_DEBUG		/* debug-level messages */

static char * logger_names[] = {
		"emerg",
		"alert",
		"crit",
		"error",
		"warn",
		"notice",
		"info",
		"debug",
		NULL
};

#define logger_debug(message , ...) {									\
		char * file = __FILE__; int line = __LINE__; 						\
		char tmpl[208] = {0};												\
		snprintf(tmpl, sizeof(tmpl), "%s:%d %s", file, line, message);		\
		logger_log(LOGGER_DEBUG, tmpl, ##__VA_ARGS__);\
}



void _logger_debug(const char * msg, const char * filename, int line, ...);
void logger_init(const char * ident, int daemon);

void logger_log(int priority, const char * msg, ...);

void logger_close();

#endif /* LOGGER_H_ */
