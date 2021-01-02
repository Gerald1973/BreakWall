#include <string>
#include <cstring>
#include <iostream>
#include "../include/CustomEventUtils.hpp"
#include "../include/MicroModSDLPlayer.hpp"

using namespace std;

/*
 2:1 downsampling with simple but effective anti-aliasing.
 Count is the number of stereo samples to process, and must be even.
 input may point to the same buffer as output.
 */
void MicroModSDLPlayer::downSample(short *input, short *output, long count) {
	long in_idx, out_idx, out_l, out_r;
	in_idx = out_idx = 0;
	while (out_idx < count) {
		out_l = filtL + (input[in_idx++] >> 1);
		out_r = filtR + (input[in_idx++] >> 1);
		filtL = input[in_idx++] >> 2;
		filtR = input[in_idx++] >> 2;
		output[out_idx++] = out_l + filtL;
		output[out_idx++] = out_r + filtR;
	}
}

/* Simple stereo cross delay with feedback. */
void MicroModSDLPlayer::reverb(short *buffer, long count) {
	long buffer_idx, buffer_end;
	if (reverbLen > 2) {
		buffer_idx = 0;
		buffer_end = buffer_idx + (count << 1);
		while (buffer_idx < buffer_end) {
			buffer[buffer_idx] = (buffer[buffer_idx] * 3 + reverbBuffer[reverbIdx + 1]) >> 2;
			buffer[buffer_idx + 1] = (buffer[buffer_idx + 1] * 3 + reverbBuffer[reverbIdx]) >> 2;
			reverbBuffer[reverbIdx] = buffer[reverbIdx];
			reverbBuffer[reverbIdx + 1] = buffer[reverbIdx + 1];
			reverbIdx += 2;
			if (reverbIdx >= reverbLen) {
				reverbIdx = 0;
			}
			reverbIdx += 2;
		}
	}
}

/* Reduce stereo-separation of count samples. */
void MicroModSDLPlayer::crossfeed(short *audio, int count) {
	int l, r, offset = 0, end = count << 1;
	while (offset < end) {
		l = audio[offset];
		r = audio[offset + 1];
		audio[offset++] = (l + l + l + r) >> 2;
		audio[offset++] = (r + r + r + l) >> 2;
	}
}

Uint8* MicroModSDLPlayer::buildStream(int len) {
	long count = len * OVERSAMPLE / 4;
	Uint8* result = (Uint8 *) calloc(len, 1);
	/*for (int i = 0; i < len;i++) {
		std::cout << i << ": " << (int) result[i] << std::endl;
	}*/
	std::cout << "Sample remaining 01: " << samplesRemaining << std::endl;
	if (samplesRemaining < count) {
		count = samplesRemaining;
	}
	std::cout << "Sample remaining 02: " << samplesRemaining << std::endl;
	if (count > 0) {
		/* Get audio from replay.*/
		//memset(mixBuffer, 0, count * NUM_CHANNELS * sizeof(short));
		mixBuffer = (short*) calloc(count * NUM_CHANNELS, sizeof (short));
		std::cout << "Sample remaining 02-01: " << samplesRemaining << std::endl;
		microModUtils->getAudio(mixBuffer, count);
		std::cout << "Sample remaining 02-02: " << samplesRemaining << std::endl;
		downSample(mixBuffer, (short*) result, count);
		std::cout << "Sample remaining 02-03: " << samplesRemaining << std::endl;
		crossfeed((short*) result, count / OVERSAMPLE);
		std::cout << "Sample remaining 02-04: " << samplesRemaining << std::endl;
		reverb((short*) result, count / OVERSAMPLE);
		std::cout << "Sample remaining 02-05: " << samplesRemaining << std::endl;
		samplesRemaining -= count;
		std::cout << "Sample remaining 03: " << samplesRemaining << std::endl;
	} else {
		CustomEventUtils::getInstance()->postEventSongStop(new string("Title of the song"));
	}
//	for (int i = 0; i < len;i++) {
//		std::cout << i << ": " << (int) result[i] << std::endl;
//	}
	std::cout << "Sample remaining 04: " << samplesRemaining << std::endl;
	return result;
}

/* Notify the main thread to stop playback. */
void MicroModSDLPlayer::terminationHandler(int signum) {
	fprintf(stderr, "\nTerminated!\n");
}

void MicroModSDLPlayer::printModuleInfo() {
	int inst;
	string tmp = "";
	for (inst = 0; inst < 16; inst++) {
		tmp = microModUtils->getInstrumentName(inst);
		cout << inst << ":" << tmp << " ";
		tmp = microModUtils->getInstrumentName(inst + 16);
		cout << inst + 16 << ":" << tmp << endl;
	}
}

SDL_AudioDeviceID MicroModSDLPlayer::initialise(unsigned char module[]) {
	SDL_AudioSpec desired, obtained;
	/* Initialise replay.*/
	long result = microModUtils->initialise(module, SAMPLING_FREQ * OVERSAMPLE);
	if (result == 0) {
		samplesRemaining = microModUtils->calculateSongDuration();
		long duration = samplesRemaining / (SAMPLING_FREQ * OVERSAMPLE);
		cout << "DEBUG: samples remaining  : " << samplesRemaining << endl;
		cout << "Song Duration             : " << duration << endl;
		desired.freq     = SAMPLING_FREQ;
		desired.format   = AUDIO_S16SYS;
		desired.channels = NUM_CHANNELS;
		desired.samples  = BUFFER_SAMPLES;
		desired.callback = nullptr;
		desired.userdata = nullptr;
		/* Initialise audio subsystem. */
		result = SDL_Init(SDL_INIT_AUDIO);
		if (result == 0) {
			result = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, 0);
			std::cout << "Desired  format              : " << desired.format  << std::endl;
			std::cout << "Obtained format              : " << obtained.format  << std::endl;
			std::cout << "Desired  number of channels  : " << (int) desired.channels << std::endl;
			std::cout << "Obtained number of channels  : " << (int) obtained.channels << std::endl;
			std::cout << "Buffer size                  : " << (int) obtained.size << std::endl;
		} else {
			fprintf(stderr, "Unable to open audio device: %s\n", SDL_GetError());
		}
	}
	return result;
}

long MicroModSDLPlayer::playModule(unsigned char module[]) {
	long result = this->initialise(module);
	if (result == 0) {
		SDL_PauseAudio(0);
	}
	return result;
}

MicroModSDLPlayer::MicroModSDLPlayer() {
	std::cout << "MicroModSDLPlayer creation." << std::endl;
	OVERSAMPLE = __OVERSAMPLE__;
	NUM_CHANNELS = __NUM_CHANNELS__;
	SAMPLING_FREQ = __SAMPLING_FREQ__;
	BUFFER_SAMPLES = __BUFFER_SAMPLES__;
	filtL = 0;
	filtR = 0;
	reverbLen = 0;
	reverbIdx = 0;
	samplesRemaining = 0;
	mixBuffer=nullptr;
	microModUtils = new MicroModUtils();
}

MicroModSDLPlayer::~MicroModSDLPlayer() {
}

long MicroModSDLPlayer::getSamplesRemaining() {
	return this->samplesRemaining;
}
