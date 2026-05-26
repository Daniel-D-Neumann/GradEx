#include "Oscillator.h"

double Oscillator::hertzToAngularVelocity(double hertz)
{
	return hertz * 2.0 * PI;
}

double Oscillator::MakeFMSin(double frequency, double deltaTime, float phase_shift, double LFOHertz, double LFOAmplitude)
{
	return std::sin(hertzToAngularVelocity(frequency) * deltaTime + LFOAmplitude * frequency * MakeSin(LFOHertz, deltaTime, phase_shift));
}

double Oscillator::MakeSin(double frequency, double deltaTime, float phase_shift)
{
	return std::sin(hertzToAngularVelocity(frequency) * deltaTime + phase_shift);
}

double Oscillator::MakeSquare(double frequency, double deltaTime, float phase_shift)
{
	return MakeSin(frequency, deltaTime, phase_shift) > 0 ? 0.2 : -0.2;
}

double Oscillator::MakeFMSquare(double frequency, double deltaTime, float phase_shift, double LFOHertz, double LFOAmplitude)
{
	return MakeFMSin(frequency, deltaTime, phase_shift, LFOHertz, LFOAmplitude) > 0 ? 0.2 : -0.2;
}

double Oscillator::MakeTriangle(double frequency, double deltaTime, float phase_shift)
{
	return asin(MakeSin(frequency, deltaTime, phase_shift));
}

double Oscillator::MakeFMTriangle(double frequency, double deltaTime, float phase_shift, double LFOHertz, double LFOAmplitude)
{
	return asin(MakeFMSin(frequency,deltaTime, phase_shift,LFOHertz,LFOAmplitude));
}

//Not a true sin wave, but much faster computationally
double Oscillator::MakeSaw(double frequency, double deltaTime, float phase_shift)
{
	return (2.0 / PI) * (frequency * PI * fmod(deltaTime, 1.0 / frequency) - (PI / 2.0f));
}

double Oscillator::MakeFMSaw(double frequency, double deltaTime, float phase_shift, double LFOHertz, double LFOAmplitude)
{
	double newFreq = atan(tan(MakeFMSin(frequency, deltaTime, phase_shift, LFOHertz, LFOAmplitude)));
	return (2.0 / PI) * ((newFreq) * PI * fmod(deltaTime, 1.0 / (newFreq)) - (PI / 2.0f));
}

double Oscillator::GetValue(double frequency, double deltaTime, OscillatorTypes type, float phase_shift, double LFOHertz, double LFOAmplitude)
{
	switch (type)
	{
	case SIN:
		return MakeSin(frequency, deltaTime, phase_shift);
		break;
	case FMSIN:
		return MakeFMSin(frequency, deltaTime, LFOHertz, LFOAmplitude, phase_shift);
		break;
	case SQUARE:
		return MakeSquare(frequency, deltaTime, phase_shift);
		break;
	case FMSQUARE:
		return MakeFMSquare(frequency, deltaTime, LFOHertz, LFOAmplitude, phase_shift);
		break;
	case TRIANGLE:
		return MakeTriangle(frequency, deltaTime, phase_shift);
		break;
	case FMTRIANGLE:
		return MakeFMTriangle(frequency, deltaTime, LFOHertz, LFOAmplitude, phase_shift);
		break;
	case SAW:
		return MakeSaw(frequency, deltaTime, phase_shift);
		break;
	case FMSAW:
		return MakeFMSaw(frequency, deltaTime, LFOHertz, LFOAmplitude, phase_shift);
		break;
	case RANDOM:
		return /*2.0 **/ ((double)std::rand() / (double)RAND_MAX) - 1.0f;
		break;
	default:
		return 0.0;
		break;
	}
}