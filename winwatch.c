#include "winwatch.h"

WinWatch StartWinWatchA(
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

void StopWinWatchA(
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

void PauseWinWatchA(
  WinWatch *w
)
{
  BOOL res;

  if (w->State == WINWATCH_STATE_RUNNING) {
    res = QueryPerformanceCounter(&w->PauseTime);
    w->State = WINWATCH_STATE_PAUSED;
  }
}

void ResumeWinWatchA(
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