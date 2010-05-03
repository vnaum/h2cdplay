/*
        BASS simple console player
        Copyright (c) 1999-2008 Un4seen Developments Ltd.
*/

#include <stdio.h>
#include <unistd.h>
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
  DWORD chan = 0;
  int trknum = 0;
  char *trkpath = "W:/h2gold_mp3";
  char trkbuf[1024];

  printf ("HOMM2 CD music player\n");

  // check the correct BASS was loaded
  if (HIWORD (BASS_GetVersion ()) != BASSVERSION)
    {
      printf ("An incorrect version of BASS was loaded");
      return 1;
    }

  // setup output - default device
  if (!BASS_Init (-1, 44100, 0, 0, NULL))
    Error ("Can't initialize device");

  while (1)
    {
      printf ("h2play> ");
      scanf ("%d", &trknum);
      if (trknum == 0)
	{
	  printf ("Terminating\n");
	  BASS_Free ();
	  return 0;
	}
      
      // stop the track that is been playing
      if (BASS_ChannelIsActive (chan))
	{
	  // fadeout and stop required
	  printf ("Fading out, stopping...\n");
	  BASS_ChannelSlideAttribute (chan, BASS_ATTRIB_VOL, -1, 200);
	  while (BASS_ChannelIsSliding (chan, 0))
	    usleep (100);
	  BASS_ChannelStop (chan);
	}
      
      // start playback if valid trk num is given (-1 will just fade out current track)
      if (trknum > 0)
      {
        sprintf (trkbuf, "%s/track%02d.mp3", trkpath, trknum);
        printf ("Playing track %s\n", trkbuf);

        chan = BASS_StreamCreateFile (FALSE, trkbuf, 0, 0, BASS_SAMPLE_LOOP);
        BASS_ChannelPlay (chan, FALSE);
      }
    }
}
