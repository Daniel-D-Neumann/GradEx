#pragma once
#include "Envelopes.h"
#include "Oscillator.h"
#include <vector>

static Oscillator oscillator = Oscillator();

// A basic note
struct Note
{
	int id;		// Position in scale
	double timeOn;	// Time note was activated
	double timeOff;	// Time note was deactivated
	bool active;
	//Instrument* instrument = nullptr;

	Note()
	{
		id = 0;
		timeOn = 0.0;
		timeOff = 0.0;
		active = false;
	}
};

class Instrument
{
protected:
	double volume = 1.0;

public:
	Instrument() { env = new ADSREnvelope(); }
	~Instrument() { delete env; env = nullptr; }
	Envelope* env = nullptr;
	double Scale(int noteID);
	double MoveSemitones(float curFreq, int numSemitones);
	virtual double sound(double time, Note note, bool& noteFinished) = 0;
	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate = 44100) {}
	//virtual void Sound(std::vector<double>* buffer, int note, float amplitude, float sampleRate = 44100) {}
};

class Bell : public Instrument
{
public:
	Bell();
	virtual double sound(double time, Note note, bool& noteFinished) override final;
};

class Harmonica : public Instrument
{
public:
	Harmonica() { env = new ADSREnvelope(0.16, 0.33, 0.5, 0.14); }
	virtual double sound(double time, Note note, bool& noteFinished) override final;

	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate) override final;
};

class Piano : public Instrument
{
public:
	Piano() { env = new ADSREnvelope(0.05,0.35,.8,0.05); }
	virtual double sound(double time, Note note, bool& noteFinished) override final { return 0; }
	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate) override final;
};

class Marimba : public Instrument
{
public:
	Marimba() { env = new ADSREnvelope(0.05, 0.3, 0.8, 0.05); }
	virtual double sound(double time, Note note, bool& noteFinished) override final { return 0; }
	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate) override final;
};

class AcousticGuitar : public Instrument
{
public:
	AcousticGuitar() { env = new ADSREnvelope(0.05, 0.67, 0.1, 0.67); }
	virtual double sound(double time, Note note, bool& noteFinished) override final { return 0; }
	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate) override final;
};

class BassGuitar : public Instrument
{
public:
	BassGuitar() { env = new ADSREnvelope(0.05, 0.8, 0.1, 0.67); }
	virtual double sound(double time, Note note, bool& noteFinished) override final { return 0; }
	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate) override final;
};

class Banjo : public Instrument
{
public:
	Banjo() { env = new ADSREnvelope(0.05, 0.67, 0.1, 0.67); }
	virtual double sound(double time, Note note, bool& noteFinished) override final { return 0; }
	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate) override final;
};

class Violin : public Instrument
{
public:
	Violin() { env = new ADSREnvelope(0.05, 0.67, 0.1, 0.67); }
	virtual double sound(double time, Note note, bool& noteFinished) override final { return 0; }
	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate) override final;
};

class Trumpet : public Instrument
{
public:
	Trumpet() { env = new ADSREnvelope(0.05, 1, 0.9, 0.67); }
	virtual double sound(double time, Note note, bool& noteFinished) override final { return 0; }
	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate) override final;
};

class Flute : public Instrument
{
public:
	Flute() { env = new ADSREnvelope(0.05, 1, 0.9, 0.67); }
	virtual double sound(double time, Note note, bool& noteFinished) override final { return 0; }
	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate) override final;
};

class Drum : public Instrument
{
public:
	Drum() { env = new ADSREnvelope(0.05, 0.15, 0., 0.1); }
	virtual double sound(double time, Note note, bool& noteFinished) override final { return 0; }
	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate) override final;
};

class Square : public Instrument
{
public:
	Square() { env = new ADSREnvelope(0.05, 0.15, 0.3, 0.1); }
	virtual double sound(double time, Note note, bool& noteFinished) override final { return 0; }
	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate) override final;
};

class Saw : public Instrument
{
public:
	Saw() { env = new ADSREnvelope(0.05, 0.15, 0.3, 0.1); }
	virtual double sound(double time, Note note, bool& noteFinished) override final { return 0; }
	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate) override final;
};

class MyInstrument : public Instrument
{
public:
	virtual double sound(double time, Note note, bool& noteFinished) override final;
};

class Silence : public Instrument
{
public:
	virtual double sound(double time, Note note, bool& noteFinished) override final { return 0; }

	virtual void Sound(std::vector<double>* buffer, double duration, float frequency, float amplitude, float sampleRate) override final;
};