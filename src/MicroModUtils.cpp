#include "../include/MicroModUtils.hpp"
#include <string>
using namespace std;

string MicroModUtils::getVersion()
{
    return MicroModUtils::MICROMOD_VERSION;
}

long MicroModUtils::calculateNumChannels(unsigned char moduleHeader[])
{
    long numchan;
    switch ((moduleHeader[1082] << 8) | moduleHeader[1083])
    {
    case 0x4b2e: /* M.K. */
    case 0x4b21: /* M!K! */
    case 0x542e: /* N.T. */
    case 0x5434: /* FLT4 */
        numchan = 4;
        break;
    case 0x484e: /* xCHN */
        numchan = moduleHeader[1080] - 48;
        break;
    case 0x4348: /* xxCH */
        numchan = ((moduleHeader[1080] - 48) * 10) + (moduleHeader[1081] - 48);
        break;
    default: /* Not recognised. */
        numchan = 0;
        break;
    }
    if (numchan > MicroModUtils::MAX_CHANNELS)
        numchan = 0;
    return numchan;
}

long MicroModUtils::calculateNumPatterns(unsigned char moduleHeader[])
{
    long num_patterns, order_entry, pattern;
    num_patterns = 0;
    for (pattern = 0; pattern < 128; pattern++)
    {
        order_entry = moduleHeader[952 + pattern] & 0x7F;
        if (order_entry >= num_patterns)
            num_patterns = order_entry + 1;
    }
    return num_patterns;
}

void MicroModUtils::updateFrequency(struct MicroModUtils::channel *chan)
{
    long period, volume;
    unsigned long freq;
    period = chan->period + chan->vibrato_add;
    period = period * arp_tuning[chan->arpeggio_add] >> 11;
    period = (period >> 1) + (period & 1);
    if (period < 14)
        period = 6848;
    freq = c2_rate * 428 / period;
    chan->step = (freq << MicroModUtils::FP_SHIFT) / sample_rate;
    volume = chan->volume + chan->tremolo_add;
    if (volume > 64)
        volume = 64;
    if (volume < 0)
        volume = 0;
    chan->ampl = (volume * gain) >> 5;
}

void MicroModUtils::setTempo(long tempo)
{
    tick_len = ((sample_rate << 1) + (sample_rate >> 1)) / tempo;
}

void MicroModUtils::tonePortamento(struct MicroModUtils::channel *chan)
{
    long source, dest;
    source = chan->period;
    dest = chan->porta_period;
    if (source < dest)
    {
        source += chan->porta_speed;
        if (source > dest)
            source = dest;
    }
    else if (source > dest)
    {
        source -= chan->porta_speed;
        if (source < dest)
            source = dest;
    }
    chan->period = source;
}

void MicroModUtils::volumeSlide(struct channel *chan, long param)
{
    long volume;
    volume = chan->volume + (param >> 4) - (param & 0xF);
    if (volume < 0)
        volume = 0;
    if (volume > 64)
        volume = 64;
    chan->volume = volume;
}
long MicroModUtils::waveform(long phase, long type)
{
    long amplitude = 0;
    switch (type & 0x3)
    {
    case 0: /* Sine. */
        amplitude = sine_table[phase & 0x1F];
        if ((phase & 0x20) > 0)
            amplitude = -amplitude;
        break;
    case 1: /* Saw Down. */
        amplitude = 255 - (((phase + 0x20) & 0x3F) << 3);
        break;
    case 2: /* Square. */
        amplitude = 255 - ((phase & 0x20) << 4);
        break;
    case 3: /* Random. */
        amplitude = (random_seed >> 20) - 255;
        random_seed = (random_seed * 65 + 17) & 0x1FFFFFFF;
        break;
    }
    return amplitude;
}
void MicroModUtils::vibrato(struct channel *chan)
{
    chan->vibrato_add = waveform(chan->vibrato_phase, chan->vibrato_type) * chan->vibrato_depth >> 7;
}
void MicroModUtils::tremolo(struct channel *chan)
{
    chan->tremolo_add = waveform(chan->tremolo_phase, chan->tremolo_type) * chan->tremolo_depth >> 6;
}
void MicroModUtils::trigger(struct channel *channel)
{
    long period, ins;
    ins = channel->note.instrument;
    if (ins > 0 && ins < 32)
    {
        channel->assigned = ins;
        channel->sample_offset = 0;
        channel->fine_tune = instruments[ins].fine_tune;
        channel->volume = instruments[ins].volume;
        if (instruments[ins].loop_length > 0 && channel->instrument > 0)
            channel->instrument = ins;
    }
    if (channel->note.effect == 0x09)
    {
        channel->sample_offset = (channel->note.param & 0xFF) << 8;
    }
    else if (channel->note.effect == 0x15)
    {
        channel->fine_tune = channel->note.param;
    }
    if (channel->note.key > 0)
    {
        period = (channel->note.key * fine_tuning[channel->fine_tune & 0xF]) >> 11;
        channel->porta_period = (period >> 1) + (period & 1);
        if (channel->note.effect != 0x3 && channel->note.effect != 0x5)
        {
            channel->instrument = channel->assigned;
            channel->period = channel->porta_period;
            channel->sample_idx = (channel->sample_offset << FP_SHIFT);
            if (channel->vibrato_type < 4)
                channel->vibrato_phase = 0;
            if (channel->tremolo_type < 4)
                channel->tremolo_phase = 0;
        }
    }
}
void MicroModUtils::channelRow(struct MicroModUtils::channel *chan)
{
    long effect, param, volume, period;
    effect = chan->note.effect;
    param = chan->note.param;
    chan->vibrato_add = chan->tremolo_add = chan->arpeggio_add = chan->fx_count = 0;
    if (!(effect == 0x1D && param > 0))
    {
        /* Not note delay. */
        trigger(chan);
    }
    switch (effect)
    {
    case 0x3: /* Tone Portamento.*/
        if (param > 0)
            chan->porta_speed = param;
        break;
    case 0x4: /* Vibrato.*/
        if ((param & 0xF0) > 0)
            chan->vibrato_speed = param >> 4;
        if ((param & 0x0F) > 0)
            chan->vibrato_depth = param & 0xF;
        vibrato(chan);
        break;
    case 0x6: /* Vibrato + Volume Slide.*/
        vibrato(chan);
        break;
    case 0x7: /* Tremolo.*/
        if ((param & 0xF0) > 0)
            chan->tremolo_speed = param >> 4;
        if ((param & 0x0F) > 0)
            chan->tremolo_depth = param & 0xF;
        tremolo(chan);
        break;
    case 0x8: /* Set Panning (0-127). Not for 4-channel Protracker. */
        if (num_channels != 4)
        {
            chan->panning = (param < 128) ? param : 127;
        }
        break;
    case 0xB: /* Pattern Jump.*/
        if (pl_count < 0)
        {
            break_pattern = param;
            next_row = 0;
        }
        break;
    case 0xC: /* Set Volume.*/
        chan->volume = param > 64 ? 64 : param;
        break;
    case 0xD: /* Pattern Break.*/
        if (pl_count < 0)
        {
            if (break_pattern < 0)
                break_pattern = pattern + 1;
            next_row = (param >> 4) * 10 + (param & 0xF);
            if (next_row >= 64)
                next_row = 0;
        }
        break;
    case 0xF: /* Set Speed.*/
        if (param > 0)
        {
            if (param < 32)
                tick = speed = param;
            else
                setTempo(param);
        }
        break;
    case 0x11: /* Fine Portamento Up.*/
        period = chan->period - param;
        chan->period = period < 0 ? 0 : period;
        break;
    case 0x12: /* Fine Portamento Down.*/
        period = chan->period + param;
        chan->period = period > 65535 ? 65535 : period;
        break;
    case 0x14: /* Set Vibrato Waveform.*/
        if (param < 8)
            chan->vibrato_type = param;
        break;
    case 0x16:          /* Pattern Loop.*/
        if (param == 0) /* Set loop marker on this channel. */
            chan->pl_row = row;
        if (chan->pl_row < row && break_pattern < 0)
        { /* Marker valid. */
            if (pl_count < 0)
            { /* Not already looping, begin. */
                pl_count = param;
                pl_channel = chan->id;
            }
            if (pl_channel == chan->id)
            { /* Next Loop.*/
                if (pl_count == 0)
                { /* Loop finished. */
                    /* Invalidate current marker. */
                    chan->pl_row = row + 1;
                }
                else
                { /* Loop. */
                    next_row = chan->pl_row;
                }
                pl_count--;
            }
        }
        break;
    case 0x17: /* Set Tremolo Waveform.*/
        if (param < 8)
            chan->tremolo_type = param;
        break;
    case 0x1A: /* Fine Volume Up.*/
        volume = chan->volume + param;
        chan->volume = volume > 64 ? 64 : volume;
        break;
    case 0x1B: /* Fine Volume Down.*/
        volume = chan->volume - param;
        chan->volume = volume < 0 ? 0 : volume;
        break;
    case 0x1C: /* Note Cut.*/
        if (param <= 0)
            chan->volume = 0;
        break;
    case 0x1E: /* Pattern Delay.*/
        tick = speed + speed * param;
        break;
    }
    updateFrequency(chan);
}

MicroModUtils::MicroModUtils()
{
    cout << "MicroModUtils creation." << endl;
}

MicroModUtils::~MicroModUtils()
{
    MicroModUtils::instance = NULL;
}
