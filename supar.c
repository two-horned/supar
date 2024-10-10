#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include "supar.h"

struct component {
  const snprintf_comp_t snprintf_comp;
  const milli_t pollrate;
  const char *icon;
  const char *background;
};

#include "config.h"

#define MAX 256
#define LIMIT(a) a < MAX ? MAX - a : 0
#define ELEMENTS sizeof(status) / sizeof(Component)

static unsigned int tiny_hash(void *userdata, size_t size);
static int prepare_buffer(char *buf, size_t *os, size_t *ss);

unsigned int
tiny_hash(void *userdata, size_t size)
{
  unsigned int hash = 0;
  unsigned char *blocks = userdata;

  for (size_t i = 0; i < size; ++i)
    hash ^= (hash - blocks[i]) << ((hash + blocks[i]) & 31);

  return hash;
}

int
prepare_buffer(char *buffer, size_t *offsets, size_t *sizes)
{
  size_t offset = snprintf(buffer, MAX, "^c%s^", text);
  for (size_t i = 0; i < ELEMENTS; i++) {
    offset += snprintf(
        buffer + offset,
        LIMIT(offset),
        "^b%s^ ^b%s^ %s",
        background,
        status[i].background,
        status[i].icon
        );
    
    offsets[i] = offset;
    sizes[i] = status[i].snprintf_comp(buffer + offset, LIMIT(offset));

    offset += sizes[i];
    offset += snprintf(buffer + offset, LIMIT(offset), " ");
  }
  return 2 < snprintf(buffer + offset, LIMIT(offset), "^d^");
}


int
main()
{
  Display *display;
  Window window;

  if (!(display = XOpenDisplay(NULL))) {
    fprintf(stderr, "Failed to open display");
    return EXIT_FAILURE;
  }
  window = XDefaultRootWindow(display);

  char buffer[MAX];
  size_t offsets[ELEMENTS];
  size_t sizes[ELEMENTS];
  if (!prepare_buffer(buffer, offsets, sizes)) {
    fprintf(stderr, "Statusbar size too limited");
    return EXIT_FAILURE;
  }
  
  size_t offset;
  size_t size;
  unsigned int hash;
  unsigned int hashes[ELEMENTS];
  milli_t counter[ELEMENTS];
  for (;;) {
    for (size_t i = 0; i < ELEMENTS; i++) {
      if (counter[i]++ < status[i].pollrate)
        continue;

      counter[i] = 0;
      offset = offsets[i];
      size = status[i].snprintf_comp(buffer + offset, LIMIT(offset));
      if (size != sizes[i]) {
        fprintf(stderr, "Statusbar size changed");

        if (prepare_buffer(buffer, offsets, sizes))
          continue;

        fprintf(stderr, "Statusbar size too limited");
        return EXIT_FAILURE;
      }
      buffer[offset + size] = ' ';
      hash = tiny_hash(buffer + offset, size);
      if (hash != hashes[i]) {
        if (XStoreName(display, window, buffer) < 0) {
          fprintf(stderr, "Failed to store statusbar");
          return EXIT_FAILURE;
        }
        XFlush(display);
        hashes[i] = hash;
      }
    }
    usleep(250000);
  }
}
