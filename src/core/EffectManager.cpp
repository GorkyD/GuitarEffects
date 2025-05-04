#include "EffectManager.h"
#include <iostream>

EffectManager::EffectManager() 
{
    audio_ = std::make_unique<RtAudio>(RtAudio::WINDOWS_ASIO);
    if (audio_->getDeviceCount() < 1) 
    {
        throw std::runtime_error("No audio devices found!");
    }
}

EffectManager::~EffectManager() 
{
    if (isStreamRunning_) {
        stopStream();
    }
}

void EffectManager::addEffect(std::shared_ptr<Effect> effect) 
{
    effects_.push_back(effect);
}

void EffectManager::removeEffect(size_t index) 
{
    if (index < effects_.size()) 
    {
        effects_.erase(effects_.begin() + index);
    }
}

void EffectManager::clearEffects() 
{
    effects_.clear();
}

std::shared_ptr<Effect> EffectManager::getEffect(size_t index) 
{
    if (index < effects_.size()) 
    {
        return effects_[index];
    }
    return nullptr;
}

void EffectManager::processBlock(float* input, float* output, unsigned int nFrames) 
{
    if (effects_.empty()) 
    {
        std::copy(input, input + nFrames, output);
        return;
    }

    if (processingMode_ == ProcessingMode::Serial) 
    {
        std::vector<float> tempBuffer(nFrames);
        std::copy(input, input + nFrames, tempBuffer.data());

        for (auto& effect : effects_) {
            if (effect->isEnabled()) {
                effect->processBlock(tempBuffer.data(), output, nFrames);
                std::copy(output, output + nFrames, tempBuffer.data());
            }
        }
        std::copy(tempBuffer.data(), tempBuffer.data() + nFrames, output);
    } else 
    {
        std::fill(output, output + nFrames, 0.0f);
        std::vector<float> tempBuffer(nFrames);

        for (auto& effect : effects_) {
            if (effect->isEnabled()) 
            {
                effect->processBlock(input, tempBuffer.data(), nFrames);
                for (unsigned int i = 0; i < nFrames; ++i) 
                {
                    output[i] += tempBuffer[i];
                }
            }
        }
    }
}

int EffectManager::audioCallback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double streamTime,RtAudioStreamStatus status, void* userData) 
{
    auto* manager = static_cast<EffectManager*>(userData);
    float* in = static_cast<float*>(inputBuffer);
    float* out = static_cast<float*>(outputBuffer);

    std::vector<float> monoOut(nBufferFrames);
    manager->processBlock(in, monoOut.data(), nBufferFrames);
    for (unsigned int i = 0; i < nBufferFrames; ++i) 
    {
        out[2*i] = monoOut[i]; 
        out[2*i+1] = monoOut[i];
    }
    return 0;
}

bool EffectManager::startStream() 
{
    if (isStreamRunning_) return true;

    RtAudio::StreamParameters inputParams, outputParams;
    inputParams.deviceId = audio_->getDefaultInputDevice();
    inputParams.nChannels = 1;
    inputParams.firstChannel = 0;

    outputParams.deviceId = audio_->getDefaultOutputDevice();
    outputParams.nChannels = 2;
    outputParams.firstChannel = 0;

    unsigned int bufferFrames = 8; 

    try 
    {
        audio_->openStream(&outputParams, &inputParams, RTAUDIO_FLOAT32,
                         44100, &bufferFrames, &audioCallback, this);
        audio_->startStream();
        isStreamRunning_ = true;
        return true;
    }
    catch (const std::exception& e) 
    {
        std::cerr << "RtAudio error: " << e.what() << std::endl;
        return false;
    }
}

void EffectManager::stopStream() 
{
    if (!isStreamRunning_) return;

    try 
    {
        audio_->stopStream();
        audio_->closeStream();
        isStreamRunning_ = false;
    }
    catch (const std::exception& e) {
        std::cerr << "RtAudio error: " << e.what() << std::endl;
    }
}

void EffectManager::moveEffectUp(size_t index) 
{
    if (index > 0 && index < effects_.size()) {
        std::swap(effects_[index], effects_[index - 1]);
    }
}

void EffectManager::moveEffectDown(size_t index) 
{
    if (index + 1 < effects_.size()) {
        std::swap(effects_[index], effects_[index + 1]);
    }
} 