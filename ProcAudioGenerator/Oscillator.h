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

	double GetValue(double frequency, double deltaTime, OscillatorTypes type, double LFOHertz = 0.0, double LFOAmplitude = 0.0);
private:
	double hertzToAngularVelocity(double hertz);
	
	double MakeSin(double frequency, double deltaTime);
	double MakeFMSin(double frequency, double deltaTime, double LFOHertz = 0.0, double LFOAmplitude = 0.0);
	
	double MakeSquare(double frequency, double deltaTime);
	double MakeFMSquare(double frequency, double deltaTime, double LFOHertz = 0.0, double LFOAmplitude = 0.0);
	
	double MakeSaw(double frequency, double deltaTime);
	double MakeFMSaw(double frequency, double deltaTime, double LFOHertz = 0.0, double LFOAmplitude = 0.0);
	
	double MakeTriangle(double frequency, double deltaTime);
	double MakeFMTriangle(double frequency, double deltaTime, double LFOHertz = 0.0, double LFOAmplitude = 0.0);
};

