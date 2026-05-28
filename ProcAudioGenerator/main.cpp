#pragma once
#include "SoundGenerator.h"
#include "MidiParser.h"
#include "Timer.h"
#include "FourierTransformation.h"
#include "InstrumentSerialser.h"

int main()
{
	{
		Timer timer = Timer("Program total time");
		//EXAMPLE OF HOW TO MANULLY CREATE MUSIC WITH THE PROGRAM
		/*ADSREnvelope env = ADSREnvelope(0.05, 0.67, 0.1, 0.67);
		std::vector<FrequencyBreakdown> freqs =
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
		Instrument* Marimba = new Instrument(env,freqs);
		Instrument* silence = new Silence();
		std::vector<double> harmC, harmA, harmD4, harmD5, harmE, harmG, sile;
		Marimba->Sound(&harmC,1.f,261.,0.2f,44100.f);
		Marimba->Sound(&harmD4,0.2f,293.66,0.2f,44100.f);
		Marimba->Sound(&harmD5,0.2f,587.33,0.2f,44100.f);
		Marimba->Sound(&harmA, 0.2f, 440., 0.2f, 44100.f);
		Marimba->Sound(&harmE, 1.f, 329., 0.2f, 44100.f);
		Marimba->Sound(&harmG, 1.f, 396., 0.2f, 44100.f);
		silence->Sound(&sile, 0.2f, 0.f, 0.f, 44100.f);

		for (int i = 0; i<harmC.size(); i++)
		{
			harmC[i] += harmE[i] + harmG[i];
		}

		WavWriter* output =  new WavWriter("MyWavFile.wav");
		output->WriteAudioToFile(harmD4);
		output->WriteAudioToFile(harmD4);
		output->WriteAudioToFile(harmD5);
		output->WriteAudioToFile(sile);
		output->WriteAudioToFile(harmA);
		output->WriteAudioToFile(sile);
		output->WriteAudioToFile(harmC);*/

		//WHAT GENERATES THE SOUND
		SoundGenerator* soundGen = new SoundGenerator();

		//NEED TO LOAD A FILE TO BE ABLE TO GENERATE MUSIC
		//TEXT FILE WITH CUSTOM RTTTL FORMAT
		//soundGen->Load_Music_File_Into_Generator("MySong.txt");
		//MIDI FILE
		/*{
			Timer t1 = Timer("Read MIDI file");
			soundGen->Load_Music_File_Into_Generator("MidiSongs/MC.mid");
		
		}
		soundGen->Generate_Music(10);*/

		soundGen->Load_Wav_File_Into_Generator("myWavFile.wav");

		FourierTransformation FT;
		InstrumentSerialser IS;

	}
}