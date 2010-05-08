#include <stdio.h>
#include <windows.h>

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

void
fire_event (int trknum)
{
  if (!SetEvent (evt[trknum]))
    {
      printf ("SetEvent failed (%ld)\n", (long int)GetLastError ());
      return;
    }
}

int
main (int argc, char **argv)
{
  int trknum = 0;
  printf
    ("HOMM2 CD music player\nEnter track number to play, 0 to stop playback, 1 to exit\n");

  create_events ();
  while (1)
    {
      printf ("h2play> ");
      scanf ("%d", &trknum);

      printf ("Event: %d\n", trknum);
      fire_event (trknum);

      if (trknum == 1)
        {
          printf ("Terminating\n");
          return 0;
        }
    }
}
