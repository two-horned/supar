#include <time.h>
#include "../supar.h"

static const char *weekdays[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

int
snprintf_date(char *buffer, size_t max)
{
  time_t rawtime;
  struct tm *ts;

  time(&rawtime);
  ts = localtime(&rawtime);

  return snprintf(
      buffer,
      max,
      "%s %02d.%02d",
      weekdays[ts->tm_wday],
      ts->tm_mday,
      ts->tm_mon + 1
      );
}
