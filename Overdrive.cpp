#include <iostream>
#include <vector>
#include <cmath>
#include <RtAudio.h>

#define SAMPLE_RATE 44100
#define BUFFER_FRAMES 32

float gain = 20.0f;
float level = 0.2f;

float processHiGainDistortion(float inputSample) 
{
    float x = inputSample * gain;

    if (x > 1.0f) x = 2.0f / 3.0f;
    else if (x < -1.0f) x = -2.0f / 3.0f;
    else x = x - (x * x * x) / 3.0f;

    return x * level;
}

int audioCallback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double, RtAudioStreamStatus, void*) 
{
    float* in = static_cast<float*>(inputBuffer);
    float* out = static_cast<float*>(outputBuffer);

    for (unsigned int i = 0; i < nBufferFrames; ++i) 
    {
        float inputSample = in[i];
        float distortedSample = processHiGainDistortion(inputSample);
        out[i] = distortedSample;
    }
    return 0;
}

int main() 
{
    RtAudio audio;

    if (audio.getDeviceCount() < 1) 
    {
        std::cerr << "Error: no available audio-devices.\n";
        return 1;
    }

    RtAudio::StreamParameters inputParams, outputParams;
    inputParams.deviceId = audio.getDefaultInputDevice();
    inputParams.nChannels = 1;
    inputParams.firstChannel = 0;

    outputParams.deviceId = audio.getDefaultOutputDevice();
    outputParams.nChannels = 1;
    outputParams.firstChannel = 0;

    unsigned int bufferFrames = BUFFER_FRAMES;

    try 
    {
        audio.openStream(&outputParams, &inputParams, RTAUDIO_FLOAT32,
            SAMPLE_RATE, &bufferFrames, &audioCallback, nullptr);
        audio.startStream();

        std::cout << "Hi-Gain Distortion is active. Press Enter to quit...\n";
        std::cin.get();

        audio.stopStream();
        audio.closeStream();
    }
    catch (const std::exception& e) 
    {
        std::cerr << "RtAudio error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}