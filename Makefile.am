ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src/lib src/dic plugin tool test

test:
	cd tool && make
	cd test && make

test_run:
	cd tool && make
	cd test && make && ./cadencii_sinsy_plugin_test
