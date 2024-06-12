AUTOMAKE_OPTIONS = foreign -Wall -Werror subdir-objects

bin_PROGRAMS = qix_line
qix_line_SOURCES = src/main.c src/globals.c src/printer.c src/bitmap.c src/drawing.c src/player.c src/qix_monster.c src/events.c src/lines.c 

# Add optimization flags (change -O2 to -O3 if needed)
AM_CFLAGS = -O2 $(GTK_CFLAGS) -Iincludes
qix_line_LDADD = $(GTK_LIBS) -lm

# Ensure the obj directory exists
all-local: $(bin_PROGRAMS)
	$(MAKE) -f Makefile.obj move-objects

# Define the clean target
clean-local:
	rm -rf obj qix_line

distclean-local: clean-local
	rm -f Makefile config.status config.log configure configure~ config.h.in aclocal.m4 compile depcomp install-sh Makefile.in missing
	rm -rf autom4te.cache src/.deps

# Add the custom clean and distclean targets to the standard ones
CLEANFILES = qix_line
DISTCLEANFILES = Makefile config.status config.log configure aclocal.m4 compile depcomp install-sh Makefile.in missing
