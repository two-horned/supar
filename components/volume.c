#include <alsa/asoundlib.h>
#include "../supar.h"

static int handle_error(snd_mixer_t *handlem, char *buffer, size_t max);

static const char *card = "pulse";
static const char *selem_name = "Master";
static const snd_mixer_selem_channel_id_t CHANNEL = SND_MIXER_SCHN_MONO;

int
handle_error(snd_mixer_t *handle, char *buffer, size_t max)
{
  if (handle)
    snd_mixer_close(handle);
  return snprintf(buffer, max, "n/a");
}

int
snprintf_volume(char *buffer, size_t max)
{
  char icon[5] = "🔈";
  long volume, max_volume;
  snd_mixer_selem_id_t *sid;
  snd_mixer_t *handle;
  snd_mixer_elem_t* elem;

  if(snd_mixer_open(&handle, 0) < 0)
    return handle_error(handle, buffer, max);
  if(snd_mixer_attach(handle, card) < 0)
    return handle_error(handle, buffer, max);
  if(snd_mixer_selem_register(handle, NULL, NULL) < 0)
    return handle_error(handle, buffer, max);
  if(snd_mixer_load(handle) < 0)
    return handle_error(handle, buffer, max);

  snd_mixer_selem_id_alloca(&sid);
  snd_mixer_selem_id_set_index(sid, 0);
  snd_mixer_selem_id_set_name(sid, selem_name);

  if(!(elem = snd_mixer_find_selem(handle, sid)))
    return handle_error(handle, buffer, max);
  if(snd_mixer_selem_get_playback_volume_range(elem, &volume, &max_volume) < 0)
    return handle_error(handle, buffer, max);
  if(snd_mixer_selem_get_playback_volume(elem, CHANNEL, &volume) < 0)
    return handle_error(handle, buffer, max);
  snd_mixer_close(handle);

  volume = volume * 100 / max_volume % 101;
  icon[3] = 0x88 + volume / 34;
  return snprintf(
      buffer,
      max,
      "%s %3lu%%",
      icon,
      volume
      );
}
