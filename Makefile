BASSPATH = C:\20100502\bass24\c
BASSLIB = $(BASSPATH)/bass.lib
OUTDIR = .
FLAGS = -Wall -s -Os -I$(BASSPATH)

CC = gcc
RM = rm
RES = windres

%.exe: %.c
	$(CC) $(FLAGS) $*.c $(BASSLIB) $(LIBS) -o $(OUTDIR)/$@

.PHONY: all clean

TARGET = contest.exe

all: $(TARGET)

clean:
	$(RM) $(OUTDIR)/$(TARGET)
