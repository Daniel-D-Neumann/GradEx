#include "SoundGenerator.h"
#include "Timer.h"

SoundGenerator::SoundGenerator()
{
	Init_Instruments();
	wav_output = new WavWriter("MyGeneratedWavFile.wav");
}

void SoundGenerator::Init_Instruments()
{
	instruments[0] = new Piano();	
	instruments[1] = new Marimba();	
	instruments[2] = new Harmonica();	
	instruments[3] = new AcousticGuitar();	
	instruments[4] = new BassGuitar();	
	instruments[5] = new Violin();	
	instruments[6] = new Trumpet();	
	instruments[7] = new Flute();	
	instruments[8] = new Banjo();	
	instruments[9] = new Drum();	

	for (int instrument = 0; instrument < NUM_AVAILABLE_INSTRUMENTS; instrument++)
	{
		markov_chains[instrument] = new MarkovChain();
		markov_chains[instrument]->instrument = instrument;
	}

	for (int i = 0; i < NUM_AVAILABLE_INSTRUMENTS * NUM_POSSIBLE_SYNCHRONOUS_STATES; i++)
	{
		note_buffers[i] = new std::vector<double>();
	}
}

SoundGenerator::~SoundGenerator()
{
	for (int i = 0; i < NUM_AVAILABLE_INSTRUMENTS; i++)
	{
		delete instruments[i];
		instruments[i] = nullptr;

		delete markov_chains[i];
		markov_chains[i] = nullptr;
	}

	delete wav_output;

	for (int i = 0; i < NUM_AVAILABLE_INSTRUMENTS * NUM_POSSIBLE_SYNCHRONOUS_STATES; i++)
	{
		delete note_buffers[i];
		note_buffers[i] = nullptr;
	}
}

bool SoundGenerator::Load_File_Into_Generator(std::string filename)
{
	File_Type file_type;
	if (filename.find(".txt") != std::string::npos) file_type = TEXT;
	else if (filename.find(".mid") != std::string::npos) file_type = MIDI;
	else
	{
		std::cout << "Cannot parse file of this format" << std::endl;
		return false;
	}

	std::fstream Song(filename, std::ios::in);

	if (!Song.is_open())
	{
		std::cout << "Couldn't open song" << std::endl;
		return false;
	}

	std::vector<int> unique_instruments;

	bool instruments_available =  Get_File_Instrument(filename, file_type, unique_instruments);

	if (!instruments_available || !unique_instruments.size())
	{
		std::cout << "Invalid instrument or not supported" << std::endl;
		return false;
	}

	{
		Timer t = Timer("Populate Markov Chains");
		for (int instrument : unique_instruments)
		{
			markov_chains[instrument]->Read_File(filename, file_type);
		}
	}

	return true;

}
bool SoundGenerator::Get_File_Instrument(std::string filename, File_Type file_type, std::vector<int>& instruments)
{
	if (file_type == TEXT)
	{
		std::string text;
		std::fstream file(filename, std::ios::in);
		std::getline(file, text, ',');
		int instrument = std::stoi(text);

		if (instrument >= 0 && instrument < NUM_AVAILABLE_INSTRUMENTS)
		{
			instruments.push_back(instrument);
			return true;
		}

		return false;
	}
	else if (file_type == MIDI)
	{
		int instrument = -1;
		smf::MidiFile midi_file;
		midi_file.read(filename);
		int tracks = midi_file.getTrackCount();
		for (int track = 0; track < tracks; track++) {
			for (int event = 0; event < midi_file[track].size(); event++) {
				if (midi_file[track][event].isTimbre())
				{
					instrument = (int)midi_file[track][event][1];
					if (instrument >= 0 && instrument < 8) instrument = PIANO;
					else if (instrument >= 8 && instrument < 16) instrument = MARIMBA;
					else if (instrument >= 16 && instrument < 24) instrument = HARMONICA;
					else if (instrument >= 24 && instrument < 32) instrument = ACOUSTIC_GUITAR;
					else if (instrument >= 32 && instrument < 40) instrument = BASS_GUITAR;
					else if (instrument >= 40 && instrument < 56) instrument = VIOLIN;
					else if (instrument >= 56 && instrument < 64) instrument = TRUMPET;
					else if (instrument >= 64 && instrument < 80) instrument = FLUTE;
					else if (instrument == 80) instrument = SYNTHSQUARE;
					else if (instrument == 81 || instrument == 87) instrument = SYNTHSAW;
					else if (instrument >= 104 && instrument < 112) instrument = BANJO;
					else if (instrument >= 112 && instrument < 120) instrument = DRUM;
					if (instrument >= 0 && instrument < NUM_AVAILABLE_INSTRUMENTS)
					{
						if(std::find(instruments.begin(), instruments.end(), instrument) == instruments.end()) instruments.push_back(instrument);
					}  
					else
					{
						std::cout << "Unsupported instrument: " << instrument << std::endl;
					}
					break;
				}
			}
		}

		if (instruments.size() == 0) return false;
		return true;
	}

	return false;
}

void SoundGenerator::Generate_Note(float frequency, double duration, float amplitude, Instruments instrument, int buffer_index)
{
	instruments[instrument]->Sound(note_buffers[buffer_index], duration, frequency, amplitude);
}

void SoundGenerator::Generate_Music(int length)
{
	//std::cout << "Generating Music" << std::endl;
	std::vector<State> instrument_states[NUM_AVAILABLE_INSTRUMENTS];
	for (int i = 0; i < NUM_AVAILABLE_INSTRUMENTS; i++)
	{
		instrument_states[i].resize(NUM_POSSIBLE_SYNCHRONOUS_STATES);
	}
	std::vector<int> instruments_to_play;
	for (int i = 0; i < NUM_AVAILABLE_INSTRUMENTS; i++)
	{
		if (markov_chains[i]->has_data())
		{
			instrument_states[i] = markov_chains[i]->Choose_Initial_State();
			instruments_to_play.push_back(i);
		}
	}

	for (int i = 0; i < length; i++)
	{
		for (int buffer = 0; buffer < NUM_AVAILABLE_INSTRUMENTS * NUM_POSSIBLE_SYNCHRONOUS_STATES; buffer++)
		{
			note_buffers[buffer]->clear();
		}

		for (int instrument : instruments_to_play)
		{
			//markov_chains[instrument]->Decode_State_Index(instrument_states[instrument], states);

			for (int k = 0; k < NUM_POSSIBLE_SYNCHRONOUS_STATES; k++)
			{
				State cur_state = instrument_states[instrument].at(k);
				///std::cout << states[k] << std::endl;
				if (cur_state.Default()) break;
				
				double duration = cur_state.note_duration * markov_chains[instrument]->time_per_tick;
				float note = Note_To_Frequency[(cur_state.note_type - 1) + ((NUM_POSSIBLE_NOTES - 1) * (cur_state.note_octave))];

				if (cur_state.Is_Silence())
				{
					if(k==0) 
						Generate_Note(0.f, duration, 0.f, static_cast<Instruments>(instrument), k);
				}
				else 
					Generate_Note(note, duration, 0.2f, static_cast<Instruments>(instrument), (instrument *NUM_POSSIBLE_SYNCHRONOUS_STATES)+k);
			}

		}

		std::vector<double> final_note;
		int largest = static_cast<int>(note_buffers[0]->size());
		double states_in_note = 0;
		//longest duration of a note
		for (int state = 1; state < NUM_AVAILABLE_INSTRUMENTS * NUM_POSSIBLE_SYNCHRONOUS_STATES; state++)
		{
			int size = static_cast<int>(note_buffers[state]->size());
			if (size)
			{
				states_in_note++;
				if (size > largest) largest = size;
			}
		}

		final_note.resize(largest);
		for (int state = 0; state < NUM_AVAILABLE_INSTRUMENTS * NUM_POSSIBLE_SYNCHRONOUS_STATES; state++)
		{
			for (int combinePoint = 0; combinePoint < note_buffers[state]->size(); combinePoint++)
			{
				final_note[combinePoint] += note_buffers[state]->at(combinePoint);
			}
		}

		double max_amp = states_in_note * 0.2f;
		if (max_amp > 1)
		{
			
			for (int note = 0; note < final_note.size(); note++)
			{
				final_note[note] /= max_amp;
				bool is_still_pos = (final_note[note] >= 0);
			}
			
		}
		

		wav_output->WriteAudioToFile(final_note);

		for (int instrument : instruments_to_play)
		{
			instrument_states[instrument] = markov_chains[instrument]->Choose_Next_State(instrument_states[instrument]);
		}
		
	}
}
