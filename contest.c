/*
        BASS simple console player
        Copyright (c) 1999-2008 Un4seen Developments Ltd.
*/

#include <stdlib.h>
#include <stdio.h>
#include "bass.h"

// display error messages
void
Error (const char *text)
{
  printf ("Error(%d): %s\n", BASS_ErrorGetCode (), text);
  BASS_Free ();
  exit (0);
}

int
main (int argc, char **argv)
{
  DWORD chan, act, time, level;
  QWORD pos;
  int a;

  printf ("Simple console mode BASS example : MOD/MPx/OGG/WAV player\n"
	  "---------------------------------------------------------\n");

  // check the correct BASS was loaded
  if (HIWORD (BASS_GetVersion ()) != BASSVERSION)
    {
      printf ("An incorrect version of BASS was loaded");
      return;
    }

  if (argc != 2)
    {
      printf ("\tusage: contest <file>\n");
      return;
    }

  // setup output - default device
  if (!BASS_Init (-1, 44100, 0, 0, NULL))
    Error ("Can't initialize device");

  // try streaming the file/url
  chan = BASS_StreamCreateFile (FALSE, argv[1], 0, 0, BASS_SAMPLE_LOOP);
  if (chan)
    {
      pos = BASS_ChannelGetLength (chan, BASS_POS_BYTE);
      printf ("streaming file [%I64u bytes]", pos);
    }

  // display the time length
  if (pos != -1)
    {
      time = (DWORD) BASS_ChannelBytes2Seconds (chan, pos);
      printf (" %u:%02u\n", time / 60, time % 60);
    }
  else				// no time length available
    printf ("\n");

  BASS_ChannelPlay (chan, FALSE);

  while ((act = BASS_ChannelIsActive (chan)))
    {
      // display some stuff and wait a bit
      level = BASS_ChannelGetLevel (chan);
      pos = BASS_ChannelGetPosition (chan, BASS_POS_BYTE);
      time = BASS_ChannelBytes2Seconds (chan, pos);
      printf ("pos %09I64u", pos);
      printf (" R - cpu %.2f%%  \r", BASS_GetCPU ());
      fflush (stdout);
      Sleep (50);
    }
  printf
    ("                                                                             \r");

  // wind the frequency down...
  BASS_ChannelSlideAttribute (chan, BASS_ATTRIB_FREQ, 1000, 500);
  Sleep (300);
  // ...and fade-out to avoid a "click"
  BASS_ChannelSlideAttribute (chan, BASS_ATTRIB_VOL, -1, 200);
  // wait for slide to finish
  while (BASS_ChannelIsSliding (chan, 0))
    Sleep (1);

  BASS_Free ();
  return 0;
}
