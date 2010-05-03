BASSPATH = C:\bass24\c
BASSLIB = $(BASSPATH)/bass.lib
OUTDIR = .
FLAGS = -Wall -s -Os -I$(BASSPATH)

CC = gcc
RM = rm

%.exe: %.c
	$(CC) $(FLAGS) $*.c $(BASSLIB) $(LIBS) -o $(OUTDIR)/$@

.PHONY: all clean

TARGET = h2cdplay.exe h2cdplay_cli.exe

all: $(TARGET)

clean:
	$(RM) $(OUTDIR)/$(TARGET)
