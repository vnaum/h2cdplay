BASSPATH = $(HOME)/build/bass
OUTDIR = .
FLAGS = -Wall -s -Os -I$(BASSPATH) -L$(BASSPATH) -lbass -Wl,-rpath,$(BASSPATH)

CC = gcc
RM = rm

%.exe: %.c
	$(CC) $(FLAGS) $*.c -o $(OUTDIR)/$@

.PHONY: all clean

TARGET = h2cdplay.exe h2cdplay_cli.exe

all: $(TARGET)

clean:
	$(RM) $(OUTDIR)/$(TARGET)
