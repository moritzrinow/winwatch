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
#define WinWatchElapsedMicroL(w) (WinWatchElapsedTicks(w) * 1000000 / w.Frequency.QuadPart) // Get the elapsed microseconds as 64 signed integer
#define WinWatchElapsedMilliL(w) (WinWatchElapsedMicroL(w) / 1000) // Get the elapsed milliseconds as 64bit signed integer
#define WinWatchElapsedSecondsL(w) (WinWatchElapsedMicroL(w) / 1000000) // Get the elapsed seconds as 64bit signed integer
#define WinWatchElapsedMicroD(w) ((double)WinWatchElapsedTicksL(w) * 1000000.0 / (double)w.Frequency.QuadPart) // Get the elapsed microseconds as 64bit float
#define WinWatchElapsedMilliD(w) ((double)WinWatchElapsedMicroD(w) / 1000.0) // Get the elapsed milliseconds as 64bit float
#define WinWatchElapsedSecondsD(w) ((double)WinWatchElapsedMicroD(w) / 1000000.0) // Get the elapsed seconds as 64bit float

static WinWatch StartWinWatchA(
  void
)
{
  BOOL res;
  WinWatch watch;
  watch.Frequency.QuadPart = 0L;
  watch.StartTime.QuadPart = 0L;
  watch.TimePaused.QuadPart = 0L;
  watch.PauseTime.QuadPart = 0L;
  watch.StopTime.QuadPart = 0L;
  watch.State = WINWATCH_STATE_RUNNING;

  res = QueryPerformanceFrequency(&watch.Frequency);
  res = QueryPerformanceCounter(&watch.StartTime);

  return watch;
}

static void StopWinWatchA(
  WinWatch *w
)
{
  BOOL res;

  if (w->State == WINWATCH_STATE_RUNNING) {
    res = QueryPerformanceCounter(&w->StopTime);
  }
  else if (w->State == WINWATCH_STATE_PAUSED) {
    w->StopTime = w->PauseTime;
  }

  w->State = WINWATCH_STATE_STOPPED;
}

static void PauseWinWatchA(
  WinWatch *w
)
{
  BOOL res;

  if (w->State == WINWATCH_STATE_RUNNING) {
    res = QueryPerformanceCounter(&w->PauseTime);
    w->State = WINWATCH_STATE_PAUSED;
  }
}

static void ResumeWinWatchA(
  WinWatch *w
)
{
  BOOL res;
  LARGE_INTEGER time;
  res = QueryPerformanceCounter(&time);

  if (w->State == WINWATCH_STATE_PAUSED) {
    LARGE_INTEGER timePaused;
    timePaused.QuadPart = time.QuadPart - w->PauseTime.QuadPart;
    w->TimePaused.QuadPart += timePaused.QuadPart;
    w->State = WINWATCH_STATE_RUNNING;
  }
}

#endif //_WINWATCH_H