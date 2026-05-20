#include "Instrument.h"

Bell::Bell()
{
	env = new AREnvelope(0.01, 1.0);
}

double Bell::sound(double time, Note note, bool& noteFinished)
{
	double deltaTime = note.timeOn - time;
	double amp = env->GetAmplitude(time, note.timeOn, note.timeOff);
	if (amp <= 0.0) noteFinished = true;
	return amp *
		(1.0 * oscillator.GetValue(Instrument::Scale(note.id + 12), deltaTime, OscillatorTypes::FMSIN, 5.0, 0.001)
		+ 0.5 * oscillator.GetValue(Instrument::Scale(note.id + 24), deltaTime, OscillatorTypes::SIN)
		+ 0.25 * oscillator.GetValue(Instrument::Scale(note.id + 36), deltaTime, OscillatorTypes::SIN))
		* volume;
}

double Harmonica::sound(double time, Note note, bool& noteFinished)
{
	double deltaTime = note.timeOn - time;
	double amp = env->GetAmplitude(time, note.timeOn, note.timeOff);
	if (amp <= 0.0) noteFinished = true;
	return  amp *
		(1.0 * oscillator.GetValue(Instrument::Scale(note.id), deltaTime, OscillatorTypes::FMSQUARE, 5.0, 0.001)
		+ 0.5 * oscillator.GetValue(Instrument::Scale(note.id + 6), deltaTime, OscillatorTypes::SQUARE)
		+ 0.25 * oscillator.GetValue(Instrument::Scale(note.id + 12), deltaTime, OscillatorTypes::SQUARE)
		+ 0.05 * oscillator.GetValue(0, deltaTime, OscillatorTypes::RANDOM))
		* volume;
}

void Harmonica::Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate)
{
	float time = 0;
	float timeOffset = 1/sampleRate;
	double fade_out = 1;
	double fade_in = 1;
	double total_samples = sampleRate * duration;

	buffer->reserve(static_cast<size_t>(total_samples));

	for (int i = 0; i < total_samples; i++)
	{
		double amp = env->GetAmplitude(time) * amplitude;
		double sample = amp*
			(1.0 * oscillator.GetValue(frequency, time, OscillatorTypes::FMSIN, 5.0, 0.001)
				+ 0.5 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 12), time, OscillatorTypes::SIN)
				+ 0.25 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 19), time, OscillatorTypes::SIN)
				+ 0.25 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 24), time, OscillatorTypes::SIN)
				+ 1.0 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 28), time, OscillatorTypes::SIN)
				+ 0.25 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 31), time, OscillatorTypes::SIN)
				+ 0.02 * oscillator.GetValue(0, time, OscillatorTypes::RANDOM))
			* volume * fade_out;
		//clamp between 0-1
		sample /= 3.27;
		time+=timeOffset;
		if (buffer->size() < total_samples * 0.05)
		{
			double percent_through = buffer->size() / total_samples;
			fade_in = 20 * percent_through;
		}
		else if (buffer->size() > total_samples * 0.95)
		{
			//percentage through sample
			double percent_through = buffer->size() / total_samples;
			fade_out = (-20 * percent_through) + 20;
		}
		buffer->push_back(sample);
	}
}

void Piano::Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate)
{
	float time = 0;
	float timeOffset = 1/ sampleRate;
	double fade_out = 1;
	double fade_in = 1;
	double total_samples = sampleRate * duration;
	buffer->reserve(static_cast<size_t>(total_samples));

	for (int i = 0; i < total_samples; i++)
	{
		double amp = env->GetAmplitude(time) * amplitude;
		double sample = amp *
			(1.0 * oscillator.GetValue(frequency, time, OscillatorTypes::FMSIN, 5.0, 0.001)
				+ 0.5 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 12), time, OscillatorTypes::SIN)
				+ 0.25 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 19), time, OscillatorTypes::SIN)
				+ 0.0625 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 24), time, OscillatorTypes::SIN)
				+ 0.0625 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 28), time, OscillatorTypes::SIN)
				+ 0.03125 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 31), time, OscillatorTypes::SIN)
				+ 0.125 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 34), time, OscillatorTypes::SIN))
			* volume * fade_out * fade_in;

		//normalize between 0-1
		sample /= 2.3125;

		time += timeOffset;
		if (buffer->size() < total_samples * 0.05)
		{
			double percent_through = buffer->size() / total_samples;
			fade_in = 20 * percent_through;
		}
		else if (buffer->size() > total_samples * 0.95)
		{
			//percentage through sample
			double percent_through = buffer->size() / total_samples;
			fade_out = (-20 * percent_through) + 20;
		}
		buffer->push_back(sample);
	}
}

void Marimba::Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate)
{
	float time = 0;
	float timeOffset = 1 / sampleRate;
	double fade_out = 1;
	double fade_in = 1;
	double total_samples = sampleRate * duration;
	buffer->reserve(static_cast<size_t>(total_samples));

	for (int i = 0; i < total_samples; i++)
	{
		double amp = env->GetAmplitude(time) * amplitude;
		double sample = amp *
			(1.0 * oscillator.GetValue(frequency, time, OscillatorTypes::FMSIN, 5.0, 0.001)
				+ 0.0625 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 24), time, OscillatorTypes::SIN)
				+ 0.9 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 40), time, OscillatorTypes::SIN)
				+ 0.015 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 47), time, OscillatorTypes::SIN)
				+ 0.0625 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 51), time, OscillatorTypes::SIN)
				+ 0.02 * oscillator.GetValue(0, time, OscillatorTypes::RANDOM))
			* volume * fade_out * fade_in;

		//normalize between 0-1
		sample /= 2.3125;

		time += timeOffset;
		if (buffer->size() < total_samples * 0.05)
		{
			double percent_through = buffer->size() / total_samples;
			fade_in = 20 * percent_through;
		}
		else if (buffer->size() > total_samples * 0.95)
		{
			//percentage through sample
			double percent_through = buffer->size() / total_samples;
			fade_out = (-20 * percent_through) + 20;
		}
		buffer->push_back(sample);
	}
}


void AcousticGuitar::Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate)
{
	float time = 0;
	float timeOffset = 1 / sampleRate;
	double fade_out = 1;
	double fade_in = 1;
	double total_samples = sampleRate * duration;
	buffer->reserve(static_cast<size_t>(total_samples));

	for (int i = 0; i < total_samples; i++)
	{
		double amp = env->GetAmplitude(time) * amplitude;
		double sample = amp *
			(1.0 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 19), time, OscillatorTypes::FMSIN, 5, 0.001)
				+ 0.90 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 12), time, OscillatorTypes::SIN)
				+ 0.0625 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 7), time, OscillatorTypes::SIN)
				+ 0.0625 * oscillator.GetValue(frequency, time, OscillatorTypes::SIN)
				+ 0.0625 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 24), time, OscillatorTypes::SIN)
				+ 0.5 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 28), time, OscillatorTypes::SIN)
				+ 0.25 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 31), time, OscillatorTypes::SIN)
				+ 0.0625 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 34), time, OscillatorTypes::SIN))
			* volume * fade_out * fade_in;

		//normalize between 0-1
		sample /= 2.9;

		time += timeOffset;
		if (buffer->size() < total_samples * 0.05)
		{
			double percent_through = buffer->size() / total_samples;
			fade_in = 20 * percent_through;
		}
		else if (buffer->size() > total_samples * 0.95)
		{
			//percentage through sample
			double percent_through = buffer->size() / total_samples;
			fade_out = (-20 * percent_through) + 20;
		}
		buffer->push_back(sample);
	}
}

void BassGuitar::Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate)
{
	float time = 0;
	float timeOffset = 1 / sampleRate;
	double fade_out = 1;
	double fade_in = 1;
	double total_samples = sampleRate * duration;
	buffer->reserve(static_cast<size_t>(total_samples));

	for (int i = 0; i < total_samples; i++)
	{
		double amp = env->GetAmplitude(time) * amplitude;
		double sample = amp *
			(1.0 * oscillator.GetValue(frequency, time, OscillatorTypes::SAW, 5.0, 0.001)
				+ 0.5 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 12), time, OscillatorTypes::SIN)
				+ 0.125 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 19), time, OscillatorTypes::SIN)
				+ 0.25 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 24), time, OscillatorTypes::SIN)
				+ 0.18 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 28), time, OscillatorTypes::SIN)
				+ 0.0156 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 31), time, OscillatorTypes::SIN))
			* volume * fade_out * fade_in;

		//normalize between 0-1
		sample /= 1.9581;

		time += timeOffset;
		if (buffer->size() < total_samples * 0.05)
		{
			double percent_through = buffer->size() / total_samples;
			fade_in = 20 * percent_through;
		}
		else if (buffer->size() > total_samples * 0.95)
		{
			//percentage through sample
			double percent_through = buffer->size() / total_samples;
			fade_out = (-20 * percent_through) + 20;
		}
		buffer->push_back(sample);
	}
}

void Banjo::Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate)
{
	float time = 0;
	float timeOffset = 1 / sampleRate;
	double fade_out = 1;
	double fade_in = 1;
	double total_samples = sampleRate * duration;
	buffer->reserve(static_cast<size_t>(total_samples));

	for (int i = 0; i < total_samples; i++)
	{
		double amp = env->GetAmplitude(time) * amplitude;
		double sample = amp *
			(1.0 * oscillator.GetValue(frequency, time, OscillatorTypes::FMSIN, 5.0, 0.001)
				+ 0.25 * oscillator.GetValue(Instrument::MoveSemitones(frequency, -12), time, OscillatorTypes::SIN)
				+ 0.5 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 7), time, OscillatorTypes::SIN)
				+ 0.25 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 12), time, OscillatorTypes::SIN)
				+ 0.0018 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 16), time, OscillatorTypes::SIN)
				+ 0.0156 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 19), time, OscillatorTypes::SIN))
			* volume * fade_out * fade_in;

		//normalize between 0-1
		sample /= 2.0174;

		time += timeOffset;
		if (buffer->size() < total_samples * 0.05)
		{
			double percent_through = buffer->size() / total_samples;
			fade_in = 20 * percent_through;
		}
		else if (buffer->size() > total_samples * 0.95)
		{
			//percentage through sample
			double percent_through = buffer->size() / total_samples;
			fade_out = (-20 * percent_through) + 20;
		}
		buffer->push_back(sample);
	}
}


double MyInstrument::sound(double time, Note note, bool& noteFinished)
{
	double deltaTime = note.timeOn - time;
	double amp = env->GetAmplitude(time, note.timeOn, note.timeOff);
	if (amp <= 0.0) noteFinished = true;
	return amp *
		(1.0 * oscillator.GetValue(Instrument::Scale(note.id), deltaTime, OscillatorTypes::FMSIN, 5.0, 0.001)
		+ 0.5 * oscillator.GetValue(Instrument::Scale(note.id +12), deltaTime, OscillatorTypes::SIN)
		+ 0.25 * oscillator.GetValue(Instrument::Scale(note.id +24), deltaTime, OscillatorTypes::SIN))
		* volume;
}

double Instrument::Scale(int noteID)
{
	return 220 * pow(1.0594630943592952645618252949463, noteID);
}

double Instrument::MoveSemitones(float curFreq, int numSemitones)
{
	return curFreq * pow(1.0594630943592952645618252949463, numSemitones);
}

void Silence::Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate)
{
	for (int i = 0; i < sampleRate * duration; i++)
	{
		buffer->push_back(0);
	}
}

void Violin::Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate)
{
	float time = 0;
	float timeOffset = 1 / sampleRate;
	double fade_out = 1;
	double fade_in = 1;
	double total_samples = sampleRate * duration;
	buffer->reserve(static_cast<size_t>(total_samples));

	for (int i = 0; i < total_samples; i++)
	{
		double amp = env->GetAmplitude(time) * amplitude;
		double sample = amp *
			(1.0 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 19), time, OscillatorTypes::SAW, 150.0, 0.003)
				+ 0.7 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 12), time, OscillatorTypes::FMSQUARE)
				+ .25 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 0), time, OscillatorTypes::SIN)
				+ .5 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 24), time, OscillatorTypes::SIN)
				+ 0.25 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 28), time, OscillatorTypes::SIN)
				+ 0.0125 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 31), time, OscillatorTypes::SIN))
			* volume * fade_out * fade_in;

		//normalize between 0-1
		sample /= 2.825;

		time += timeOffset;
		if (buffer->size() < total_samples * 0.05)
		{
			double percent_through = buffer->size() / total_samples;
			fade_in = 20 * percent_through;
		}
		else if (buffer->size() > total_samples * 0.95)
		{
			//percentage through sample
			double percent_through = buffer->size() / total_samples;
			fade_out = (-20 * percent_through) + 20;
		}
		buffer->push_back(sample);
	}
}

void Trumpet::Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate)
{
	float time = 0;
	float timeOffset = 1 / sampleRate;
	double fade_out = 1;
	double fade_in = 1;
	double total_samples = sampleRate * duration;
	buffer->reserve(static_cast<size_t>(total_samples));

	for (int i = 0; i < total_samples; i++)
	{
		double amp = env->GetAmplitude(time) * amplitude;
		double sample = amp *
			(1.0 * oscillator.GetValue(frequency, time, OscillatorTypes::SAW, 0.05, 1)
				+ 1 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 12), time, OscillatorTypes::SIN)
				+ .9 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 19), time, OscillatorTypes::SIN)
				+ .25 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 24), time, OscillatorTypes::SIN)
				+ 0.125 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 28), time, OscillatorTypes::SIN)
				+ 0.0625 * oscillator.GetValue(0, time, OscillatorTypes::RANDOM)
				+ 0.0625 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 31), time, OscillatorTypes::SIN))
			* volume * fade_out * fade_in;

		//normalize between 0-1
		sample /= 3.3375;

		time += timeOffset;
		if (buffer->size() < total_samples * 0.05)
		{
			double percent_through = buffer->size() / total_samples;
			fade_in = 20 * percent_through;
		}
		else if (buffer->size() > total_samples * 0.95)
		{
			//percentage through sample
			double percent_through = buffer->size() / total_samples;
			fade_out = (-20 * percent_through) + 20;
		}
		buffer->push_back(sample);
	}
}

void Flute::Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate)
{
	float time = 0;
	float timeOffset = 1 / sampleRate;
	double fade_out = 1;
	double fade_in = 1;
	double total_samples = sampleRate * duration;
	buffer->reserve(static_cast<size_t>(total_samples));

	for (int i = 0; i < total_samples; i++)
	{
		double amp = env->GetAmplitude(time) * amplitude;
		double sample = amp *
			(1.0 * oscillator.GetValue(frequency, time, OscillatorTypes::FMTRIANGLE, 0.05, 1)
				+ 0.0625 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 12), time, OscillatorTypes::SIN)
				+ .015 * oscillator.GetValue(Instrument::MoveSemitones(frequency, -5), time, OscillatorTypes::FMTRIANGLE, 0.05, 1)
				+ .25 * oscillator.GetValue(Instrument::MoveSemitones(frequency, 19), time, OscillatorTypes::FMTRIANGLE, 0.05, 1)
				+ 0.0625 * oscillator.GetValue(0, time, OscillatorTypes::RANDOM))
			* volume * fade_out * fade_in;

		//normalize between 0-1
		sample /= 1.39;

		time += timeOffset;
		if (buffer->size() < total_samples * 0.05)
		{
			double percent_through = buffer->size() / total_samples;
			fade_in = 20 * percent_through;
		}
		else if (buffer->size() > total_samples * 0.95)
		{
			//percentage through sample
			double percent_through = buffer->size() / total_samples;
			fade_out = (-20 * percent_through) + 20;
		}
		buffer->push_back(sample);
	}
}

void Drum::Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate)
{
	float time = 0;
	float timeOffset = 1 / sampleRate;
	double fade_out = 1;
	double fade_in = 1;
	double total_samples = sampleRate * duration;
	buffer->reserve(static_cast<size_t>(total_samples));

	for (int i = 0; i < total_samples; i++)
	{
		double amp = env->GetAmplitude(time) * amplitude;
		double sample = amp *
			(.01 * oscillator.GetValue(frequency, time, OscillatorTypes::RANDOM, 0.05, 1)
			+ 1.0 * oscillator.GetValue(55, time, OscillatorTypes::SQUARE))
			* volume * fade_out * fade_in;

		//normalize between 0-1
		/*sample /= 1.1;*/

		time += timeOffset;
		if (buffer->size() < total_samples * 0.05)
		{
			double percent_through = buffer->size() / total_samples;
			fade_in = 20 * percent_through;
		}
		else if (buffer->size() > total_samples * 0.95)
		{
			//percentage through sample
			double percent_through = buffer->size() / total_samples;
			fade_out = (-20 * percent_through) + 20;
		}
		buffer->push_back(sample);
	}
}

void Square::Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate)
{
	float time = 0;
	float timeOffset = 1 / sampleRate;
	double fade_out = 1;
	double fade_in = 1;
	double total_samples = sampleRate * duration;
	buffer->reserve(static_cast<size_t>(total_samples));

	for (int i = 0; i < total_samples; i++)
	{
		double amp = env->GetAmplitude(time) * amplitude;
		double sample = amp *
			(1 * oscillator.GetValue(frequency, time, OscillatorTypes::SQUARE))
			* volume * fade_out * fade_in;

		time += timeOffset;
		if (buffer->size() < total_samples * 0.05)
		{
			double percent_through = buffer->size() / total_samples;
			fade_in = 20 * percent_through;
		}
		else if (buffer->size() > total_samples * 0.95)
		{
			//percentage through sample
			double percent_through = buffer->size() / total_samples;
			fade_out = (-20 * percent_through) + 20;
		}
		buffer->push_back(sample);
	}
}

void Saw::Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate)
{
	float time = 0;
	float timeOffset = 1 / sampleRate;
	double fade_out = 1;
	double fade_in = 1;
	double total_samples = sampleRate * duration;
	buffer->reserve(static_cast<size_t>(total_samples));

	for (int i = 0; i < total_samples; i++)
	{
		double amp = env->GetAmplitude(time) * amplitude;
		double sample = amp *
			(1 * oscillator.GetValue(frequency, time, OscillatorTypes::SAW))
			* volume * fade_out * fade_in;

		time += timeOffset;
		if (buffer->size() < total_samples * 0.05)
		{
			double percent_through = buffer->size() / total_samples;
			fade_in = 20 * percent_through;
		}
		else if (buffer->size() > total_samples * 0.95)
		{
			//percentage through sample
			double percent_through = buffer->size() / total_samples;
			fade_out = (-20 * percent_through) + 20;
		}
		buffer->push_back(sample);
	}
}
