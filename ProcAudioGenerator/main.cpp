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
		/*Instrument* Marimba = new Marimba();
		Instrument* silence = new Silence();
		std::vector<double> harmC, harmA, harmD4, harmD5, harmE, harmG, sile;
		harmonica->Sound(&harmC,1.f,261.,0.2f,44100.f);
		harmonica->Sound(&harmD4,0.2f,293.66,0.2f,44100.f);
		harmonica->Sound(&harmD5,0.2f,587.33,0.2f,44100.f);
		harmonica->Sound(&harmA, 0.2f, 440., 0.2f, 44100.f);
		harmonica->Sound(&harmE, 1.f, 329., 0.2f, 44100.f);
		harmonica->Sound(&harmG, 1.f, 396., 0.2f, 44100.f);
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
		//SoundGenerator* soundGen = new SoundGenerator();

		//NEED TO LOAD A FILE TO BE ABLE TO GENERATE MUSIC
		//TEXT FILE WITH CUSTOM RTTTL FORMAT
		//soundGen->Load_File_Into_Generator("MySong.txt");
		//MIDI FILE
		//{
		//	Timer t1 = Timer("Read MIDI file");
		//	soundGen->Load_File_Into_Generator("MidiSongs/MC.mid");
		//
		//}
		//soundGen->Generate_Music(1000);

		FourierTransformation FT;
		InstrumentSerialser IS;

	}
}