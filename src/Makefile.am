#
# Makefile.am for GNU External Ballistics Library
# Jonathan A. Zdziarski <jonathan@zdziarski.com>
#
# autoconf (GNU Autoconf) 2.59
# ltmain.sh (GNU libtool) 1.5.6
# automake (GNU automake) 1.9.2

includedir=@includedir@/libballistics

SUBDIRS = . 
DIST_SUBDIRS = . 

pkgconfigdir = $(libdir)/pkgconfig
libversion = 0:0:0

EXTRA_DIST = example.c

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 auto-config.h.in \
	config.guess config.sub configure depcomp install-sh   \
	ltmain.sh missing mkinstalldirs

DEFS = @DEFS@ 

lib_LTLIBRARIES = libballistics.la
include_HEADERS = ballistics.h

libballistics_la_SOURCES = \
	ballistics.h \
	angle.c \
	atmosphere.c \
	retardation.c \
        retrieve.c \
	solve.c \
	windage.c \
	zero.c

#   current:revision:age
libballistics_la_LDFLAGS = -rpath '$(libdir)' -version-info $(libversion)

ACLOCAL_AMFLAGS =
