/*
 * logger.c
 *
 *  Created on: 2012-8-21
 *      Author: hujin
 */

#include "logger.h"
char logger_ident[50] = {0};

void logger_printf(const char * message, int priority) {
	time_t t = time(NULL);
	char * str = ctime(&t);
	str[strlen(str) - 1] = '\0';
	printf("%s %s: <%s> %s\n", str, logger_ident, logger_names[priority],message);
}

void logger_init(const char * ident) {
#ifdef LOGGER_DAEMON
	openlog(ident, LOG_CONS, LOG_USER);
#else
	strcpy(logger_ident, ident);
#endif

}


void logger_log(int priority, const char * msg, ...) {
	va_list va;
	va_start(va, msg);
	char message[208] = {0};
	vsnprintf(message, sizeof(message), msg, va);
	va_end(va);

#ifdef LOGGER_DAEMON
	syslog(priority, "<%s> %s", logger_names[priority], message);
#else
	logger_printf(message, priority);
#endif
}

void logger_close() {
#ifdef LOGGER_DAEMON
	closelog();
#endif
}


