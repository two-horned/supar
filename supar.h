#include <stdio.h>

int snprintf_date(char *buffer, size_t max);
int snprintf_time(char *buffer, size_t max);
int snprintf_ram(char *buffer, size_t max);
int snprintf_updates(char *buffer, size_t max);
int snprintf_volume(char *buffer, size_t max);

typedef int (*snprintf_comp_t)(char *, size_t);
typedef struct component Component;
typedef unsigned int milli_t;
