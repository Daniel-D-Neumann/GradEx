#include "WriteToWav.h"
#include <string>

//PCM 16 bit
void WavWriter::WriteAudioToFile(std::vector<double> buffer)
{
	for (double sample : buffer)
	{
		int intSample = static_cast<int> (sample * maxAmplitude);
		writeToFile(intSample, 2);
	}

	//update how big the data chunk is
	postDataPos = static_cast<int>(file.tellp());
	std::cout << postDataPos - preDataPos << std::endl;
	int index = preDataPos - 4;
	file.seekp(index);
	writeToFile(postDataPos - preDataPos, 4);

	//update how big the file is
	file.seekp(4, std::ios::beg);
	writeToFile(postDataPos - 8, 4); //-8 due to header data

	file.seekp(postDataPos);
}

void WavWriter::writeToFile(int value, int size)
{
	file.write(reinterpret_cast<const char*> (&value), size);
}

void WavWriter::setupWavFile(const char* filename)
{
	maxAmplitude = static_cast<int>(std::pow(2, bitDepth - 1) - 1);

	//get file, if none found creates new one
	file.open(filename, std::ios::binary);

	//HEADER
	file << "RIFF";
	file << "----"; //Size of file (filled in when writing to file)
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
	file << "----"; //Size of data chunk (filled in when writing to file)
	preDataPos = static_cast<int>(file.tellp());
}

std::vector<double> WavWriter::ReadWavFile(std::string filepath)
{
	std::vector<double> signal;

	std::ifstream wav_file(filepath, std::ios::binary);

	if (wav_file.is_open())
	{
		//Read in all the relevant header data
		WavHeader wav_data{};
		wav_file.read(reinterpret_cast<char*>(&wav_data), sizeof(WavHeader));

		if (std::string(wav_data.wave, 4) != "WAVE" || std::string(wav_data.riff, 4) != "RIFF")
		{
			wav_file.close();
			std::cerr << "Not a WAVE or RIFF!" << std::endl;
			return signal;
		}
		
		//Read all data as bytes from data chunk
		std::vector<uint8_t> raw_data;
		raw_data.resize(wav_data.data_chunk_size);

		wav_file.read(reinterpret_cast<char*>(raw_data.data()), wav_data.data_chunk_size);
		
		int num_samples = wav_data.data_chunk_size / (wav_data.bits_per_sample / 8);
		signal.reserve(num_samples);

		if (wav_data.audio_format == WAV_FORMAT_PCM)
		{
			//used to normalise value between -1 and 1
			float norm_factor = 1.0f / (powf(2.0f,wav_data.bits_per_sample-1.0f)-1.0f);
			switch (wav_data.bits_per_sample)
			{
			case 8:
				for (int8_t i = 0; i < num_samples; i++)
				{
					//unsigned so -128 to get + & - vals
					int8_t sample = (raw_data[i] - 128);
					//convert to float for normalisation between 0 and 1
					float sample_float = static_cast<float>(sample);
					sample_float *= norm_factor;
					signal.push_back(sample_float);
				}
				break;
			case 16:
				for (int16_t i = 0; i < num_samples; i++)
				{
					//data twice as big so jump twice as far into data
					int index = i * 2;
					int16_t sample = *reinterpret_cast<int16_t*>(&raw_data[index]);
					float sample_float = static_cast<float>(sample);
					sample_float *= norm_factor;
					signal.push_back(sample_float);
				}
				break;
			case 24:
				for (int i = 0; i < num_samples; i++)
				{
					//No inherent 24 bit data type so shift the values into a 32 bit width
					int index_byte_1 = i * 3;
					int index_byte_2 = i * 3 + 1;
					int index_byte_3 = i * 3 + 2;
					int32_t sample = (raw_data[index_byte_1] | (raw_data[index_byte_2] << 8) | (raw_data[index_byte_3] << 16));
					//Then preserve the twos compliment bit
					if (sample & 0x800000) sample |= ~0xFFFFFF;
					float sample_float = static_cast<float>(sample);
					sample_float *= norm_factor;
					signal.push_back(sample_float);
				}
				break;
			case 32:
				for (int32_t i = 0; i < num_samples; i++)
				{
					int index = i * 4;
					int32_t sample = *reinterpret_cast<int32_t*>(&raw_data[index]);
					float sample_float = static_cast<float>(sample);
					sample_float *= norm_factor;
					signal.push_back(sample_float);
				}
				break;
			default:
				std::cerr << "Unsupported PCM bit depth of " << wav_data.bits_per_sample << std::endl;
				break;
			}
		}
		else if (wav_data.audio_format == WAV_FORMAT_IEEE_FLOAT && wav_data.bits_per_sample == 32)
		{
			for (int i = 0; i < num_samples; i++)
			{
				int index = i * 4;
				signal.push_back(*reinterpret_cast<float*>(&raw_data[index]));
			}
		}
		else
		{
			std::cerr << "Unsupported format of" << wav_data.audio_format << std::endl;
		}
		
		return signal;
	}
	else
	{
		std::cerr << "Couldn't open file" << std::endl;
	}

	return signal;
}


