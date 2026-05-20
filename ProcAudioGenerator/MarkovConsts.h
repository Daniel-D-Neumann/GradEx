#pragma once
#include <vector>

//state bit representation
//0000 0000 0000 0000 0000 0000 0000 1111 note type                0xF             shift 0
//0000 0000 0000 0000 0000 0000 0111 0000 note duration            0x70            shift 4
//0000 0000 0000 0000 0000 0000 0000 0000  
//0000 0000 0000 0000 0000 0111 1000 0000 note octave              0x780           shift 7

//combined state bit representation                
//0000 0000 0000 0000 0000 0000 0000 0011 1111 1111      0x3FF           shift 0
//0000 0000 0000 0000 0000 1111 1111 1100 0000 0000	     0xFFC00         shift 10
//0000 0000 0011 1111 1111 0000 0000 0000 0000 0000      0x3FF00000      shift 20
//1111 1111 1100 0000 0000 0000 0000 0000 0000 0000      0xFFC0000000    shift 30

const int NUM_POSSIBLE_NOTES = 13; //silence + C-B
const int NUM_POSSIBLE_DURATIONS = 5000;
const int NUM_POSSIBLE_OCTAVES = 9;
const int NUM_POSSIBLE_SYNCHRONOUS_STATES = 10;

const int NUM_FACTORS = 3;

struct State
{
public:
	short note_type;
	short note_duration;
	short note_octave;

	std::vector<State> next_states;

	short* operator[](const int index)
	{
		int i = index % NUM_FACTORS;
		switch (i)
		{
		case 0:
			return &note_type;
			break;
		case 1:
			return &note_duration;
			break;
		case 2:
			return &note_octave;
			break;
		default:
			break;
		}
		return nullptr;
	}

	bool operator==(const State& comparator)
	{
		if (comparator.note_type != note_type) return false;
		if (comparator.note_duration != note_duration) return false;
		if (comparator.note_octave != note_octave) return false;
		return true;
	}

	bool operator!=(const State& comparator)
	{
		if (comparator.note_type == note_type && comparator.note_duration == note_duration && comparator.note_octave == note_octave) return false;
		return true;
	}

	bool Default() const
	{
		if (note_type == 0 && note_duration == -1 && note_octave == 0) return true;
		return false;
	}

	bool Is_Silence() const
	{
		if (note_type == 0 && note_octave == 0) return true;
		else return false;
	}

	State()
	{
		note_type = 0;
		note_duration = -1;
		note_octave = 0;
	}
	State(short type, short duration, short octave) : note_type(type), note_duration(duration), note_octave(octave) {}
};

struct Combined_State
{
	State states[NUM_POSSIBLE_SYNCHRONOUS_STATES];
};

const int MAX_STATE_INDEX = NUM_POSSIBLE_NOTES * NUM_POSSIBLE_DURATIONS * NUM_POSSIBLE_OCTAVES * NUM_POSSIBLE_SYNCHRONOUS_STATES;
const int SYNCHRONOUS_STATE_JUMP = NUM_POSSIBLE_NOTES * NUM_POSSIBLE_DURATIONS * NUM_POSSIBLE_OCTAVES;

enum Note_Types
{
	S, C, Cs, D, Ds, E, F, Fs, G, Gs, A, As, B
};

enum Note_Duration
{
	DOUBLE, FULL, HALF, QUARTER, EIGHTH, SIXTEENTH, THIRTYSECOND
};

enum Note_Octaves
{
	ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT
};

static const char* Note_Types_Chars[NUM_POSSIBLE_NOTES] =
{
	"S", "C", "c", "D", "d", "E", "F", "f", "G", "g", "A", "a", "B"
};

static const char* Note_Durations_Chars[NUM_POSSIBLE_DURATIONS]
{
	"d", "1", "2", "4", "8", "6", "3"
};

static const char* Note_Octaves_Chars[NUM_POSSIBLE_OCTAVES]
{
	"0", "1", "2", "3", "4", "5", "6", "7", "8"
};

static const char** Factors[NUM_FACTORS] = { Note_Types_Chars, Note_Durations_Chars, Note_Octaves_Chars };
static const int Factor_Nums[NUM_FACTORS] = { NUM_POSSIBLE_NOTES, NUM_POSSIBLE_DURATIONS, NUM_POSSIBLE_OCTAVES };
static const int Factor_Muls[NUM_FACTORS] = { NUM_POSSIBLE_DURATIONS* NUM_POSSIBLE_OCTAVES, NUM_POSSIBLE_OCTAVES , 1 };