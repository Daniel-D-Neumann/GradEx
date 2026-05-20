#include "MidiParser.h"

std::vector<int> MidiParser::Get_Relevant_Tracks(int instrument, float& seconds_per_tick)
{
    std::vector<int> relevant_tracks;

    int track_instrument = -1;
    int tracks = midi_file.getTrackCount();
    for (int track = 0; track < tracks; track++) {
        for (int event = 0; event < midi_file[track].size(); event++) {
            if (midi_file[track][event].isMetaMessage())
            {
                if (midi_file[track][event].isTempo())
                {
                    seconds_per_tick = midi_file[track][event].getTempoBPM();
                }
            }
            if (midi_file[track][event].isTimbre())
            {
                track_instrument = (int)midi_file[track][event][1];
                if (track_instrument >= 0 && track_instrument < 8) track_instrument = 0;
                else if (track_instrument >= 8 && track_instrument < 16) track_instrument = 1;
                else if (track_instrument >= 16 && track_instrument < 24) track_instrument = 2;
                else if (track_instrument >= 24 && track_instrument < 32) track_instrument = 3;
                else if (track_instrument >= 32 && track_instrument < 40) track_instrument = 4;
                else if (track_instrument >= 40 && track_instrument < 56) track_instrument = 5;
                else if (track_instrument >= 56 && track_instrument < 64) track_instrument = 6;
                else if (track_instrument >= 64 && track_instrument < 80) track_instrument = 7;
                else if (instrument == 80) instrument = 10;
                else if (instrument == 81 || instrument == 87) instrument = 11;
                else if (track_instrument >= 104 && track_instrument < 112) track_instrument = 8;
                else if (track_instrument >= 112 && track_instrument < 120) track_instrument = 9;
                if (track_instrument == instrument) relevant_tracks.push_back(track);
                break;
            }
        }
    }
    return relevant_tracks;
}

MidiParser::MidiParser()
{

}

Midi_Data MidiParser::ParseFile(std::string filename, int instrument)
{
	midi_file.read(filename);
	midi_file.doTimeAnalysis();
	midi_file.linkNotePairs();

	Midi_Data midi_data = Midi_Data();

	midi_data.ticks_per_quater_note = midi_file.getTicksPerQuarterNote();
	float tempo = 120;
	midi_data.seconds_per_tick = static_cast<float>(60) / (midi_data.ticks_per_quater_note * tempo);

	std::vector<int> relevant_tracks = Get_Relevant_Tracks(instrument, tempo);

	for (int track : relevant_tracks)
	{
		midi_data.track_events.push_back(std::move(Get_Midi_Events(track)));
	}

	return midi_data;
}

std::queue<Midi_Event> MidiParser::Get_Midi_Events(int track)
{
	std::queue<Midi_Event> midi_events;

	for (int event = 0; event < midi_file[track].size(); event++) {
		if (midi_file[track][event].isNoteOn())
		{
			Midi_Event event_to_store = Midi_Event();
			event_to_store.ticks_since_epoch = midi_file[track][event].tick;
			event_to_store.note_type.note_duration = midi_file[track][event].getTickDuration();
			event_to_store.note_type.note_type = ((int)midi_file[track][event][1] % (NUM_POSSIBLE_NOTES - 1)) + 1;
			event_to_store.note_type.note_octave = (int)midi_file[track][event][1] / (NUM_POSSIBLE_NOTES - 1);
			midi_events.push(event_to_store);
		}
	}

	return midi_events;
}
