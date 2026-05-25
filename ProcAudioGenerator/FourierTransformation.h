#pragma once
#include "Vector.h"
#include <cmath>
#include <numbers>
#include <vector>
#include <numeric>
#include <execution>

struct WaveData
{
	float frequency = 0;
	float amp = 0;
	float phase = 0;
};

struct Complex
{
	double X;
	double Y; //imaginary

	double Magnitude()
	{
		return sqrt(X * X + Y * Y);
	}
	double Angle()
	{
		return atan2(Y, X);
	}

	Complex()
	{
		X = 0;
		Y = 0;
	}

	Complex(double x, double y)
	{
		X = x;
		Y = y;
	}

	Complex operator+=(const Complex& other)
	{
		X += other.X;
		X += other.Y;
		return *this;
	}

	Complex operator-=(const Complex& other)
	{
		X -= other.X;
		X -= other.Y;
		return *this;
	}
};

static Complex operator+(const Complex& lhs, const Complex& rhs)
{
	return Complex(lhs.X + rhs.X, lhs.Y + rhs.Y);
}

static Complex operator-(const Complex& lhs, const Complex& rhs)
{
	return Complex(lhs.X - rhs.X, lhs.Y - rhs.Y);
}

static Complex operator*(const Complex& lhs, const Complex& rhs)
{
	return Complex(lhs.X * rhs.X - lhs.Y * rhs.Y, lhs.X * rhs.Y + lhs.Y * rhs.X);
}

static Complex CreateFromPolar(double angle, double magnitude)
{
	return Complex(cos(angle) * magnitude, sin(angle) * magnitude);
}

class FourierTransformation
{
public:

	const double TAU = 2 * std::numbers::pi;

	std::vector<WaveData> DFT(std::vector<float> signal, int sample_rate)
	{
		//Number of frequencies to test
		const float num_samples = static_cast<float>(signal.size());
		const int num_frequencies = static_cast<int>(signal.size()) / 2 + 1; //start at 0Hz

		std::vector<WaveData> result;
		result.resize(num_frequencies);

		const float frequency_step = sample_rate / static_cast<float>(num_samples); // 1 / duration

		for (int frequency_index = 0; frequency_index < num_samples; frequency_index++)
		{
			static int i = 0;
			Vec2 sample_total = std::transform_reduce(std::execution::par, signal.begin(), signal.end(), Vec2(),
				std::plus<>(),
				[this, num_samples, frequency_index](const float& sample)
				{
					float angle = (i++) / num_samples * static_cast<float>(TAU) * frequency_index;
					return Vec2(cosf(angle),sinf(angle)) * sample;
				});
			i = 0;

			Vec2 sample_center = sample_total / num_samples;

			bool is_0_Hz = frequency_index == 0;
			//last frequency is sample_rate/2 if sample count is even
			bool is_nyquist_frequency = frequency_index == num_frequencies - 1 && static_cast<int>(num_samples) % 2 == 0;
			float amp_scale = is_0_Hz || is_nyquist_frequency ? 1.0f : 2.0f;
			float amp = sample_center.Length() * amp_scale;

			float freq = frequency_index * frequency_step;
			float phase = -atan2f(sample_center.y, sample_center.x);
			result.at(frequency_index) = WaveData{
				.frequency = freq,
				.amp = amp,
				.phase = phase,
			};
		}

		return result;
	}

	std::vector<WaveData> FFT(std::vector<float> signal, int sample_rate)
	{
		const int num_samples = static_cast<int>(signal.size());
		const int length_pow_2 = NextPowerOfTwo(num_samples);
		signal.resize(length_pow_2);

		const double nyquist_freq = sample_rate / 2.0;

		std::vector<Complex> signal_complex;
		signal_complex.reserve(num_samples);
		for (int i = 0; i < num_samples; i++)
		{
			signal_complex[i] = Complex(signal[i], 0);
		}

		std::vector<Complex> result = FFTInternal(signal_complex);

		int num_frequencies = num_samples / 2 + 1;
		std::vector<WaveData> waves;
		waves.resize(num_frequencies);

		for (int i = 0; i < num_frequencies; i++)
		{
			bool is_0_hz = i == 0;
			bool is_nyqiust_frequency = i == num_frequencies - 1 && num_samples % 2 == 0;
			float amp_scale = is_0_hz || is_nyqiust_frequency ? 1.0f : 2.0f;

			double freq = i / (num_frequencies - 1.0) * nyquist_freq;
			waves[i] = WaveData(static_cast<float>(freq), static_cast<float>(result[i].Magnitude() / num_samples * amp_scale), static_cast<float>(-result[i].Angle()));
		}
	}

	std::vector<Complex> FFTInternal(const std::vector<Complex>& values)
	{
		int num_vals = static_cast<int>(values.size());
		if (num_vals == 1)
		{
			return values;
		}

		std::vector<Complex> evenSteps = FFTInternal(TakeEverySecond(values, true));
		std::vector<Complex> oddSteps = FFTInternal(TakeEverySecond(values, false));
		std::vector<Complex> results;
		results.resize(num_vals);

		double angle_increment = TAU / num_vals;

		for (int i = 0; i < num_vals; i++)
		{
			Complex c = CreateFromPolar(angle_increment * i, 1);
			Complex new_c = (oddSteps[i] * c);
			results[i] = evenSteps[i] + new_c;
			results[i + num_vals / 2] = evenSteps[i] - new_c;
		}

		return results;
	}

	std::vector<Complex> TakeEverySecond(const std::vector<Complex>& values, bool startOnFirst)
	{
		std::vector<Complex> result;
		result.reserve(values.size() / 2);
		int offset = startOnFirst ? 0 : 1;
		for (int i = 0; i < result.size(); i++)
		{
			result.push_back(values[i * 2 + offset]);
		}

		return result;
	}

	int NextPowerOfTwo(int num)
	{
		num -= 1;
		num |= num >> 16;
		num |= num >> 8;
		num |= num >> 4;
		num |= num >> 2;
		num |= num >> 1;
		return num + 1;
	}
};

