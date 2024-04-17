#include <time.h>
#include "../supar.h"

int
snprintf_time(char *buffer, size_t max)
{
  char icon[5] = "🕐";
  time_t rawtime;
  struct tm *ts;

  time(&rawtime);
  ts = localtime(&rawtime);

  icon[3] = 0x90 + ((ts->tm_hour + 11) % 12) + 12 * ((ts->tm_min + 15) / 30 % 2);
  return snprintf(
      buffer,
      max,
      "%s %02d:%02d:%02d",
      icon,
      ts->tm_hour,
      ts->tm_min,
      ts->tm_sec
      );
}
