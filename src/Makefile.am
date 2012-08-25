bin_PROGRAMS=tools-daemon
tools_daemon_SOURCES=tools.c\
	logger.c\
	lt_event.c\
	lt_monitor.c\
	lt_settings.c


tools_daemon_LDFLAGS=-ludev
tools_daemon_CFLAGS=`pkg-config --libs --cflags glib-2.0`