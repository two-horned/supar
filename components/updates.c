#include <stdio.h>
#include "../supar.h"

#define BUF_SIZE 65536

int
snprintf_updates(char *buffer, size_t max)
{
  FILE *fp;
  char buf[BUF_SIZE];
  int pending_updates;

  fp = popen("checkupdates", "r");
  pending_updates = 0;

  while(!feof(fp)) {
        size_t res = fread(buf, 1, BUF_SIZE, fp);

        for (size_t i = 0; i < res; i++)
            if (buf[i] == '\n')
                pending_updates++;
  }

  pclose(fp);

  return snprintf(
      buffer,
      max,
      "%2d",
      pending_updates % 100
      );
}
