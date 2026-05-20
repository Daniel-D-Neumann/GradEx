#pragma once

class Envelope
{
public:
	Envelope() = default;
	Envelope(double attackTime, double releaseTime, double startAmplitude = 1.0) : attackTime(attackTime), releaseTime(releaseTime), startAmplitude(startAmplitude) {};
	virtual double GetAmplitude(double time, double timeOn, double timeOff) = 0;
	virtual double GetAmplitude(float time) const { return 0; }

protected:
	double startAmplitude = 1.0;
	double attackTime = 0.1;
	double releaseTime = 0.2;
};

class ADSREnvelope : public Envelope
{
	double decayTime = 0.01;
	double sustainAmplitude = 0.8;
public:
	ADSREnvelope() = default;
	ADSREnvelope(double attackTime, double decayTime, double sustainAmplitude, double releaseTime, double startAmplitude = 1.0);
	double GetAmplitude(double time, double timeOn, double timeOff) override;
	double GetAmplitude(float time) const override;
};

class AREnvelope : public Envelope
{
	double endAmp = 0.0;
public:
	AREnvelope(double attackTime, double releaseTime, double startAmplitude = 1.0);
	double GetAmplitude(double time, double timeOn, double timeOff) override;
};