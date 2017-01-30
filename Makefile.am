bin_PROGRAMS = bin/gtk-tac-toe

bin_gtk_tac_toe_SOURCES = \
	src/main.c \
	src/application.c \
	src/game.c \
	src/ioext.c \
	src/resources.c \
	src/window.c \
	include/application.h \
	include/game.h \
	include/ioext.h \
	include/window.h

bin_gtk_tac_toe_LDADD = \
	$(GTK_LIBS)

bin_gtk_tac_toe_CFLAGS = \
	$(GTK_CFLAGS) \
	$(AM_CFLAGS) \
	-I$(top_srcdir)/include

EXTRA_DIST = \
	builder
dist-hook:
	rm -f `find $(top_distdir) -name '*~'`

src_resources = $(shell glib-compile-resources --sourcedir=$(top_srcdir)/builder --generate-dependencies $(top_srcdir)/builder/sct.gresource.xml)

BUILT_SOURCES = \
	src/resources.c

CLEANFILES = \
	$(BUILT_SOURCES)

src/resources.c: $(top_srcdir)/builder/sct.gresource.xml $(src_resources)
	glib-compile-resources --target=$(top_builddir)/$@ --sourcedir=$(top_srcdir)/builder --generate-source --c-name gtk_tac_resources $<
