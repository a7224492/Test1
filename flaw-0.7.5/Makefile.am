## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src \
	data

flawdocdir = ${prefix}/doc/flaw
flawdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(flawdoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(flawdocdir)
