#include <SDL2/SDL.h>
#include <iostream>
#include "MicroModUtils.hpp"

#define __SAMPLING_FREQ__ 48000  /* 48khz. */
#define __REVERB_BUF_LEN__ 4800  /* 50ms. */
#define __OVERSAMPLE__ 2         /* 2x oversampling. */
#define __NUM_CHANNELS__ 2       /* Stereo. */
#define __BUFFER_SAMPLES__ 16384 /* 64k buffer. */

class MicroModSDLPlayer
{
public:
    int OVERSAMPLE;
    int NUM_CHANNELS;
    int SAMPLING_FREQ;
    int BUFFER_SAMPLES;
    long samplesRemaining;
    short reverbBuffer[__REVERB_BUF_LEN__];
    //short mixBuffer[__BUFFER_SAMPLES__ * __NUM_CHANNELS__ * __OVERSAMPLE__];
    short* mixBuffer;
    long reverbLen, reverbIdx, filtL, filtR;

    void downSample(short *input, short *output, long count);
    void reverb(short *buffer, long count);
    void crossfeed(short *audio, int count);
    void callback(Uint8 *stream, int len);
    void terminationHandler(int signum);
    void printModuleInfo();
    long getSamplesRemaining();
    SDL_AudioDeviceID initialise(unsigned char module[]);
    long playModule(unsigned char module[]);
    Uint8* buildStream(int len);
    MicroModSDLPlayer();
    ~MicroModSDLPlayer();
private:
    MicroModUtils* microModUtils;

};


