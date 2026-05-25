#pragma once
#include "Envelopes.h"
#include "Oscillator.h"
#include <vector>

static Oscillator oscillator = Oscillator();

struct OscillatorBreakdown
{
	bool is_FM = false;
	OscillatorTypes oscillator_type = SIN;
	double LFO_hertz = 0;
	double LFO_amp = 0;
};

struct FrequencyBreakdown
{
	OscillatorBreakdown osc{};
	double amp = 1.0;
	int relative_semitones = 0;
};

class Instrument
{
protected:
	//overall volume
	double volume = 1.0;

public:
	Instrument(const ADSREnvelope& envelope, const std::vector<FrequencyBreakdown>& constituent_freqs) { env = envelope; constituent_frequencies = constituent_freqs; CalculateMaxAmp(); }
	Instrument() {}
	~Instrument() {}
	ADSREnvelope env;
	double MoveSemitones(float curFreq, int numSemitones);
	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate = 44100);

	void CalculateMaxAmp();

	//Uses additive synthesis of constituent frequencies making up a sound
	//storing the maximum amplitude from the addition allows for normalisation of the volume so as not to break ears (:
	double additive_max_amp = 0;
	std::vector<FrequencyBreakdown> constituent_frequencies;
};

//OBSELETE
class Silence : public Instrument
{
public:
	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate) override final;
};