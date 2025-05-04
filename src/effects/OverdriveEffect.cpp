#include "OverdriveEffect.h"

OverdriveEffect::OverdriveEffect() : Effect("Overdrive"), gain_(20.0f), level_(0.2f), threshold_(1.0f), mix_(1.0f), tone_(0.5f) {}

float OverdriveEffect::softClip(float x) const 
{
    float limit = threshold_;
    if (x > limit) return limit;
    if (x < -limit) return -limit;
    return x;
}

float OverdriveEffect::processSample(float inputSample) 
{
    float amplified = inputSample * gain_;
    float distorted = softClip(amplified);
    distorted = softClip(distorted);

    lastLow_ = 0.99f * lastLow_ + 0.01f * distorted;
    float low = lastLow_;
    float high = distorted - low;
    float eq = (1.0f - tone_) * low + tone_ * high + 0.5f * distorted;
    float clean = inputSample * (1.0f - mix_);
    float wet = eq * level_ * mix_;
    return clean + wet;
}

void OverdriveEffect::processBlock(float* input, float* output, unsigned int nFrames) 
{
    for (unsigned int i = 0; i < nFrames; ++i) 
    {
        output[i] = processSample(input[i]);
    }
}

void OverdriveEffect::setParameter(const std::string& name, float value) 
{
    if (name == "gain") 
    {
        gain_ = value;
    }
    else if (name == "level") 
    {
        level_ = value;
    }
    else if (name == "threshold") 
    {
        threshold_ = value;
    }
    else if (name == "mix") 
    {
        mix_ = value;
    }
    else if (name == "tone")
    {
        tone_ = value;
    }
}

float OverdriveEffect::getParameter(const std::string& name) const 
{
    if (name == "gain") return gain_;
    if (name == "level") return level_;
    if (name == "threshold") return threshold_;
    if (name == "mix") return mix_;
    if (name == "tone") return tone_;
    return 0.0f;
}

std::vector<std::string> OverdriveEffect::getParameterNames() const 
{
    return {"gain", "level", "threshold", "mix", "tone"};
}

std::pair<float, float> OverdriveEffect::getParameterRange(const std::string& name) const 
{
    if (name == "gain") return {0.0f, 500.0f};
    if (name == "level") return {0.0f, 1.0f};
    if (name == "threshold") return {0.0f, 1.0f};
    if (name == "mix") return {0.0f, 1.0f};
    if (name == "tone") return {0.0f, 1.0f};
    return {0.0f, 1.0f};
} 