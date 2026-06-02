#pragma once
#include "SoundGenerator.h"
#include "MidiParser.h"
#include "Timer.h"
#include "FourierTransformation.h"
#include "InstrumentSerialser.h"

int main()
{
	{
		SoundGenerator* soundGen = new SoundGenerator();

		//soundGen->Load_Wav_File_Into_Generator("InstrumentSamples/hello.wav");

		//double msf = soundGen->Generate_Instrument_From_Wav(0);

		//Timer timer = Timer("Program total time");
		////EXAMPLE OF HOW TO MANULLY CREATE MUSIC WITH THE PROGRAM
		//ADSREnvelope env = ADSREnvelope(0.05, 0.15, 0.8, 0.1);
		//std::vector<FrequencyBreakdown> freqs = soundGen->instrument_serialiser.custom_instruments[0];

		//Instrument* mc = new Instrument(env,freqs);
		//Instrument* silence = new Silence();
		//std::vector<double> harmC, harmA, harmD4, harmD5, harmE, harmG, sile;
		//mc->Sound(&harmC,1.f, msf,1.f,44100.f);
		//mc->Sound(&harmD4,0.2f,mc->MoveSemitones(msf,1),1.f,44100.f);
		//mc->Sound(&harmD5,0.2f, mc->MoveSemitones(msf, 13),1.f,44100.f);
		//mc->Sound(&harmA, 0.2f, mc->MoveSemitones(msf, 9), 1.f, 44100.f);
		//mc->Sound(&harmE, 1.f, mc->MoveSemitones(msf, 4), 1.f, 44100.f);
		//mc->Sound(&harmG, 1.f, mc->MoveSemitones(msf, 7), 1.f, 44100.f);
		//silence->Sound(&sile, 0.2f, 0.f, 0.f, 44100.f);

		//for (int i = 0; i<harmC.size(); i++)
		//{
		//	harmC[i] += harmE[i] + harmG[i];
		//}

		//WavWriter* output =  new WavWriter("MyWavFile.wav");
		//output->WriteAudioToFile(harmD4);
		//output->WriteAudioToFile(harmD4);
		//output->WriteAudioToFile(harmD5);
		//output->WriteAudioToFile(sile);
		//output->WriteAudioToFile(harmA);
		//output->WriteAudioToFile(sile);
		//output->WriteAudioToFile(harmC);

		//WHAT GENERATES THE SOUND
		

		//NEED TO LOAD A FILE TO BE ABLE TO GENERATE MUSIC
		//TEXT FILE WITH CUSTOM RTTTL FORMAT
		//soundGen->Load_Music_File_Into_Generator("MySong.txt");
		//MIDI FILE
		{
			Timer t1 = Timer("Read MIDI file");
			soundGen->Load_Music_File_Into_Generator("MidiSongs/MC.mid");
		
		}

		//soundGen->Load_Wav_File_Into_Generator("InstrumentSamples/MC_sample.wav");

		//soundGen->Generate_Instrument_From_Wav(0);

		soundGen->Generate_Music(10);

		//InstrumentSerialser IS;

	}
}