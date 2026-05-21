#pragma once
#include "Instrument.h"
#include "WriteToWav.h"
#include "MarkovChain.h"
#include "MidiParser.h"

const enum
{
	C0, Cs0, D0, Ds0, E0, F0, Fs0, G0, Gs0, A0, As0, B0,
	C1, Cs1, D1, Ds1, E1, F1, Fs1, G1, Gs1, A1, As1, B1,
	C2, Cs2, D2, Ds2, E2, F2, Fs2, G2, Gs2, A2, As2, B2,
	C3, Cs3, D3, Ds3, E3, F3, Fs3, G3, Gs3, A3, As3, B3,
	C4, Cs4, D4, Ds4, E4, F4, Fs4, G4, Gs4, A4, As4, B4,
	C5, Cs5, D5, Ds5, E5, F5, Fs5, G5, Gs5, A5, As5, B5,
	C6, Cs6, D6, Ds6, E6, F6, Fs6, G6, Gs6, A6, As6, B6,
	C7, Cs7, D7, Ds7, E7, F7, Fs7, G7, Gs7, A7, As7, B7,
	C8, Cs8, D8, Ds8, E8, F8, Fs8, G8, Gs8, A8, As8, B8,
};
static const float Note_To_Frequency[108]
{
	16.35f, 17.32f, 18.35f, 19.45f, 20.60f, 21.83f, 23.12f, 24.50f, 25.96f, 27.50f, 29.14f, 30.87f,
	32.70f, 34.65f, 36.71f, 38.89f, 41.20f, 43.65f, 46.25f, 49.00f, 51.91f, 55.00f, 58.27f, 61.74f,
	65.41f, 69.30f, 73.42f, 77.78f, 82.41f, 87.31f, 92.50f, 98.00f, 103.86f, 110.00f, 116.54f, 123.47f,
	130.81f, 138.59f, 146.83f, 155.56f, 164.81f, 174.61f, 185.00f, 196.00f, 207.65f, 220.00f, 233.08f, 246.94f,
	261.63f, 277.18f, 293.66f, 311.13f, 329.63f, 349.23f, 369.99f, 392.00f, 415.30f, 440.00f, 466.16f, 493.88f,
	523.25f, 554.37f, 587.33f, 622.25f, 659.26f, 698.46f, 739.99f, 783.99f, 830.61f, 880.00f, 932.33f, 987.77f,
	1046.50f, 1108.73f, 1174.66f, 1244.51f, 1318.51f, 1396.91f, 1479.98f, 1567.98f, 1661.22f, 1760.00f, 1864.66f, 1975.53f,
	2093.00f, 2217.46f, 2349.32f, 2489.02f, 2637.02f, 2793.83f, 2959.96f, 3135.96f, 3322.44f, 3520.00f, 3729.31f, 3951.07f,
	4186.01f, 4434.92f, 4698.64f, 4978.03f, 5274.04f, 5587.65f, 5919.91f, 6271.93f, 6644.88f, 7040.00f, 7458.62f, 7902.13f
};

const int NUM_AVAILABLE_INSTRUMENTS = 12;

const enum Instruments
{
	PIANO, MARIMBA, HARMONICA, ACOUSTIC_GUITAR, BASS_GUITAR, VIOLIN, TRUMPET, FLUTE, BANJO, DRUM, SYNTHSQUARE, SYNTHSAW
};

class SoundGenerator
{
private:
	//possible instruments to play
	Instrument instruments[NUM_AVAILABLE_INSTRUMENTS];
	MarkovChain* markov_chains[NUM_AVAILABLE_INSTRUMENTS];

	//a buffer to store the sound values in (1 for each state so they can be combined)
	std::vector<double>* note_buffers[NUM_AVAILABLE_INSTRUMENTS*NUM_POSSIBLE_SYNCHRONOUS_STATES];

	//so the music can be outputted as a wav
	WavWriter* wav_output;

	void Init_Instruments();

	void Generate_Note(float frequency, double duration, float amplitude, Instruments instrument, int buffer_index);

	bool Get_File_Instrument(std::string filename, File_Type file_type, std::vector<int>& instruments);

	//TODO
	//void Pre_Generate_Instrument_Sounds();

public:
	SoundGenerator();
	~SoundGenerator();

	bool Load_File_Into_Generator(std::string filename);

	void Generate_Music(int length = -1);
};

