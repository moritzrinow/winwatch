# winwatch
Single header, high precision stopwatch for Windows written in C

```C
WinWatch w = StartWinWatch();

for (int i = 0; i < 1000; i++) {
  void *mem = malloc(1000000);
  free(mem);
}

StopWinWatch(w);

double elapsedMs = WinWatchElapsedMilli(w);
printf("ElapsedMs: %f\n", elapsedMs);
```
