#include <sys/sysinfo.h>
#include "../supar.h"

#define MEGA 1000000.0f
#define GIGA 1000000000.0f

int
snprintf_ram(char *buffer, size_t max)
{
  struct sysinfo si;
  float used;
  int size;

  sysinfo(&si);
  used = si.totalram - si.freeram;
  size = GIGA < used;
  return snprintf(
      buffer,
      max,
      "% 3.1f%c",
      used / (size ? GIGA : MEGA),
      size ? 'G' : 'M'
      );
}
