#include "ReverbEffect.h"

ReverbEffect::ReverbEffect() : Effect("Reverb"), combFeedback_(0.805f), allpassFeedback_(0.7f), wetLevel_(0.7f), dryLevel_(0.3f)
{
    initializeBuffers();
}

void ReverbEffect::initializeBuffers() 
{
    combBuffers_.resize(numCombs);
    allpassBuffers_.resize(numAllpass);

    for (int c = 0; c < numCombs; ++c) 
    {
        unsigned int combLen = static_cast<unsigned int>(44100 * combTimes_[c]);
        combBuffers_[c].resize(combLen, 0.0f);
        combIndices_[c] = 0;
    }

    for (int a = 0; a < numAllpass; ++a) 
    {
        unsigned int apLen = static_cast<unsigned int>(44100 * allpassTimes_[a]);
        allpassBuffers_[a].resize(apLen, 0.0f);
        allpassIndices_[a] = 0;
    }
}

float ReverbEffect::processSample(float inputSample) 
{
    float combSum = 0.0f;

    for (int c = 0; c < numCombs; ++c) 
    {
        float delayed = combBuffers_[c][combIndices_[c]];
        float combOut = inputSample + delayed * combFeedback_;
        combBuffers_[c][combIndices_[c]] = combOut;
        combSum += delayed;
        combIndices_[c] = (combIndices_[c] + 1) % combBuffers_[c].size();
    }
    combSum /= numCombs;

    float apOut = combSum;
    for (int a = 0; a < numAllpass; ++a) 
    {
        float bufOut = allpassBuffers_[a][allpassIndices_[a]];
        float input = apOut + bufOut * allpassFeedback_;
        allpassBuffers_[a][allpassIndices_[a]] = input;
        apOut = -input * allpassFeedback_ + bufOut;
        allpassIndices_[a] = (allpassIndices_[a] + 1) % allpassBuffers_[a].size();
    }

    return inputSample * dryLevel_ + apOut * wetLevel_;
}

void ReverbEffect::processBlock(float* input, float* output, unsigned int nFrames) 
{
    for (unsigned int i = 0; i < nFrames; ++i) 
    {
        output[i] = processSample(input[i]);
    }
}

void ReverbEffect::setParameter(const std::string& name, float value) 
{
    if (name == "combFeedback") 
    {
        combFeedback_ = value;
    }
    else if (name == "allpassFeedback")
     {
        allpassFeedback_ = value;
    }
    else if (name == "wetLevel") 
    {
        wetLevel_ = value;
        dryLevel_ = 1.0f - value; 
    }
    else if (name == "roomSize") 
    {
        float scale = value;

        for (int c = 0; c < numCombs; ++c) 
        {
            combTimes_[c] *= scale;
        }

        for (int a = 0; a < numAllpass; ++a) 
        {
            allpassTimes_[a] *= scale;
        }

        initializeBuffers(); 
    }
}

float ReverbEffect::getParameter(const std::string& name) const 
{
    if (name == "combFeedback") return combFeedback_;
    if (name == "allpassFeedback") return allpassFeedback_;
    if (name == "wetLevel") return wetLevel_;
    if (name == "roomSize") return 1.0f; 
    return 0.0f;
}

std::vector<std::string> ReverbEffect::getParameterNames() const
{
    return {"combFeedback", "allpassFeedback", "wetLevel", "roomSize"};
}

std::pair<float, float> ReverbEffect::getParameterRange(const std::string& name) const 
{
    if (name == "combFeedback") return {0.0f, 0.95f};
    if (name == "allpassFeedback") return {0.0f, 0.95f};
    if (name == "wetLevel") return {0.0f, 1.0f};
    if (name == "roomSize") return {0.5f, 2.0f};
    return {0.0f, 1.0f};
} 