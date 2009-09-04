dnl
dnl    configure.ac for GNU External Ballistics Library
dnl    Jonathan A. Zdziarski <jonathan@nuclearelephant.com>
dnl
dnl    autoconf (GNU Autoconf) 2.59
dnl    ltmain.sh (GNU libtool) 1.5.6
dnl    automake (GNU automake) 1.9.2
dnl
AC_PREREQ(2.59)
AC_COPYRIGHT([GPL])
AC_INIT([libballistics],[1.1.0],[jonathan@zdziarski.com])
CONFIGURE_ARGS=$@
AC_CONFIG_SRCDIR(./src)

AM_INIT_AUTOMAKE([1.7 foreign])

AC_CANONICAL_HOST

#---------------------------------------------------------
#	C language relative
AC_LANG(C)
AC_PROG_CC
#AC_PROG_CPP
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET

AC_C_INLINE

AC_DEFINE_UNQUOTED(CONFIGURE_ARGS, ["$CONFIGURE_ARGS"],
    [Configure Arguments])

# Check for various flavors of varargs macros
AC_MSG_CHECKING([for ISO C99 varargs macros in C])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
        [[
            int a(int, int, int);
#           define call_a(...) a(1,__VA_ARGS__)
        ]],
        [[ call_a(2,3); ]])
    ],  dnl AC_LANG_PROGRAM() end
    [
        # action if-found
        AC_MSG_RESULT([yes])
        AC_DEFINE([HAVE_ISO_VARARGS], 1,
                  [Define if you have ISO C99 vararg macros])
    ],
    [
        # action if-not-found
        AC_MSG_RESULT([no])
    ])

#---------------------------------------------------------
#   Libc features
#
AC_CHECK_HEADERS(string.h)
AC_CHECK_HEADERS(math.h)
AC_CHECK_HEADERS(sys/time.h)
AC_HEADER_TIME

LIBS="-lm"

#
#   Debug support
#
AC_ARG_ENABLE(debug,
    [AS_HELP_STRING(--enable-debug,
                        Enable debugging support
                        Don't enable this unless something needs testing!
                    )])
AC_MSG_CHECKING([whether to enable debug output])
case x"$enable_debug" in
    xyes)   # debug output enabled explicity
            ;;
    xno)    # debug output disabled explicity
            ;;
    x)      # debug output disabled by default
            enable_debug=no
            ;;
    *)      AC_MSG_ERROR([unexpected value $enable_debug for --{enable,disable}-debug configure option])
            ;;
esac
if test x"$enable_debug" != xyes
then
    enable_debug=no
else
    enable_debug=yes    # overkill, but convenient
    AC_DEFINE(DEBUG, 1, [Defined if debug output is enabled])
fi
AC_MSG_RESULT([$enable_debug])

#----------------------------------------------------------
# final cut
#
AC_CONFIG_HEADERS(src/auto-config.h)
AH_TOP([
#ifndef __auto_config_h
#define __auto_config_h
])
AH_BOTTOM([
#endif /* !__auto_config_h */
])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

