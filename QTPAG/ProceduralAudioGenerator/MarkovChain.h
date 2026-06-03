#pragma once
#include "MarkovConsts.h"
#include "MidiParser.h"
#include <fstream>
#include <string>
#include <iostream>
#include <optional>
#include <vector>

enum File_Type
{
	TEXT,
	MIDI
};


class MarkovChain
{
public:
	MarkovChain() 
	{ 
		srand(static_cast<unsigned int>(time(NULL))); 
	}
	~MarkovChain() {}

	void Read_File(std::string filename, File_Type file_type);

	int instrument = -1;
	double time_per_tick = 0.0026757;

	std::vector<State> Choose_Initial_State() const;

	std::vector<State> Choose_Next_State(std::vector<State> states);

	bool has_data() { return !state_starting_probabilities.empty(); }

private:
	
	std::vector<std::vector<State>> state_starting_probabilities = {};

	std::vector<std::vector<State>> state_probabilities[MAX_STATE_INDEX];

	void Read_Text_File(std::string filename);
	void Read_Midi_File(std::string filename);

	void Populate_Current_Events(Midi_Data& midi_data, std::vector<Midi_Event>& current_events, int current_tick, int num_tracks);

	//Handling between strings, states and state indexes
	int Char_State_Factor_To_Int(char factor_char, int factor_index);
	int String_To_Encoded_State(std::string string_state) const;
	int Indexes_To_Encoded_State(const std::vector<int>& factors);
	int State_To_Encoded_State(State state);
	int Combine_States(const int* states);



};

