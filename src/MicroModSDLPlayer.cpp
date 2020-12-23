#include "../include/MicroModSDLPlayer.hpp"
#include "../include/MicroModUtils.hpp"

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
        out_l = MicroModSDLPlayer::filtL + (input[in_idx++] >> 1);
        out_r = MicroModSDLPlayer::filtR + (input[in_idx++] >> 1);
        MicroModSDLPlayer::filtL = input[in_idx++] >> 2;
        MicroModSDLPlayer::filtR = input[in_idx++] >> 2;
        output[out_idx++] = out_l + MicroModSDLPlayer::filtL;
        output[out_idx++] = out_r + MicroModSDLPlayer::filtR;
    }
}

/* Simple stereo cross delay with feedback. */
void MicroModSDLPlayer::reverb(short *buffer, long count)
{
    long buffer_idx, buffer_end;
    if (MicroModSDLPlayer::reverbLen > 2)
    {
        buffer_idx = 0;
        buffer_end = buffer_idx + (count << 1);
        while (buffer_idx < buffer_end)
        {
            buffer[buffer_idx] = (buffer[buffer_idx] * 3 + MicroModSDLPlayer::reverbBuffer[MicroModSDLPlayer::reverbIdx + 1]) >> 2;
            buffer[buffer_idx + 1] = (buffer[buffer_idx + 1] * 3 + MicroModSDLPlayer::reverbBuffer[MicroModSDLPlayer::reverbIdx]) >> 2;
            MicroModSDLPlayer::reverbBuffer[MicroModSDLPlayer::reverbIdx] = buffer[MicroModSDLPlayer::reverbIdx];
            MicroModSDLPlayer::reverbBuffer[MicroModSDLPlayer::reverbIdx + 1] = buffer[MicroModSDLPlayer::reverbIdx + 1];
            MicroModSDLPlayer::reverbIdx += 2;
            if (MicroModSDLPlayer::reverbIdx >= MicroModSDLPlayer::reverbLen)
            {
                MicroModSDLPlayer::reverbIdx = 0;
            }
            MicroModSDLPlayer::reverbIdx += 2;
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
    count = len * MicroModSDLPlayer::OVERSAMPLE / 4;
    if (MicroModSDLPlayer::samplesRemaining < count)
    {
        /* Clear output.*/
        memset(stream, 0, len);
        count = samplesRemaining;
    }
    if (count > 0)
    {
        /* Get audio from replay.*/
        memset(MicroModSDLPlayer::mixBuffer, 0, count * NUM_CHANNELS * sizeof(short));
        MicroModUtils::getInstance()->getAudio(MicroModSDLPlayer::mixBuffer, count);
        downSample(MicroModSDLPlayer::mixBuffer, (short *)stream, count);
        crossfeed((short *)stream, count / MicroModSDLPlayer::OVERSAMPLE);
        reverb((short *)stream, count / MicroModSDLPlayer::OVERSAMPLE);
        MicroModSDLPlayer::samplesRemaining -= count;
    }
    else
    {
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
    char string[23];
    for (inst = 0; inst < 16; inst++)
    {
        MicroModUtils::getInstance()->getString(inst, string);
        printf("%02i - %-22s ", inst, string);
        MicroModUtils::getInstance()->getString(inst + 16, string);
        printf("%02i - %-22s\n", inst + 16, string);
    }
}

long MicroModSDLPlayer::playModule(unsigned char module[])
{
    long result;
    SDL_AudioSpec audiospec;
    /* Initialise replay.*/
    result = MicroModUtils::getInstance()->initialise(module, MicroModSDLPlayer::SAMPLING_FREQ * OVERSAMPLE);
    if (result == 0)
    {
        printModuleInfo();
        /* Calculate song length. */
        MicroModSDLPlayer::samplesRemaining = MicroModUtils::getInstance()->calculateSongDuration();
        printf("Song Duration: %li seconds.\n", MicroModSDLPlayer::samplesRemaining / (SAMPLING_FREQ * OVERSAMPLE));
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
            /* Open the audio device. */
            result = SDL_OpenAudio(&audiospec, NULL);
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
                SDL_Quit();
            }
            else
            {
                fprintf(stderr, "Unable to open audio device: %s\n", SDL_GetError());
            }
        }
        else
        {
            fprintf(stderr, "Unable to initialise SDL: %s\n", SDL_GetError());
        }
    }
    else
    {
        fprintf(stderr, "Unable to initialise replay.\n");
    }
    return result;
}

MicroModSDLPlayer::MicroModSDLPlayer() {}

MicroModSDLPlayer::~MicroModSDLPlayer() {}