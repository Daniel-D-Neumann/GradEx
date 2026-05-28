#pragma once

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

struct WavHeader
{
    char riff[4];
    uint32_t chunk_size;
    char wave[4];

    char fmt_chunk_ID[4];
    uint32_t fmt_chunk_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;

    char data_chunk_ID[4];
    uint32_t data_chunk_size;
};

class WavWriter{
public:
    WavWriter(const char* filename, int sampleRate = 44100, int bitDepth = 16) :sampleRate(sampleRate), bitDepth(bitDepth) { setupWavFile(filename); }

    ~WavWriter() { file.close(); }
    void WriteAudioToFile(std::vector<double> buffer);
    std::vector<double> ReadWavFile(std::string filepath);

private:
    const int sampleRate;
    const int bitDepth;
    int maxAmplitude;

    std::ofstream file;

    int preDataPos;
    int postDataPos;

    void writeToFile(int value, int size);
    void setupWavFile(const char* filename);

    const unsigned int WAV_FORMAT_PCM = 0x0001;
    const unsigned int WAV_FORMAT_IEEE_FLOAT = 0x0003;


};
