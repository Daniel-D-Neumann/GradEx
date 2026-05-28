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
	void ReadInstruments(const std::string& filePath, std::vector<std::vector<FrequencyBreakdown>>* instrument_vec);
	std::vector<std::vector<FrequencyBreakdown>> instruments;
	std::vector<std::vector<FrequencyBreakdown>> custom_instruments;
public:

	InstrumentSerialser();

	FrequencyBreakdown WaveDataToFrequencyBreakdown(const WaveData& wave, const double base_frequency);

	std::vector<FrequencyBreakdown>* SaveInstrument(const std::vector<FrequencyBreakdown>& constituent_frequencies);


};

