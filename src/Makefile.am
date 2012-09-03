bin_PROGRAMS=laptop-tools-daemon
laptop_tools_daemon_SOURCES=\
	tools.c\
	logger.c\
	lt_event.c\
	lt_monitor.c\
	lt_settings.c\
	handlers.c\
	lt_instance.c\
	lt_device.c


laptop_tools_daemon_LDFLAGS=-ludev
laptop_tools_daemon_CFLAGS=`pkg-config --libs --cflags glib-2.0`