#include "Instrument.h"

//Moves a frequency by a number of semitones e.g. a 12 semitone jump is up an octave
double Instrument::MoveSemitones(float curFreq, double numSemitones)
{
	return curFreq * pow(semitone_exponent, numSemitones);
}

//Finds the maximum possible value from additive synthesis for this instrument, used to normalise amplitude
void Instrument::CalculateMaxAmp()
{
	additive_max_amp = 0;

	for (const FrequencyBreakdown& frequency : constituent_frequencies)
	{
		additive_max_amp += frequency.amp;
	}
}

//Fills a buffer with the amplitude values for the instrument for a given duration
void Instrument::Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate)
{
	//How far through the duration
	float time = 0;
	//The distance to step each iteration
	float timeOffset = 1 / sampleRate;
	//A calculated value to phase out/in sounds to prevent loud clicks
	double fade_out = 1;
	double fade_in = 1;
	//How many samples are going to be created
	double total_samples = sampleRate * duration;

	buffer->reserve(static_cast<size_t>(total_samples));

	for (int i = 0; i < total_samples; i++)
	{
		double sample = 0;
		//add all the amplitudes of all constiuent frequencies that make this note
		for (const FrequencyBreakdown& freq : constituent_frequencies)
		{
			sample += freq.amp * oscillator.GetValue(MoveSemitones(frequency, freq.relative_semitones), time, freq.osc.oscillator_type, freq.phase, freq.osc.LFO_hertz, freq.osc.LFO_amp);
		}

		//Modify by the amplitude of the envelope
		double amp = env.GetAmplitude(time) * amplitude;

		//Modify by instrument volume and account for fading in and out the note to prevent harsh sounds
		sample = amp * sample * volume * fade_out * fade_in;

		//clamp amplitude between 0-1
		sample /= additive_max_amp;

		//increment the time
		time += timeOffset;

		//if needed, calculate new fade ins  / fade outs
		if (buffer->size() < total_samples * 0.05)
		{
			double percent_through = buffer->size() / total_samples;
			fade_in = 20 * percent_through;
		}
		else if (buffer->size() > total_samples * 0.95)
		{
			//percentage through total samples
			double percent_through = buffer->size() / total_samples;
			fade_out = (-20 * percent_through) + 20;
		}

		//Add sample to the buffer
		buffer->push_back(sample);
	}
}

void Silence::Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate)
{
	for (int i = 0; i < sampleRate * duration; i++)
	{
		buffer->push_back(0);
	}
}

WaveData::WaveData(float frequency, float amp, float phase)
{
	this->frequency = frequency;
	this->amp = amp;
	this->phase = phase;
}
