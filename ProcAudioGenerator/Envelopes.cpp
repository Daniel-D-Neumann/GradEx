#include "Envelopes.h"

ADSREnvelope::ADSREnvelope(double attackTime, double decayTime, double sustainAmplitude, double releaseTime, double startAmplitude)
{
	this->attackTime = attackTime;
	this->releaseTime = releaseTime;
	this->decayTime = decayTime;
	this->sustainAmplitude = sustainAmplitude;
	this->startAmplitude = startAmplitude;
}

double ADSREnvelope::GetAmplitude(double time, double timeOn, double timeOff)
{
	double amp = 0.0;
	double releaseAmplitude = 0.0;
	double lifeTime = timeOn - timeOff;

	if (timeOn > timeOff)
	{
		//ADS
		//Attack
		if (lifeTime <= attackTime)
		{
			amp = (lifeTime / attackTime) * startAmplitude;
		}
		else if (lifeTime > attackTime && lifeTime <= attackTime + decayTime)
		{
			amp = ((lifeTime - attackTime) / decayTime) * (sustainAmplitude - startAmplitude) + startAmplitude;
		}
		//if (lifeTime > attackTime + decayTime)
		else
		{
			amp = sustainAmplitude;
		}
	}
	else
	{
		if (lifeTime <= attackTime)
			releaseAmplitude = (lifeTime / attackTime) * startAmplitude;

		if (lifeTime > attackTime && lifeTime <= (attackTime + decayTime))
			releaseAmplitude = ((lifeTime - attackTime) / decayTime) * (sustainAmplitude - startAmplitude) + startAmplitude;

		if (lifeTime > (attackTime + decayTime))
			releaseAmplitude = sustainAmplitude;
		
		amp = ((time - timeOff) / releaseTime) * (0.0 - releaseAmplitude) + releaseAmplitude;
	}

	if (amp <= 0.0001) amp = 0.0;

	return amp;
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
	else if (time > attackTime && time <= attackTime + decayTime)
	{
		amp = ((time - attackTime) / decayTime) * (sustainAmplitude - startAmplitude) + startAmplitude;
	}
	else
	{
		amp = sustainAmplitude;
	}

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

double AREnvelope::GetAmplitude(double time, double timeOn, double timeOff)
{
	double amp = 0.0f;
	double lifeTime = time - timeOn;
	attackTime = 0.2;
	releaseTime = 0.4;

	if (timeOn > timeOff)
	{
		//Attack
		if (lifeTime < attackTime)
		{
			amp = (lifeTime / attackTime) * startAmplitude;
			endAmp = amp;
		}
		else
		{
			amp = ((time - (timeOn + attackTime)) / releaseTime) * (0.0 - startAmplitude) + startAmplitude;
		}
	}
	else
	{
		amp = ((time - (timeOn + attackTime)) / releaseTime) * (0.0 - endAmp) + endAmp;
	}

	if (amp <= 0.0001) amp = 0.0;
	
	return amp;
}
