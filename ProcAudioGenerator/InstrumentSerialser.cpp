#include "InstrumentSerialser.h"

void InstrumentSerialser::ReadInstruments(const std::string& filePath, std::vector<std::vector<FrequencyBreakdown>>* instrument_vec)
{
	std::ifstream file(required_file_path);
	json data = json::parse(file);

	auto& json_instruments = data["Instruments"];

	for (const auto& instrument : json_instruments)
	{
		std::vector<FrequencyBreakdown> freqs;

		for (const auto& frequency_data : instrument)
		{
			FrequencyBreakdown freq_brk;
			freq_brk.osc.is_FM = frequency_data["osc"]["is_FM"];
			freq_brk.osc.oscillator_type = frequency_data["osc"]["oscillator_type"];
			freq_brk.osc.LFO_hertz = frequency_data["osc"]["LFO_hertz"];
			freq_brk.osc.LFO_amp = frequency_data["osc"]["LFO_amp"];
			freq_brk.amp = frequency_data["amp"];
			freq_brk.phase = frequency_data["phase"];
			freq_brk.relative_semitones = frequency_data["relative_semitones"];
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

	data["Instruments"] = custom_instruments;



	return &instruments.back();
}
