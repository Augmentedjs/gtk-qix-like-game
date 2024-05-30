bin_PROGRAMS = qix_line
qix_line_SOURCES = src/main.c src/drawing.c src/player.c src/line.c src/trails.c src/events.c

# Add optimization flags (change -O2 to -O3 if needed)
AM_CFLAGS = -O2 $(GTK_CFLAGS) -Iincludes
qix_line_LDADD = $(GTK_LIBS) -lm

# Define the clean target
clean-local:
	rm -f qix_line.o src/*.o includes/*.o qix_line

# Define the distclean target
distclean-local: clean-local
	rm -f Makefile config.status config.log configure aclocal.m4 compile depcomp install-sh Makefile.in missing
	rm -rf autom4te.cache .deps

# Add the custom clean and distclean targets to the standard ones
CLEANFILES = qix_line
DISTCLEANFILES = Makefile config.status config.log configure aclocal.m4 compile depcomp install-sh Makefile.in missing
