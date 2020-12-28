#include <iostream>
#include <string>
#include <vector>

#define __CHANNELS__ 16
#define __FP_SHIFT__ 14
#define __FP_ONE__ 16384

class MicroModUtils
{
public:
    struct note
    {
        unsigned short key;
        unsigned char instrument, effect, param;
    };

    struct instrument
    {
        unsigned char volume, fine_tune;
        unsigned long loop_start, loop_length;
        signed char *sample_data;
    };

    struct channel
    {
        struct note note;
        unsigned short period, porta_period;
        unsigned long sample_offset, sample_idx, step;
        unsigned char volume, panning, fine_tune, ampl, mute;
        unsigned char id, instrument, assigned, porta_speed, pl_row, fx_count;
        unsigned char vibrato_type, vibrato_phase, vibrato_speed, vibrato_depth;
        unsigned char tremolo_type, tremolo_phase, tremolo_speed, tremolo_depth;
        signed char tremolo_add, vibrato_add, arpeggio_add;
    };

    static inline int MAX_CHANNELS = __CHANNELS__;

    static inline int FP_SHIFT = __FP_SHIFT__;

    static inline unsigned long FP_ONE = __FP_ONE__;

    static inline unsigned short fine_tuning[] = {
        4340, 4308, 4277, 4247, 4216, 4186, 4156, 4126,
        4096, 4067, 4037, 4008, 3979, 3951, 3922, 3894};

    static inline unsigned short arp_tuning[] = {
        4096, 3866, 3649, 3444, 3251, 3069, 2896, 2734,
        2580, 2435, 2299, 2170, 2048, 1933, 1825, 1722};

    static inline unsigned char sine_table[] = {
        0, 24, 49, 74, 97, 120, 141, 161, 180, 197, 212, 224, 235, 244, 250, 253,
        255, 253, 250, 244, 235, 224, 212, 197, 180, 161, 141, 120, 97, 74, 49, 24};

    static inline MicroModUtils *getInstance()
    {
        if (!instance)
        {
            instance = new MicroModUtils();
        }
        return instance;
    }

    std::string getVersion();
    long calculateNumChannels(unsigned char moduleHeader[]);
    long calculateNumPatterns(unsigned char moduleHeader[]);
    long unsignedShortBigEndian(unsigned char buf[], long offset);
    void setTempo(long tempo);
    void updateFrequency(channel *chan);
    void tonePortamento(channel *chan);
    void volumeSlide(channel *chan, long param);
    long waveform(long phase, long type);
    void vibrato(channel *chan);
    void tremolo(channel *chan);
    void trigger(channel *channel);
    void channelRow(channel *chan);
    void channelTick(channel *chan);
    long sequenceRow();
    long sequenceTick();
    void resample(channel *chan, short *buf, long offset, long count);
    long calculateModFileLen(unsigned char moduleHeader[]);
    long initialise(unsigned char data[], long sampling_rate);
    std::string getInstrumentName(long instrument);
    long calculateSongDuration();
    void setPosition(long pos);
    long muteChannel(long channel);
    void setGain(long value);
    void getAudio(short outputBuffer[], long count);

private:
    static inline MicroModUtils *instance = NULL;
    unsigned char *module_data;
    unsigned char *pattern_data, *sequence;
    long song_length, restart, num_patterns, num_channels;
    instrument instruments[32];

    long sample_rate, gain, c2_rate, tick_len, tick_offset;
    long pattern, break_pattern, row, next_row, tick;
    long speed, pl_count, pl_channel, random_seed;

    channel channels[__CHANNELS__];
    static inline std::string MICROMOD_VERSION = "Based on: Micromod Protracker replay 20180625 (c)mumart@gmail.com";
    MicroModUtils();
    ~MicroModUtils();
};
