#include "Envelopes.h"

ADSREnvelope::ADSREnvelope(double attackTime, double decayTime, double sustainAmplitude, double releaseTime, double startAmplitude)
{
	this->attackTime = attackTime;
	this->releaseTime = releaseTime;
	this->decayTime = decayTime;
	this->sustainAmplitude = sustainAmplitude;
	this->startAmplitude = startAmplitude;
}

double ADSREnvelope::GetAmplitude(float time) const
{
	double amp = 0.0;
	//ADS
	//Attack
	if (time <= attackTime)
	{
		amp = (time / attackTime) * startAmplitude;
	}
	//Decay
	else if (time > attackTime && time <= attackTime + decayTime)
	{
		amp = ((time - attackTime) / decayTime) * (sustainAmplitude - startAmplitude) + startAmplitude;
	}
	//Sustain
	else
	{
		amp = sustainAmplitude;
	}

	//ensure divisions don't cause floating point issues
	if (amp <= 0.0001) amp = 0.0;

	return amp;
}

AREnvelope::AREnvelope(double attackTime, double releaseTime, double startAmplitude)
{
	this->attackTime = attackTime;
	this->releaseTime = releaseTime;
	this->startAmplitude = startAmplitude;
	endAmp = 0;
}
