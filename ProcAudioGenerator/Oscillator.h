#pragma once
#include <iostream>

const double PI = 2.0 * acos(0.0);

enum OscillatorTypes
{
	SIN = 0,
	SQUARE,
	SAW,
	TRIANGLE,
	RANDOM,
	FMSIN,
	FMSQUARE,
	FMSAW,
	FMTRIANGLE,
};

class Oscillator
{
public:
	Oscillator() = default;

	double GetValue(double frequency, double deltaTime, OscillatorTypes type, double phase_shift = 0.0f, double LFOHertz = 0.0, double LFOAmplitude = 0.0);
	double MakeSin(double frequency, double deltaTime, double phase_shift = 0.0f);
private:
	double hertzToAngularVelocity(double hertz);
	
	double MakeFMSin(double frequency, double deltaTime, double phase_shift = 0.0f, double LFOHertz = 0.0, double LFOAmplitude = 0.0);
	
	double MakeSquare(double frequency, double deltaTime, double phase_shift = 0.0f);
	double MakeFMSquare(double frequency, double deltaTime, double phase_shift = 0.0f, double LFOHertz = 0.0, double LFOAmplitude = 0.0);
	
	double MakeSaw(double frequency, double deltaTime, double phase_shift = 0.0f);
	double MakeFMSaw(double frequency, double deltaTime, double phase_shift = 0.0f, double LFOHertz = 0.0, double LFOAmplitude = 0.0);
	
	double MakeTriangle(double frequency, double deltaTime, double phase_shift = 0.0f);
	double MakeFMTriangle(double frequency, double deltaTime, double phase_shift = 0.0f, double LFOHertz = 0.0, double LFOAmplitude = 0.0);
};

