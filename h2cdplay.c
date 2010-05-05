/*
 * Basic HOMM2 player
 */

#include <stdio.h>
#include <unistd.h>
#include "bass.h"

#define MAXTRACK 64
HANDLE evt[MAXTRACK];

void
create_events ()
{
  char evtnamebuf[1024];
  int i;
  for (i = 0; i < MAXTRACK; i++)
    {
      sprintf (evtnamebuf, "h2cd_play_track#%02d", i);
      evt[i] = CreateEvent (NULL, FALSE, FALSE, evtnamebuf);
    }
}

// display error messages
void
Error (const char *text)
{
  printf ("Error(%d): %s\n", BASS_ErrorGetCode (), text);
  BASS_Free ();
  exit (0);
}

// display error messages
void
StopChan (DWORD chan)
{
  // stop the track that is been playing
  if (BASS_ChannelIsActive (chan))
    {
      // fadeout and stop required
      printf ("Stopping playback...\n");
      BASS_ChannelSlideAttribute (chan, BASS_ATTRIB_VOL, -1, 200);
      while (BASS_ChannelIsSliding (chan, 0))
        usleep (100);
      BASS_ChannelStop (chan);
    }
}

int
main (int argc, char **argv)
{
  DWORD chan = 0;
  int trknum = 0;
  int i;
  char *trkpath = "./tracks";
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
  create_events();
  
  // Reset all pending events:
  for (i = 0; i < MAXTRACK; i++)
    ResetEvent(evt[i]);

  printf ("Will play tracks from '%s' on events\n", trkpath);
  printf ("Waiting for events...\n");
  while (1)
    {
      DWORD dwWaitResult = WaitForMultipleObjects(
        MAXTRACK,      // number of handles in array
        evt,           // array of thread handles
        FALSE,         // wait until all are signaled
        INFINITE);

      printf ("Event fired: %ld\n", dwWaitResult);
      trknum = dwWaitResult;
      
      if (trknum == 0)
	{
          StopChan (chan);
          continue;
	}

      if (trknum == 1)
	{
	  printf ("Terminating\n");
	  BASS_Free ();
	  return 0;
	}
      
      // start playback if valid trk num is given
      if (trknum >=2 && trknum <= 49)
      {
        StopChan (chan);
        sprintf (trkbuf, "%s/track%02d.mp3", trkpath, trknum);
        printf ("Playing track %s\n", trkbuf);

        chan = BASS_StreamCreateFile (FALSE, trkbuf, 0, 0, BASS_SAMPLE_LOOP);
        if (!chan)
        {
          printf ("Error while opening\n");
          continue;
        }

        // set some flags:
        // free stream once playback ends
        BASS_ChannelFlags(chan, BASS_STREAM_AUTOFREE,
            BASS_STREAM_AUTOFREE|BASS_SAMPLE_LOOP);
        
        BASS_ChannelPlay (chan, FALSE);
      }
      
      // process volume events
      if (trknum >=50 && trknum <= 60)
      {
        int vol_req = trknum - 50;
        float vol_fin = 0.0;
        int tvar = 11 - vol_req;
        /*
         * on -- 1
         * 9 -- 2
         * 8 -- 3
         * 3 -- 8
         * 2 -- 9
         * 1 -- 10
         * off - 0
         * */

        if (tvar > 10)
          tvar = 0;
        vol_fin = 0.1 * tvar;
        printf ("Volume requested: %d, final: %.2f\n", vol_req, vol_fin);
        
        BASS_ChannelSetAttribute(
            chan,
            BASS_ATTRIB_VOL,
            vol_fin
            );
        continue;
      }
    }
}
