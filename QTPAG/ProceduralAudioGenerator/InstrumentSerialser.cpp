#include "InstrumentSerialser.h"
#include <fstream>

void InstrumentSerialser::ReadInstruments(const std::string& filePath, std::vector<std::vector<FrequencyBreakdown>>* instrument_vec)
{
	if (!std::filesystem::file_size(filePath)) return;

	std::ifstream file(filePath);
	if (!file.is_open()) return;
	
	json data = json::parse(file);

	auto& json_instruments = data;

	for (const auto& instrument : json_instruments)
	{
		std::vector<FrequencyBreakdown> freqs;

		for (const auto& frequency_data : instrument)
		{
			FrequencyBreakdown freq_brk;
			freq_brk.osc.is_FM = (frequency_data["osc"]["is_FM"]);
			freq_brk.osc.oscillator_type = frequency_data["osc"]["oscillator_type"].get<OscillatorTypes>();
			freq_brk.osc.LFO_hertz = frequency_data["osc"]["LFO_hertz"].get<double>();
			freq_brk.osc.LFO_amp = frequency_data["osc"]["LFO_amp"].get<double>();
			freq_brk.amp = frequency_data["amp"].get<double>();
			freq_brk.phase = frequency_data["phase"].get<double>();
			freq_brk.relative_semitones = frequency_data["relative_semitones"].get<double>();
			freqs.push_back(freq_brk);
		}
		instrument_vec->push_back(freqs);
	}

	file.close();
}

InstrumentSerialser::InstrumentSerialser()
{
	ReadInstruments(required_file_path, &instruments);
	ReadInstruments(custom_file_path, &custom_instruments);
}

FrequencyBreakdown InstrumentSerialser::WaveDataToFrequencyBreakdown(const WaveData& wave, const double base_frequency)
{
	FrequencyBreakdown breakdown;
	//Oscillator is always SIN with fourier transform
	breakdown.amp = wave.amp;
	//TODO maybe need to shift 90 to make cosine waves
	breakdown.phase = wave.phase;
	//frequency of the wave needs to be relevant to the base frequency
	//this means when the note is shifted all constituent frequencies shift with it

	//this isn't just a static frequency difference as pitch scaling is not linear
	breakdown.relative_semitones = std::log(wave.frequency / base_frequency) / std::log(semitone_exponent);

	return breakdown;
}

std::vector<FrequencyBreakdown>* InstrumentSerialser::SaveInstrument(const std::vector<FrequencyBreakdown>& constituent_frequencies)
{
	std::ofstream file(custom_file_path);
	json data;

	//push back newly created instrument
    custom_instruments.push_back(constituent_frequencies);

    //custom_instruments = { constituent_frequencies };

	data = custom_instruments;

	std::string s = data.dump(1);

	file << s;

    file.close();
	return &custom_instruments.back();
}

void InstrumentSerialser::ClearCustomInstruments()
{
    custom_instruments.clear();

    std::ofstream file(custom_file_path, std::ios::trunc);
    file.close();
}

std::vector<FrequencyBreakdown>* InstrumentSerialser::GetCustomInstrument(int index)
{
    return &custom_instruments.at(index);
}

std::vector<FrequencyBreakdown>* InstrumentSerialser::GetDefaultInstrument(int index)
{
    return & instruments.at(index);
}

int InstrumentSerialser::GetNumberOfCustomInstruments()
{
    return static_cast<int>(custom_instruments.size());
}
