dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(flaw, 0.7.5)

prefix=/usr/share
AC_SUBST(prefix)

datadir=/usr/share/games
AC_SUBST(datadir)

bindir=/usr/games
AC_SUBST(bindir)

SDL_REQUIRED=1.2.0

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_PATH_XTRA


AC_PROG_CPP
AC_PROG_CXX




AM_PROG_LIBTOOL

dnl ***************************************************************************
dnl Check for SDL
dnl ***************************************************************************
AM_PATH_SDL($SDL_REQUIRED,
            :,
	    AC_MSG_ERROR([SDL version $SDL_REQUIRED not found]))
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
CFLAGS="$CFLAGS $SDL_CFLAGS"
CPPFLAGS="$CPPFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"


dnl ***************************************************************************
dnl Check for SDL_image
dnl ***************************************************************************
AC_CHECK_LIB(SDL_image, IMG_Load,
    [ LIBS="$LIBS -lSDL_image" ],
    AC_MSG_ERROR([SDL_image not found]))





dnl ***************************************************************************
dnl Check for SDL_ttf
dnl ***************************************************************************
AC_CHECK_LIB(SDL_ttf, TTF_OpenFont,
    [ LIBS="$LIBS -lSDL_ttf" ],
    AC_MSG_ERROR([SDL_ttf not found]))



dnl ***************************************************************************
dnl Check for SDL_mixer
dnl ***************************************************************************
AC_CHECK_LIB(SDL_mixer, Mix_OpenAudio,
    [ LIBS="$LIBS -lSDL_mixer" ],
    AC_MSG_ERROR([SDL_mixer not found]))




AC_OUTPUT([
Makefile
data/16/Makefile
data/8/Makefile
data/Makefile
src/Makefile

])
