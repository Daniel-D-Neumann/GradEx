#pragma once
#include "MidiFile/MidiFile.h"
#include <string>
#include <queue>
#include "MarkovConsts.h"

struct Midi_Event
{
    State note_type;
    int ticks_since_epoch = -1;
};

struct Midi_Data
{
    std::vector<std::queue<Midi_Event>> track_events;
    int ticks_per_quater_note = -1;
    float seconds_per_tick = -1;
};

class MidiParser
{
private:

	smf::MidiFile midi_file;

    std::queue<Midi_Event> Get_Midi_Events(int track);

    std::vector<int> Get_Relevant_Tracks(int instrument, float& seconds_per_tick);

public:
    MidiParser();

    Midi_Data ParseFile(std::string filename, int instrument);
};

