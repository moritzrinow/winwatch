#ifndef _WINWATCH_H
#define _WINWATCH_H

#include <Windows.h>

#define WINWATCH_STATE_RUNNING 1
#define WINWATCH_STATE_PAUSED 2
#define WINWATCH_STATE_STOPPED 3

typedef struct WinWatch {
  LARGE_INTEGER Frequency;
  LARGE_INTEGER StartTime;
  LARGE_INTEGER TimePaused;
  LARGE_INTEGER PauseTime;
  LARGE_INTEGER StopTime;
  int           State;
} WinWatch;

#define StartWinWatch() StartWinWatchA() // Creates and starts a new winwatch
#define StopWinWatch(w) StopWinWatchA(&w) // Stops a winwatch, which disables pausing or resuming the watch instance in the future
#define PauseWinWatch(w) PauseWinWatchA(&w) // Pauses a winwatch. The time between now and the next resume will be measured and subtracted from the final measurement
#define ResumeWinWatch(w) ResumeWinWatchA(&w) // Resumes a winwatch

#define WinWatchElapsedTicksL(w) (w.StopTime.QuadPart - w.StartTime.QuadPart - w.TimePaused.QuadPart) // Get the elapsed ticks as 64bit signed integer
#define WinWatchElapsedNanoL(w) (WinWatchElapsedTicksL(w) * 1000000000 / w.Frequency.QuadPart) // Get the elapsed nanoseconds as 64bit signed integer
#define WinWatchElapsedMicroL(w) (WinWatchElapsedNanoL(w) / 1000) // Get the elapsed microseconds as 64 signed integer
#define WinWatchElapsedMilliL(w) (WinWatchElapsedMicroL(w) / 1000) // Get the elapsed milliseconds as 64bit signed integer
#define WinWatchElapsedSecondsL(w) (WinWatchElapsedMilliL(w) / 1000) // Get the elapsed seconds as 64bit signed integer

#define WinWatchElapsedNanoD(w) ((double)WinWatchElapsedTicksL(w) * 1000000000.0 / (double)w.Frequency.QuadPart) // Get the elapsed microseconds as 64bit float
#define WinWatchElapsedMicroD(w) ((double)WinWatchElapsedNanoD(w) / 1000.0) // Get the elapsed microseconds as 64bit float
#define WinWatchElapsedMilliD(w) ((double)WinWatchElapsedMicroD(w) / 1000.0) // Get the elapsed milliseconds as 64bit float
#define WinWatchElapsedSecondsD(w) ((double)WinWatchElapsedMilliD(w) / 1000.0) // Get the elapsed seconds as 64bit float

WinWatch StartWinWatchA(
  void
);

void StopWinWatchA(
  WinWatch *w
);

void PauseWinWatchA(
  WinWatch *w
);

void ResumeWinWatchA(
  WinWatch *w
);

#endif //_WINWATCH_H