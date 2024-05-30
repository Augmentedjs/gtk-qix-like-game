bin_PROGRAMS = qix_line
qix_line_SOURCES = src/main.c src/drawing.c src/player.c src/line.c src/trails.c src/events.c

# Add optimization flags
AM_CFLAGS = -O2 $(GTK_CFLAGS) -Iincludes
qix_line_LDADD = $(GTK_LIBS) -lm
