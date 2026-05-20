#pragma once

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

class WavWriter{
public:
    WavWriter(const char* filename, int sampleRate = 44100, int bitDepth = 16) :sampleRate(sampleRate), bitDepth(bitDepth) { setupWavFile(filename); }
    WavWriter() = default;

    ~WavWriter() { file.close(); }
    void WriteAudioToFile(std::vector<double> buffer);
private:
    const int sampleRate;
    const int bitDepth;
    int maxAmplitude;

    std::ofstream file;

    int preDataPos;
    int postDataPos;

    void writeToFile(int value, int size);
    void setupWavFile(const char* filename);
};
