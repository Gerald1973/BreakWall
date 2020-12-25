#include "../include/MicroModUtils.hpp"
#include <string>
#include <iostream>
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

long MicroModUtils::unsignedShortBigEndian(unsigned char buf[], long offset)
{
    return ((buf[offset] & 0xFF) << 8) | (buf[offset + 1] & 0xFF);
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
        //if (instruments[ins].loop_length > 0 && channel->instrument > 0)
        if (instruments[ins].loop_length > 0)
        {
            channel->instrument = ins;
        }
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

void MicroModUtils::channelTick(struct MicroModUtils::channel *chan)
{
    long effect, param, period;
    effect = chan->note.effect;
    param = chan->note.param;
    chan->fx_count++;
    switch (effect)
    {
    case 0x1: /* Portamento Up.*/
        period = chan->period - param;
        chan->period = period < 0 ? 0 : period;
        break;
    case 0x2: /* Portamento Down.*/
        period = chan->period + param;
        chan->period = period > 65535 ? 65535 : period;
        break;
    case 0x3: /* Tone Portamento.*/
        tonePortamento(chan);
        break;
    case 0x4: /* Vibrato.*/
        chan->vibrato_phase += chan->vibrato_speed;
        vibrato(chan);
        break;
    case 0x5: /* Tone Porta + Volume Slide.*/
        tonePortamento(chan);
        volumeSlide(chan, param);
        break;
    case 0x6: /* Vibrato + Volume Slide.*/
        chan->vibrato_phase += chan->vibrato_speed;
        vibrato(chan);
        volumeSlide(chan, param);
        break;
    case 0x7: /* Tremolo.*/
        chan->tremolo_phase += chan->tremolo_speed;
        tremolo(chan);
        break;
    case 0xA: /* Volume Slide.*/
        volumeSlide(chan, param);
        break;
    case 0xE: /* Arpeggio.*/
        if (chan->fx_count > 2)
            chan->fx_count = 0;
        if (chan->fx_count == 0)
            chan->arpeggio_add = 0;
        if (chan->fx_count == 1)
            chan->arpeggio_add = param >> 4;
        if (chan->fx_count == 2)
            chan->arpeggio_add = param & 0xF;
        break;
    case 0x19: /* Retrig.*/
        if (chan->fx_count >= param)
        {
            chan->fx_count = 0;
            chan->sample_idx = 0;
        }
        break;
    case 0x1C: /* Note Cut.*/
        if (param == chan->fx_count)
            chan->volume = 0;
        break;
    case 0x1D: /* Note Delay.*/
        if (param == chan->fx_count)
            trigger(chan);
        break;
    }
    if (effect > 0)
        updateFrequency(chan);
}

long MicroModUtils::sequenceRow()
{
    long song_end, chan_idx, pat_offset;
    long effect, param;
    struct note *note;
    song_end = 0;
    if (next_row < 0)
    {
        break_pattern = pattern + 1;
        next_row = 0;
    }
    if (break_pattern >= 0)
    {
        if (break_pattern >= song_length)
            break_pattern = next_row = 0;
        if (break_pattern <= pattern)
            song_end = 1;
        pattern = break_pattern;
        for (chan_idx = 0; chan_idx < num_channels; chan_idx++)
            channels[chan_idx].pl_row = 0;
        break_pattern = -1;
    }
    row = next_row;
    next_row = row + 1;
    if (next_row >= 64)
        next_row = -1;
    pat_offset = (sequence[pattern] * 64 + row) * num_channels * 4;
    for (chan_idx = 0; chan_idx < num_channels; chan_idx++)
    {
        note = &channels[chan_idx].note;
        note->key = (pattern_data[pat_offset] & 0xF) << 8;
        note->key |= pattern_data[pat_offset + 1];
        note->instrument = pattern_data[pat_offset + 2] >> 4;
        note->instrument |= pattern_data[pat_offset] & 0x10;
        effect = pattern_data[pat_offset + 2] & 0xF;
        param = pattern_data[pat_offset + 3];
        pat_offset += 4;
        if (effect == 0xE)
        {
            effect = 0x10 | (param >> 4);
            param &= 0xF;
        }
        if (effect == 0 && param > 0)
            effect = 0xE;
        note->effect = effect;
        note->param = param;
        channelRow(&channels[chan_idx]);
    }
    return song_end;
}

long MicroModUtils::sequenceTick()
{
    long song_end, chan_idx;
    song_end = 0;
    if (--tick <= 0)
    {
        tick = speed;
        song_end = sequenceRow();
    }
    else
    {
        for (chan_idx = 0; chan_idx < num_channels; chan_idx++)
            channelTick(&channels[chan_idx]);
    }
    return song_end;
}
void MicroModUtils::resample(channel *chan, short *buf, long offset, long count)
{
    unsigned long epos;
    unsigned long buf_idx = offset << 1;
    unsigned long buf_end = (offset + count) << 1;
    unsigned long sidx = chan->sample_idx;
    unsigned long step = chan->step;
    unsigned long llen = instruments[chan->instrument].loop_length;
    unsigned long lep1 = instruments[chan->instrument].loop_start + llen;
    signed char *sdat = instruments[chan->instrument].sample_data;
    short ampl = buf && !chan->mute ? chan->ampl : 0;
    short lamp = ampl * (127 - chan->panning) >> 5;
    short ramp = ampl * chan->panning >> 5;
    while (buf_idx < buf_end)
    {
        if (sidx >= lep1)
        {
            /* Handle loop. */
            if (llen <= FP_ONE)
            {
                /* One-shot sample. */
                sidx = lep1;
                break;
            }
            /* Subtract loop-length until within loop points. */
            while (sidx >= lep1)
                sidx -= llen;
        }
        /* Calculate sample position at end. */
        epos = sidx + ((buf_end - buf_idx) >> 1) * step;
        /* Most of the cpu time is spent here. */
        if (lamp || ramp)
        {
            /* Only mix to end of current loop. */
            if (epos > lep1)
                epos = lep1;
            if (lamp && ramp)
            {
                /* Mix both channels. */
                while (sidx < epos)
                {
                    ampl = sdat[sidx >> FP_SHIFT];
                    buf[buf_idx++] += ampl * lamp >> 2;
                    buf[buf_idx++] += ampl * ramp >> 2;
                    sidx += step;
                }
            }
            else
            {
                /* Only mix one channel. */
                if (ramp)
                    buf_idx++;
                while (sidx < epos)
                {
                    buf[buf_idx] += sdat[sidx >> FP_SHIFT] * ampl;
                    buf_idx += 2;
                    sidx += step;
                }
                buf_idx &= -2;
            }
        }
        else
        {
            /* No need to mix.*/
            buf_idx = buf_end;
            sidx = epos;
        }
    }
    chan->sample_idx = sidx;
}

long MicroModUtils::calculateModFileLen(unsigned char moduleHeader[])
{
    long length, numchan, inst_idx;
    numchan = calculateNumChannels(moduleHeader);
    if (numchan >= 128)
        return -1;
    length = 1084 + 4 * numchan * 64 * calculateNumPatterns(moduleHeader);
    for (inst_idx = 1; inst_idx < 32; inst_idx++)
        length += unsignedShortBigEndian(moduleHeader, inst_idx * 30 + 12) * 2;
    return length;
}

long MicroModUtils::initialise(unsigned char data[], long sampling_rate)
{
    struct MicroModUtils::instrument *inst;
    long sample_data_offset, inst_idx;
    long sample_length, volume, fine_tune, loop_start, loop_length;
    num_channels = calculateNumChannels(data);
    if (num_channels <= 0)
    {
        num_channels = 0;
        return -1;
    }
    if (sampling_rate < 8000)
        return -2;
    module_data = &data[0];
    sample_rate = sampling_rate;
    song_length = module_data[950] & 0x7F;
    restart = module_data[951] & 0x7F;
    if (restart >= song_length)
        restart = 0;
    sequence = (unsigned char *)module_data + 952;
    pattern_data = (unsigned char *)module_data + 1084;
    num_patterns = calculateNumPatterns(module_data);
    sample_data_offset = 1084 + num_patterns * 64 * num_channels * 4;
    for (inst_idx = 1; inst_idx < 32; inst_idx++)
    {
        inst = &instruments[inst_idx];
        sample_length = unsignedShortBigEndian(module_data, inst_idx * 30 + 12) * 2;
        fine_tune = module_data[inst_idx * 30 + 14] & 0xF;
        inst->fine_tune = (fine_tune & 0x7) - (fine_tune & 0x8) + 8;
        volume = module_data[inst_idx * 30 + 15] & 0x7F;
        inst->volume = volume > 64 ? 64 : volume;
        loop_start = unsignedShortBigEndian(module_data, inst_idx * 30 + 16) * 2;
        loop_length = unsignedShortBigEndian(module_data, inst_idx * 30 + 18) * 2;
        if (loop_start + loop_length > sample_length)
        {
            if (loop_start / 2 + loop_length <= sample_length)
            {
                /* Some old modules have loop start in bytes. */
                loop_start = loop_start / 2;
            }
            else
            {
                loop_length = sample_length - loop_start;
            }
        }
        if (loop_length < 4)
        {
            loop_start = sample_length;
            loop_length = 0;
        }
        inst->loop_start = loop_start << FP_SHIFT;
        inst->loop_length = loop_length << FP_SHIFT;
        inst->sample_data = (signed char *)module_data + sample_data_offset;
        cout << "DEBUG Instrument idx        : " << inst_idx << endl;
        cout << "DEBUG ptrInstrument         : " << inst << endl;
        cout << "DEBUG Instrument data       : " << inst->sample_data << endl;
        cout << "DEBUG Instrument loop start : " << inst->loop_start << endl;
        cout << "DEBUG Instrument loop length: " << inst->loop_length << endl;
        cout << "DEBUG Instrument offset     : " << sample_data_offset << endl;
        cout << "DEBUG Instrument length     : " << sample_length << endl;
        cout << endl;
        sample_data_offset += sample_length;
    }
    c2_rate = (num_channels > 4) ? 8363 : 8287;
    gain = (num_channels > 4) ? 32 : 64;
    muteChannel(-1);
    setPosition(0);
    return 0;
}

/*
	Obtains song and instrument names from the module.
	The song name is returned as instrument 0.
	The name is copied into the location pointed to by string,
	and is at most 23 characters long, including the trailing null.
*/
string MicroModUtils::getString(long instrument)
{
    long index, offset, length, character;
    string result = "";
    if (num_channels > 0)
    {
        offset = 0;
        length = 20;
        if (instrument > 0 && instrument < 32)
        {
            offset = (instrument - 1) * 30 + 20;
            length = 22;
        }
        for (index = 0; index < length; index++)
        {
            character = module_data[offset + index];
            if (character < 32 || character > 126)
            {
                character = ' ';
            }
            result = result + (char)character;
        }
    }
    return result;
}
long MicroModUtils::calculateSongDuration(void)
{
    long duration, song_end;
    duration = 0;
    if (num_channels > 0)
    {
        setPosition(0);
        song_end = 0;
        while (!song_end)
        {
            duration += tick_len;
            song_end = sequenceTick();
        }
        setPosition(0);
    }
    return duration;
}
void MicroModUtils::setPosition(long pos)
{
    long chan_idx;
    struct MicroModUtils::channel *chan;
    if (num_channels <= 0)
        return;
    if (pos >= song_length)
        pos = 0;
    break_pattern = pos;
    next_row = 0;
    tick = 1;
    speed = 6;
    setTempo(125);
    pl_count = pl_channel = -1;
    random_seed = 0xABCDEF;
    for (chan_idx = 0; chan_idx < num_channels; chan_idx++)
    {
        chan = &MicroModUtils::channels[chan_idx];
        chan->id = chan_idx;
        chan->instrument = chan->assigned = 0;
        chan->volume = 0;
        switch (chan_idx & 0x3)
        {
        case 0:
        case 3:
            chan->panning = 0;
            break;
        case 1:
        case 2:
            chan->panning = 127;
            break;
        }
    }
    sequenceTick();
    tick_offset = 0;
}
long MicroModUtils::muteChannel(long channel)
{
    long chan_idx;
    if (channel < 0)
    {
        for (chan_idx = 0; chan_idx < num_channels; chan_idx++)
        {
            channels[chan_idx].mute = 0;
        }
    }
    else if (channel < num_channels)
    {
        channels[channel].mute = 1;
    }
    return num_channels;
}
void MicroModUtils::setGain(long value) { gain = value; }

void MicroModUtils::getAudio(short outputBuffer[], long count)
{
    long offset, remain, chan_idx;
    if (num_channels <= 0)
        return;
    offset = 0;
    while (count > 0)
    {
        remain = tick_len - tick_offset;
        if (remain > count)
            remain = count;
        for (chan_idx = 0; chan_idx < num_channels; chan_idx++)
        {
            resample(&channels[chan_idx], &outputBuffer[0], offset, remain);
        }
        tick_offset += remain;
        if (tick_offset == tick_len)
        {
            sequenceTick();
            tick_offset = 0;
        }
        offset += remain;
        count -= remain;
    }
}

MicroModUtils::MicroModUtils()
{
    cout << "MicroModUtils creation." << endl;
}

MicroModUtils::~MicroModUtils()
{
    MicroModUtils::instance = NULL;
}
