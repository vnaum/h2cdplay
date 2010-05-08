OUTDIR = .

# mingw build (win32)
BASSPATH = C:\bass24\c
BASSLIB = $(BASSPATH)/bass.lib
FLAGS = -Wall -s -Os -I$(BASSPATH)
CC = gcc
RM = rm

# winelib build (linux)
# BASSPATH = $(HOME)/build/bass
# BASSLIB = -L$(BASSPATH) -lbass -Wl,-rpath,$(BASSPATH)
# FLAGS = -Wall -s -Os -I$(BASSPATH) -mconsole -mno-cygwin
# CC = winegcc
# RM = rm

%.exe: %.c
	$(CC) $(FLAGS) $*.c $(BASSLIB) $(LIBS) -o $(OUTDIR)/$@

.PHONY: all clean

TARGET = h2cdplay.exe h2cdplay_cli.exe

all: $(TARGET)

clean:
	$(RM) $(OUTDIR)/$(TARGET)
