#pragma once

class Envelope
{
public:
	Envelope() = default;
	Envelope(double attackTime, double releaseTime, double startAmplitude = 1.0) : attackTime(attackTime), releaseTime(releaseTime), startAmplitude(startAmplitude) {};
	//Gets the amplitude given the current point in the notes lifecycle
	virtual double GetAmplitude(float time) const { return 0; }

protected:
	//amplitude the envelope climbs to
	double startAmplitude = 1.0;
	//how long it takes to reach that point
	double attackTime = 0.1;
	//how long before the sound completely cuts off
	double releaseTime = 0.2;
};

class ADSREnvelope : public Envelope
{
	//amplitude the envelope decays to after the attack
	double sustainAmplitude = 0.8;
	//how long it takes to decay to this amplitude
	double decayTime = 0.01;
public:
	ADSREnvelope() = default;
	ADSREnvelope(double attackTime, double decayTime, double sustainAmplitude, double releaseTime, double startAmplitude = 1.0);
	double GetAmplitude(float time) const override;
};

class AREnvelope : public Envelope
{
	double endAmp = 0.0;
public:
	AREnvelope(double attackTime, double releaseTime, double startAmplitude = 1.0);
};