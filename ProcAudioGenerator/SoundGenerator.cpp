#include "SoundGenerator.h"
#include "Timer.h"

SoundGenerator::SoundGenerator()
{
	Init_Instruments();
	wav_output = new WavWriter("BlankGeneratedWavFile.wav");
}

void SoundGenerator::Init_Instruments()
{
	std::vector<FrequencyBreakdown> freqs;
	ADSREnvelope env;

	//Piano
	env = ADSREnvelope(0.05, 0.35, .8, 0.05);
	freqs = {
		FrequencyBreakdown {
			.osc = OscillatorBreakdown {
				.is_FM = true,
				.oscillator_type = SIN,
				.LFO_hertz = 5.0,
				.LFO_amp = 0.001
			},
		},
		FrequencyBreakdown {
			.amp = 0.5,
			.relative_semitones = 12,
		},
		FrequencyBreakdown {
			.amp = 0.25,
			.relative_semitones = 19,
		},
		FrequencyBreakdown {
			.amp = 0.0625,
			.relative_semitones = 24,
		},
		FrequencyBreakdown {
			.amp = 0.0625,
			.relative_semitones = 28,
		},
		FrequencyBreakdown {
			.amp = 0.03125,
			.relative_semitones = 31,
		},
		FrequencyBreakdown {
			.amp = 0.125,
			.relative_semitones = 34,
		},
	};
	instruments[0] = Instrument(env, freqs);

	//Marimba
	env = ADSREnvelope(0.05, 0.3, 0.8, 0.05);
	freqs = {
		FrequencyBreakdown {
			.osc = OscillatorBreakdown {
				.is_FM = true,
				.oscillator_type = SIN,
				.LFO_hertz = 5.0,
				.LFO_amp = 0.001
			},
		},
		FrequencyBreakdown {
			.amp = 0.0625,
			.relative_semitones = 24,
		},
		FrequencyBreakdown {
			.amp = 0.9,
			.relative_semitones = 40,
		},
		FrequencyBreakdown {
			.amp = 0.015,
			.relative_semitones = 47,
		},
		FrequencyBreakdown {
			.amp = 0.0625,
			.relative_semitones = 51,
		},
		FrequencyBreakdown {
			.osc = OscillatorBreakdown {
				.oscillator_type = RANDOM
				},
			.amp = 0.02,
			.relative_semitones = 31,
		},
	};
	instruments[1] = Instrument(env, freqs);	

	//Harmonica
	env = ADSREnvelope(0.16, 0.33, 0.5, 0.14);
	freqs =
	{
		FrequencyBreakdown {
			.osc = OscillatorBreakdown{
				.is_FM = true,
				.oscillator_type = FMSIN,
				.LFO_hertz = 5.0,
				.LFO_amp = 0.001
			},
		},
		FrequencyBreakdown {
			.amp = 0.5f,
			.relative_semitones = 12
		},
		FrequencyBreakdown {
			.amp = 0.25f,
			.relative_semitones = 19
		},
		FrequencyBreakdown {
			.amp = 0.25f,
			.relative_semitones = 24
		},
		FrequencyBreakdown {
			.amp = 1.0f,
			.relative_semitones = 28
		},
		FrequencyBreakdown {
			.amp = 0.25f,
			.relative_semitones = 28
		},
		FrequencyBreakdown {
			.osc = OscillatorBreakdown {
				.oscillator_type = RANDOM
			},
			.amp = 0.02f,
			.relative_semitones = 12
		},
	};
	instruments[2] = Instrument(env, freqs);

	//AcousticGuitar
	env = ADSREnvelope(0.05, 0.67, 0.1, 0.67);
	freqs =
	{
		FrequencyBreakdown {
			.osc = OscillatorBreakdown{
				.is_FM = true,
				.oscillator_type = FMSIN,
				.LFO_hertz = 5.0,
				.LFO_amp = 0.001
			},
			.relative_semitones = 19
		},
		FrequencyBreakdown {
			.amp = 0.90,
			.relative_semitones = 12
		},
		FrequencyBreakdown {
			.amp = 0.0625,
			.relative_semitones = 7
		},
		FrequencyBreakdown {
			.amp = 0.0625,
		},
		FrequencyBreakdown {
			.amp = 0.0625,
			.relative_semitones = 24
		},
		FrequencyBreakdown {
			.amp = 0.5,
			.relative_semitones = 28
		},
		FrequencyBreakdown {
			.amp = 0.25,
			.relative_semitones = 31
		},
		FrequencyBreakdown {
			.amp = 0.0625,
			.relative_semitones = 34
		},
	};
	instruments[3] = Instrument(env, freqs);	

	//BassGuitar
	env = ADSREnvelope(0.05, 0.8, 0.1, 0.67);
	freqs =
	{
		FrequencyBreakdown {
			.osc = OscillatorBreakdown{
				.oscillator_type = SAW,
			},
		},
		FrequencyBreakdown {
			.amp = 0.50,
			.relative_semitones = 12
		},
		FrequencyBreakdown {
			.amp = 0.125,
			.relative_semitones = 19
		},
		FrequencyBreakdown {
			.amp = 0.25,
			.relative_semitones = 24
		},
		FrequencyBreakdown {
			.amp = 0.18,
			.relative_semitones = 28
		},
		FrequencyBreakdown {
			.amp = 0.0156,
			.relative_semitones = 31
		}
	};
	instruments[4] = Instrument(env,freqs);	

	//Violin
	env = ADSREnvelope(0.05, 0.67, 0.1, 0.67);
	freqs =
	{
		FrequencyBreakdown {
			.osc = OscillatorBreakdown{
				.oscillator_type = SAW,
			},
			.relative_semitones = 19,
		},
		FrequencyBreakdown {
			.osc = OscillatorBreakdown{
				.is_FM = true,
				.oscillator_type = FMSIN,
				.LFO_hertz = 5.0,
				.LFO_amp = 0.001,
			},
			.amp = 0.7,
			.relative_semitones = 12
		},
		FrequencyBreakdown {
			.amp = .25,
		},
		FrequencyBreakdown {
			.amp = .5,
			.relative_semitones = 24
		},
		FrequencyBreakdown {
			.amp = 0.25,
			.relative_semitones = 28
		},
		FrequencyBreakdown {
			.amp = 0.0125,
			.relative_semitones = 31
		}
	};
	instruments[5] = Instrument(env, freqs);

	//Trumpet
	env = ADSREnvelope(0.05, 1, 0.9, 0.67);
	freqs =
	{
		FrequencyBreakdown {
			.osc = OscillatorBreakdown{
				.oscillator_type = SAW,
			},
		},
		FrequencyBreakdown {
			.amp = 1,
			.relative_semitones = 12
		},
		FrequencyBreakdown {
			.amp = 0.9,
			.relative_semitones = 19
		},
		FrequencyBreakdown {
			.amp = 0.25,
			.relative_semitones = 24
		},
		FrequencyBreakdown {
			.amp = 0.125,
			.relative_semitones = 28
		},
		FrequencyBreakdown {
			.osc = OscillatorBreakdown {
				.oscillator_type = RANDOM
			},
			.amp = 0.0625,
		},
		FrequencyBreakdown {
			.amp = 0.0625,
			.relative_semitones = 31
		},
	};
	instruments[6] = Instrument(env, freqs);

	//Flute
	env = ADSREnvelope(0.05, 1, 0.9, 0.67);
	//TODO FIX
	freqs =
	{
		FrequencyBreakdown {
			.osc = OscillatorBreakdown{
				.is_FM = true,
				.oscillator_type = FMTRIANGLE,
				.LFO_hertz = 0.05,
				.LFO_amp = 1
			},
		},
		FrequencyBreakdown {
			.amp = 0.50,
			.relative_semitones = 12
		},
		FrequencyBreakdown {
			.amp = 0.125,
			.relative_semitones = 19
		},
		FrequencyBreakdown {
			.amp = 0.25,
			.relative_semitones = 24
		},
		FrequencyBreakdown {
			.amp = 0.18,
			.relative_semitones = 28
		},
		FrequencyBreakdown {
			.amp = 0.0156,
			.relative_semitones = 31
		}
	};
	instruments[7] = Instrument(env, freqs);

	//Banjo
	env = ADSREnvelope(0.05, 0.67, 0.1, 0.67);
	freqs =
	{
		FrequencyBreakdown {
			.osc = OscillatorBreakdown{
				.is_FM = true,
				.oscillator_type = FMSIN,
				.LFO_hertz = 5.0,
				.LFO_amp = 0.001,
			},
		},
		FrequencyBreakdown {
			.amp = 0.25,
			.relative_semitones = -12
		},
		FrequencyBreakdown {
			.amp = 0.5,
			.relative_semitones = 7
		},
		FrequencyBreakdown {
			.amp = 0.25,
			.relative_semitones = 12
		},
		FrequencyBreakdown {
			.amp = 0.0018,
			.relative_semitones = 16
		},
		FrequencyBreakdown {
			.amp = 0.0156,
			.relative_semitones = 19
		}
	};
	instruments[8] = Instrument(env, freqs);

	//Drum
	env = ADSREnvelope(0.05, 0.15, 0., 0.1);
	freqs =
	{
		FrequencyBreakdown {
			.osc = OscillatorBreakdown {
				.oscillator_type = RANDOM
			},
			.amp = .01
		},
		FrequencyBreakdown {
			.osc = OscillatorBreakdown {
				.oscillator_type = SQUARE
			},
			.amp = .01
		//TODO set frequency over semitones
		},
	};
	instruments[9] = Instrument(env, freqs);

	//Square
	env = ADSREnvelope(0.05, 0.15, 0.3, 0.1);
	//freqs = square

	//Saw
	env = ADSREnvelope(0.05, 0.15, 0.3, 0.1);
	//freqs = sin

	//Bell
	env = ADSREnvelope(0.01, 0, 0, 1.0);
	//freqs = ...

	/*for (int instrument = 0; instrument < NUM_AVAILABLE_INSTRUMENTS; instrument++)
	{
		markov_chains[instrument] = new MarkovChain();
		markov_chains[instrument]->instrument = instrument;
	}

	for (int i = 0; i < NUM_AVAILABLE_INSTRUMENTS * NUM_POSSIBLE_SYNCHRONOUS_STATES; i++)
	{
		note_buffers[i] = new std::vector<double>();
	}*/
}

SoundGenerator::~SoundGenerator()
{
	for (int i = 0; i < NUM_AVAILABLE_INSTRUMENTS; i++)
	{
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

bool SoundGenerator::Load_Music_File_Into_Generator(std::string filename)
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

void SoundGenerator::Load_Wav_File_Into_Generator(std::string filename)
{
	//loadedWavs.push_back();
	wav_output->WriteAudioToFile(wav_output->ReadWavFile(filename));

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
	instruments[instrument].Sound(note_buffers[buffer_index], duration, frequency, amplitude);
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
