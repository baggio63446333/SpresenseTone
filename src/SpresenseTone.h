#ifndef __SPRESENSE_TONE_H__
#define __SPRESENSE_TONE_H__

#include <Audio.h>

#ifndef TONE_VOLUME
#define TONE_VOLUME (-45) // -90 ~ 0
#endif

AudioClass *toneAudio = NULL;

void tone(uint8_t pin, unsigned int frequency, unsigned long duration)
{
  (void)pin;

  if (!toneAudio) {
    toneAudio = AudioClass::getInstance();
    toneAudio->begin();
    toneAudio->setPlayerMode(AS_SETPLAYER_OUTPUTDEVICE_SPHP, 0, 0);
  }
  toneAudio->setBeep(1, TONE_VOLUME, frequency);
  if (duration > 0) {
    delay(duration);
    toneAudio->setBeep(0, TONE_VOLUME, 0);
  }
}

void noTone(uint8_t pin)
{
  (void)pin;

  if (!toneAudio) {
    toneAudio = AudioClass::getInstance();
    toneAudio->begin();
    toneAudio->setPlayerMode(AS_SETPLAYER_OUTPUTDEVICE_SPHP, 0, 0);
  }
  toneAudio->setBeep(0, TONE_VOLUME, 0);
}

void setAudio(AudioClass *theAudio)
{
  toneAudio = theAudio;
}

#endif // __SPRESENSE_TONE_H__
