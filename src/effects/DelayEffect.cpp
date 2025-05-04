#include "DelayEffect.h"
#include <QDebug>

DelayEffect::DelayEffect() : Effect("Delay"), delayTime_(0.5f), feedback_(0.5f), delaySamples_(0), delayWriteIndex_(0) 
{
    qDebug() << "DelayEffect: constructor";
    delaySamples_ = static_cast<unsigned int>(44100 * delayTime_);
    delayBuffer_.resize(delaySamples_, 0.0f);
}

float DelayEffect::processSample(float inputSample) 
{
    float delayedSample = delayBuffer_[delayWriteIndex_];
    float outputSample = inputSample + delayedSample * feedback_;

    delayBuffer_[delayWriteIndex_] = outputSample;
    delayWriteIndex_ = (delayWriteIndex_ + 1) % delaySamples_;

    return outputSample;
}

void DelayEffect::processBlock(float* input, float* output, unsigned int nFrames) 
{
    for (unsigned int i = 0; i < nFrames; ++i) 
    {
        output[i] = processSample(input[i]);
    }
}

void DelayEffect::setParameter(const std::string& name, float value) 
{
    if (name == "delayTime") 
    {
        delayTime_ = value;
        delaySamples_ = static_cast<unsigned int>(44100 * delayTime_);
        delayBuffer_.resize(delaySamples_, 0.0f);
        delayWriteIndex_ = 0;
    }
    else if (name == "feedback") 
    {
        feedback_ = value;
    }
}

float DelayEffect::getParameter(const std::string& name) const 
{
    if (name == "delayTime") return delayTime_;
    if (name == "feedback") return feedback_;
    return 0.0f;
}

std::vector<std::string> DelayEffect::getParameterNames() const 
{
    qDebug() << "DelayEffect: getParameterNames called";
    return {"delayTime", "feedback"};
} 