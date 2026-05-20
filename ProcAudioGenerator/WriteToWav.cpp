#include "WriteToWav.h"


void WavWriter::WriteAudioToFile(std::vector<double> buffer)
{
	for (float sample : buffer)
	{
		int intSample = static_cast<int> (sample * maxAmplitude);
		writeToFile(intSample, 2);
	}

	//update how big the data is
	postDataPos = file.tellp();
	file.seekp(preDataPos);
	writeToFile(postDataPos - preDataPos, 4);

	file.seekp(4, std::ios::beg);
	writeToFile(postDataPos - 8, 4);

	file.seekp(postDataPos);
}

void WavWriter::writeToFile(int value, int size)
{
	file.write(reinterpret_cast<const char*> (&value), size);
}

void WavWriter::setupWavFile(const char* filename)
{
	maxAmplitude = std::pow(2, bitDepth - 1) - 1;

	//get file, if none found creates new one
	file.open(filename, std::ios::binary);

	//HEADER
	file << "RIFF";
	file << "----";
	file << "WAVE";

	//FORMAT
	file << "fmt ";
	writeToFile(16, 4); //Size
	writeToFile(1, 2); // Compression code
	writeToFile(1, 2); // Number of channels
	writeToFile(sampleRate, 4); // Sample rate
	writeToFile(sampleRate * bitDepth / 8, 4); // Byte rate
	writeToFile(bitDepth / 8, 2); // Block align
	writeToFile(bitDepth, 2); // Bit depth

	//DATA
	file << "data";
	file << "----";
	preDataPos = file.tellp();
}


