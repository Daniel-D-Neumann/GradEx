#pragma once
#include<nlohmann/json.hpp>
#include<fstream>
#include<string>
#include "Instrument.h"

using json = nlohmann::json;
using namespace nlohmann::literals;

const std::string required_file_path = "JsonInstruments/instruments.json";
const std::string custom_file_path = "JsonInstruments/custom_instruments.json";

class InstrumentSerialser
{
	void ReadInstruments(std::string filePath)
	{
		std::ifstream file(required_file_path);
		json data = json::parse(file);

		auto  json_instruments = data["Instruments"];

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
				freq_brk.relative_semitones = frequency_data["relative_semitones"];
				freqs.push_back(freq_brk);
			}
			instruments.push_back(freqs);
		}
	}

	std::vector<std::vector<FrequencyBreakdown>> instruments;
public:

	InstrumentSerialser()
	{
		ReadInstruments(required_file_path);
		ReadInstruments(custom_file_path);
	}

	std::vector<FrequencyBreakdown>* SaveInstrument(std::vector<FrequencyBreakdown>& constituent_frequencies)
	{




		//push back newly created instrument
		instruments.push_back(constituent_frequencies);

		return &instruments.back();
	}


};

