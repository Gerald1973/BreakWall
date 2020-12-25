#include "../include/MicroModSDLPlayer.hpp"
#include "../include/MicroModUtils.hpp"
#include <string>
#include <iostream>

using namespace std;

/*
	2:1 downsampling with simple but effective anti-aliasing.
	Count is the number of stereo samples to process, and must be even.
	input may point to the same buffer as output.
*/
void MicroModSDLPlayer::downSample(short *input, short *output, long count)
{
    long in_idx, out_idx, out_l, out_r;
    in_idx = out_idx = 0;
    while (out_idx < count)
    {
        out_l = filtL + (input[in_idx++] >> 1);
        out_r = filtR + (input[in_idx++] >> 1);
        filtL = input[in_idx++] >> 2;
        filtR = input[in_idx++] >> 2;
        output[out_idx++] = out_l + filtL;
        output[out_idx++] = out_r + filtR;
    }
}

/* Simple stereo cross delay with feedback. */
void MicroModSDLPlayer::reverb(short *buffer, long count)
{
    long buffer_idx, buffer_end;
    if (reverbLen > 2)
    {
        buffer_idx = 0;
        buffer_end = buffer_idx + (count << 1);
        while (buffer_idx < buffer_end)
        {
            buffer[buffer_idx] = (buffer[buffer_idx] * 3 + reverbBuffer[reverbIdx + 1]) >> 2;
            buffer[buffer_idx + 1] = (buffer[buffer_idx + 1] * 3 + reverbBuffer[reverbIdx]) >> 2;
            reverbBuffer[reverbIdx] = buffer[reverbIdx];
            reverbBuffer[reverbIdx + 1] = buffer[reverbIdx + 1];
            reverbIdx += 2;
            if (reverbIdx >= reverbLen)
            {
                reverbIdx = 0;
            }
            reverbIdx += 2;
        }
    }
}

/* Reduce stereo-separation of count samples. */
void MicroModSDLPlayer::crossfeed(short *audio, int count)
{
    int l, r, offset = 0, end = count << 1;
    while (offset < end)
    {
        l = audio[offset];
        r = audio[offset + 1];
        audio[offset++] = (l + l + l + r) >> 2;
        audio[offset++] = (r + r + r + l) >> 2;
    }
}

void MicroModSDLPlayer::callback(Uint8 *stream, int len)
{
    long count;
    count = len * OVERSAMPLE / 4;
    if (samplesRemaining < count)
    {
        /* Clear output.*/
        memset(stream, 0, len);
        count = samplesRemaining;
    }
    if (count > 0)
    {
        /* Get audio from replay.*/
        memset(mixBuffer, 0, count * NUM_CHANNELS * sizeof(short));
        MicroModUtils::getInstance()->getAudio(mixBuffer, count);
        downSample(mixBuffer, (short *)stream, count);
        crossfeed((short *)stream, count / OVERSAMPLE);
        reverb((short *)stream, count / OVERSAMPLE);
        samplesRemaining -= count;
    }
    else
    {
        Uint32 myEventType = SDL_RegisterEvents(1);
        if (myEventType != ((Uint32)-1))
        {
            SDL_Event event;
            event.type = SDL_USEREVENT;
            event.user.code = 2;
            event.user.data1 = (void *)&"Sound finish."[0];
            event.user.data2 = NULL;
            event.user.data2 = 0;
            SDL_PushEvent(&event);
        }
        /* Notify the main thread to stop playback.*/
        SDL_SemPost(semaphore);
    }
}

/* Notify the main thread to stop playback. */
void MicroModSDLPlayer::terminationHandler(int signum)
{
    SDL_SemPost(semaphore);
    fprintf(stderr, "\nTerminated!\n");
}

void MicroModSDLPlayer::printModuleInfo()
{
    int inst;
    string tmp = "";
    for (inst = 0; inst < 16; inst++)
    {
        tmp = MicroModUtils::getInstance()->getInstrumentName(inst);
        cout << inst << ":" << tmp << " ";
        tmp = MicroModUtils::getInstance()->getInstrumentName(inst + 16);
        cout << inst + 16 << ":" << tmp << endl;
    }
}

long MicroModSDLPlayer::initialise(unsigned char module[])
{
    SDL_AudioSpec audiospec;
    /* Initialise replay.*/
    cout << "DEBUG : " << SAMPLING_FREQ << endl;
    long result = MicroModUtils::getInstance()->initialise(module, SAMPLING_FREQ * OVERSAMPLE);
    if (result == 0)
    {
        //printModuleInfo();
        /* Calculate song length. */
        samplesRemaining = MicroModUtils::getInstance()->calculateSongDuration();
        printf("Song Duration: %li seconds.\n", samplesRemaining / (SAMPLING_FREQ * OVERSAMPLE));
        fflush(NULL);
        /* Initialise SDL_AudioSpec Structure. */
        memset(&audiospec, 0, sizeof(SDL_AudioSpec));
        audiospec.freq = SAMPLING_FREQ;
        audiospec.format = AUDIO_S16SYS;
        audiospec.channels = NUM_CHANNELS;
        audiospec.samples = BUFFER_SAMPLES;
        audiospec.callback = audioCallback;
        audiospec.userdata = NULL;
        /* Initialise audio subsystem. */
        result = SDL_Init(SDL_INIT_AUDIO);
        if (result == 0)
        {
            result = SDL_OpenAudio(&audiospec, NULL);
        }
        else
        {
            fprintf(stderr, "Unable to open audio device: %s\n", SDL_GetError());
        }
    }
    return result;
}

long MicroModSDLPlayer::playModule(unsigned char module[])
{
    long result = this->initialise(module);
    if (result == 0)
    {
        /* Begin playback. */
        SDL_PauseAudio(0);
        /* Wait for playback to finish. */
        semaphore = SDL_CreateSemaphore(0);
        result = SDL_SemWait(semaphore);
        if (result != 0)
        {
            fprintf(stderr, "SDL_SemWait() failed.\n");
        }
        /* Close audio device and shut down SDL. */
        SDL_CloseAudio();
    }
    return result;
}

MicroModSDLPlayer::MicroModSDLPlayer()
{
    OVERSAMPLE = __OVERSAMPLE__;
    NUM_CHANNELS = __NUM_CHANNELS__;
    SAMPLING_FREQ = __SAMPLING_FREQ__;
    BUFFER_SAMPLES = __BUFFER_SAMPLES__;
}

MicroModSDLPlayer::~MicroModSDLPlayer() {}