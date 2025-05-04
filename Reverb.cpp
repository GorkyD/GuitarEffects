#include <iostream>
#include <vector>
#include <RtAudio.h>

#define SAMPLE_RATE 44100
#define BUFFER_FRAMES 32

const int numCombs = 4;
double combTimes[numCombs] = {0.0297, 0.0371, 0.0411, 0.0437};
float combFeedback = 0.805f;

const int numAllpass = 2;
double allpassTimes[numAllpass] = {0.005, 0.0017};
float allpassFeedback = 0.7f;

std::vector<std::vector<float>> combBuffers(numCombs);
unsigned int combIndices[numCombs] = {0};

std::vector<std::vector<float>> allpassBuffers(numAllpass);
unsigned int allpassIndices[numAllpass] = {0};

float processSchroederReverb(float inputSample) 
{
    float combSum = 0.0f;

    for (int c = 0; c < numCombs; ++c) 
    {
        float delayed = combBuffers[c][combIndices[c]];
        float combOut = inputSample + delayed * combFeedback;
        combBuffers[c][combIndices[c]] = combOut;
        combSum += delayed;
        combIndices[c] = (combIndices[c] + 1) % combBuffers[c].size();
    }
    combSum /= numCombs;

    float apOut = combSum;
    for (int a = 0; a < numAllpass; ++a) 
    {
        float bufOut = allpassBuffers[a][allpassIndices[a]];
        float input = apOut + bufOut * allpassFeedback;
        allpassBuffers[a][allpassIndices[a]] = input;
        apOut = -input * allpassFeedback + bufOut;
        allpassIndices[a] = (allpassIndices[a] + 1) % allpassBuffers[a].size();
    }

    return inputSample * 0.3f + apOut * 0.7f;
}

int audioCallback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double, RtAudioStreamStatus, void*) 
{
    float* in = static_cast<float*>(inputBuffer);
    float* out = static_cast<float*>(outputBuffer);

    for (unsigned int i = 0; i < nBufferFrames; ++i) 
    {
        float inputSample = in[i];
        float reverbSample = processSchroederReverb(inputSample);
        out[i] = reverbSample;
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

    for (int c = 0; c < numCombs; ++c) 
    {
        unsigned int combLen = static_cast<unsigned int>(SAMPLE_RATE * combTimes[c]);
        combBuffers[c].resize(combLen, 0.0f);
        combIndices[c] = 0;
    }

    for (int a = 0; a < numAllpass; ++a) 
    {
        unsigned int apLen = static_cast<unsigned int>(SAMPLE_RATE * allpassTimes[a]);
        allpassBuffers[a].resize(apLen, 0.0f);
        allpassIndices[a] = 0;
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

        std::cout << "Schroeder Reverb is active. Press Enter to quit...\n";
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