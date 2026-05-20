#include "MarkovChain.h"

void MarkovChain::Read_File(std::string filename, File_Type file_type)
{
	if (file_type == TEXT) Read_Text_File(filename);
	else if (file_type == MIDI) Read_Midi_File(filename);
}

std::vector<State> MarkovChain::Choose_Initial_State() const
{
	std::vector<State> return_vec;
	int max_rand = static_cast<int>(state_starting_probabilities.size());
	if (!max_rand) return return_vec;
	return_vec = state_starting_probabilities.at(std::rand() % max_rand);
	return_vec.resize(NUM_POSSIBLE_SYNCHRONOUS_STATES);
	return return_vec;
}

std::vector<State> MarkovChain::Choose_Next_State(std::vector<State> states)
{
	std::vector<State> return_vec;
	int state_indexes[NUM_POSSIBLE_SYNCHRONOUS_STATES];
	for (int i = 0; i < NUM_POSSIBLE_SYNCHRONOUS_STATES; i++)
	{
		state_indexes[i] = State_To_Encoded_State(states.at(i));
	}
	int state_index = Combine_States(state_indexes);
	int max_rand = static_cast<int>(state_probabilities[state_index].size());
	if (max_rand == 0)
	{
		max_rand = static_cast<int>(state_starting_probabilities.size());
		return_vec = state_starting_probabilities.at(std::rand() % max_rand);
		return_vec.resize(10);
		return  return_vec;
	}
	return_vec = state_probabilities[state_index].at(std::rand() % max_rand);
	return_vec.resize(10);
	return return_vec;
}

int MarkovChain::Char_State_Factor_To_Int(char factor_char, int factor_index)
{
	int encoded_note = 0;

	for (int j = 0; j < Factor_Nums[factor_index]; j++)
	{
		if (*Factors[factor_index][j] == factor_char)
		{
			encoded_note = j;
			break;
		}
	}

	return encoded_note;
}

void MarkovChain::Read_Midi_File(std::string filename)
{
	std::vector<State> states;
	int state_indexes[NUM_POSSIBLE_SYNCHRONOUS_STATES];
	std::optional<int> previous_state;

	MidiParser parser = MidiParser();

	Midi_Data midi_data = parser.ParseFile(filename, instrument);
	time_per_tick = midi_data.seconds_per_tick;

	int num_tracks = static_cast<int>(midi_data.track_events.size());

	//how far through the track we are
	int current_tick = 0;

	std::vector<Midi_Event> current_events;

	int total_events = 0;

	for (int i = 0; i < num_tracks; i++)
	{
		total_events += static_cast<int>(midi_data.track_events[i].size());
	}

	for (int current_event = 0; current_event < total_events; )
	{
		states.clear();
		memset(state_indexes, 0, sizeof(state_indexes));
		Populate_Current_Events(midi_data, current_events, current_tick, num_tracks);
		int events_size = static_cast<int>(current_events.size());

		if (events_size > NUM_POSSIBLE_SYNCHRONOUS_STATES)
		{
			std::cout << "Not enough synchronisable states available: requested = " << events_size << std::endl;
			current_event += events_size;
			for (int i = 0; i < events_size; i++)
			{
				current_events[i].ticks_since_epoch = -1;
			}
			previous_state.reset();
			continue;
		}

		int smallest_ticks;
		bool additional_silence = false;

		//no notes playing at this tick count so it's silence
		if (!events_size)
		{
			//find our next smallest tick value to determine how long the silence lasts
			smallest_ticks = INT_MAX;
			for (int i = 0; i < num_tracks; i++)
			{
				if (midi_data.track_events[i].empty())continue;
				int tick_count = midi_data.track_events[i].front().ticks_since_epoch - current_tick;
				if (tick_count < smallest_ticks) smallest_ticks = tick_count;
			}
			int duration = smallest_ticks;

			//silence
			std::vector<int> state_factors = { 0,duration,0 };
			state_indexes[0] = Indexes_To_Encoded_State(state_factors);
			states.push_back(State(0, duration, 0));
		}
		else
		{

			//find our next smallest tick value
			smallest_ticks = current_events[0].note_type.note_duration;
			for (int i = 1; i < events_size; i++)
			{
				int tick_count = current_events[i].note_type.note_duration;
				if (tick_count < smallest_ticks) smallest_ticks = tick_count;
			}
			int next_tick_time = INT_MAX;
			for (int i = 0; i < num_tracks; i++)
			{
				if (midi_data.track_events[i].empty())continue;
				int tick_count = midi_data.track_events[i].front().ticks_since_epoch;
				if (tick_count < next_tick_time) next_tick_time = tick_count;
			}

			if (current_tick + smallest_ticks > next_tick_time)
			{
				Midi_Event event;
				event.note_type.note_duration = next_tick_time - current_tick;
				event.note_type.note_type = -1;
				event.ticks_since_epoch = current_tick;
				current_events.push_back(event);
				additional_silence = true;
			}

			for (int i = 0; i < events_size; i++)
			{
				int note;
				if (current_events[i].note_type.note_type == -1) note = 0;
				else note = current_events[i].note_type.note_type;
				int octave = current_events[i].note_type.note_octave;
				int duration = current_events[i].note_type.note_duration;

				std::vector<int> state_factors = { note, duration, octave };
				states.push_back(State(note, duration, octave));
				state_indexes[i] = Indexes_To_Encoded_State(state_factors);

				current_events[i].ticks_since_epoch = -1;
			}

		}

		int combined_state_index = Combine_States(state_indexes);

		if (previous_state.has_value())
		{
			state_probabilities[previous_state.value()].push_back(states);
		}
		state_starting_probabilities.push_back(states);
		previous_state = combined_state_index;

		current_event += events_size;
		if (!additional_silence) current_tick += smallest_ticks;
	}
}

void MarkovChain::Read_Text_File(std::string filename)
{
	std::fstream Song(filename, std::ios::in);
	std::string text;

	//skip over instrument details
	std::getline(Song, text, ',');

	int state_indexes[NUM_POSSIBLE_SYNCHRONOUS_STATES];
	std::vector<State> states;
	std::optional<int> previous_state;

	//read all states in the file
	while (std::getline(Song, text, ','))
	{
		memset(state_indexes, 0, sizeof(state_indexes));
		states.clear();

		//read first (doesn't start with a plus)
		std::string state;
		state.resize(NUM_FACTORS);
		for (int i = 0; i < NUM_FACTORS; i++)
		{
			state[i] = text[i];
		}

		std::vector<int> factors = { Char_State_Factor_To_Int(state[0],0), Char_State_Factor_To_Int(state[1],1), Char_State_Factor_To_Int(state[2],2) };
		text.erase(0, NUM_FACTORS);
		switch (factors[1])
		{
		case 1:
			factors[1] = 747;
			break;
		case 2:
			factors[1] = 374;
			break;
		case 4:
			factors[1] = 187;
			break;
		case 8:
			factors[1] = 93;
			break;
		case 6:
			factors[1] = 46;
			break;
		case 3:
			factors[1] = 23;
			break;
		default:
			break;
		}
		states.push_back(State(factors[0], factors[1], factors[2]));
		state_indexes[0] = String_To_Encoded_State(state);

		//read notes synchronous with the first
		for (int i = 1; i < NUM_POSSIBLE_SYNCHRONOUS_STATES; i++)
		{
			int pos = static_cast<int>(text.find('+'));
			if (pos == std::string::npos) break;
			for (int j = 0; j < NUM_FACTORS; j++)
			{
				state[j] = text[static_cast<size_t>(j + 1)];
			}
			std::vector<int> factors = { Char_State_Factor_To_Int(state[0],0), Char_State_Factor_To_Int(state[1],1), Char_State_Factor_To_Int(state[2],2) };
			text.erase(0, static_cast<size_t>(NUM_FACTORS + 1));
			switch (factors[1])
			{
			case 1:
				factors[1] = 747;
				break;
			case 2:
				factors[1] = 373;
				break;
			case 4:
				factors[1] = 187;
				break;
			case 8:
				factors[1] = 93;
				break;
			case 6:
				factors[1] = 46;
				break;
			case 3:
				factors[1] = 23;
				break;
			default:
				break;
			}
			states.push_back(State(factors[0], factors[1], factors[2]));

			state_indexes[i] = String_To_Encoded_State(state);
		}

		int state_index = Combine_States(state_indexes);
		
		//now we have the index for counting how many times this note has been played in the context of what the previous note was
		if (previous_state.has_value())
		{
			state_probabilities[previous_state.value()].push_back(states);
		}
		state_starting_probabilities.push_back(states);
		previous_state = state_index;
	}
}

int MarkovChain::String_To_Encoded_State(std::string string_state) const
{
	int encoded_state = 0;

	for (int i = 0; i < NUM_FACTORS; i++)
	{
		char currentFactor = string_state[i];

		for (int j = 0; j < Factor_Nums[i]; j++)
		{
			if (*Factors[i][j] == currentFactor)
			{
				encoded_state+= j * Factor_Muls[i];
				break;
			}
		}
	}
	
	return encoded_state;
}

int MarkovChain::Indexes_To_Encoded_State(const std::vector<int>& factors)
{
	int encoded_state = 0;

	for (int i = 0; i < NUM_FACTORS; i++)
	{
		int current_factor = factors.at(i);
		encoded_state += current_factor * Factor_Muls[i];
	}

	return encoded_state;
}


void MarkovChain::Populate_Current_Events(Midi_Data& midi_data, std::vector<Midi_Event>& current_events, int current_tick, int num_tracks)
{
	//clear used events
	for (int i = static_cast<int>(current_events.size()) - 1; i >= 0; i--)
	{
		if (current_events[i].ticks_since_epoch == -1)
		{
			current_events.erase(std::begin(current_events) + i);
			//midi_data.track_events[i].pop();
		}
	}

	//fill events
	for (int i = 0; i < num_tracks; i++)
	{
		if (midi_data.track_events[i].empty()) continue;
		while (midi_data.track_events[i].front().ticks_since_epoch == current_tick)
		{
			current_events.push_back(midi_data.track_events[i].front());
			midi_data.track_events[i].pop();
			if (midi_data.track_events[i].empty()) break;
		}
	}
}

int MarkovChain::State_To_Encoded_State(State state)
{
	int encoded_state = 0;
	for (int i = 0; i < NUM_FACTORS; i++)
	{
		int current_factor = *state[i];
		if (current_factor == -1) break;
		encoded_state += current_factor * Factor_Muls[i];
	}

	return encoded_state;
}

int MarkovChain::Combine_States(const int* states)
{
	//combine the found states to create an index for indexing and setting the previous state
	int num_synchronous_states = NUM_POSSIBLE_SYNCHRONOUS_STATES;
	for (int i = 0; i < NUM_POSSIBLE_SYNCHRONOUS_STATES; i++)
	{
		if (states[i] == 0)
		{
			if (i)num_synchronous_states = i;
			//silence at minimum duration will be 0 so catch
			else num_synchronous_states = 1;
			break;
		}
	}
	int state_index = ((num_synchronous_states - 1) * SYNCHRONOUS_STATE_JUMP) + states[(num_synchronous_states - 1)];

	return state_index;
}