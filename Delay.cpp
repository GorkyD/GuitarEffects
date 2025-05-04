#include <iostream>
#include <vector>
#include <RtAudio.h>

#define SAMPLE_RATE 44100
#define BUFFER_FRAMES 32

double delayTime = 0.5;
float feedback = 0.5f;

std::vector<float> delayBuffer;
unsigned int delayWriteIndex = 0;
unsigned int delaySamples;

int audioCallback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void* userData) 
{
   float* in = static_cast<float*>(inputBuffer);
   float* out = static_cast<float*>(outputBuffer);

   for (unsigned int i = 0; i < nBufferFrames; ++i) 
   {
      float inputSample = in[i];
      float delayedSample = delayBuffer[delayWriteIndex];
      float outputSample = inputSample + delayedSample * feedback;

      delayBuffer[delayWriteIndex] = outputSample;

      out[i] = outputSample;

      delayWriteIndex = (delayWriteIndex + 1) % delaySamples;
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

   delaySamples = static_cast<unsigned int>(SAMPLE_RATE * delayTime);
   delayBuffer.resize(delaySamples, 0.0f);

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

      std::cout << "Delay pedal is active. Press Enter to quit...\n";
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
