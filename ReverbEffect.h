#pragma once
#include "Effect.h"
#include <vector>

class ReverbEffect : public Effect 
{
    public:
        ReverbEffect();
        ~ReverbEffect() override = default;

        float processSample(float inputSample) override;
        void processBlock(float* input, float* output, unsigned int nFrames) override;
        void setParameter(const std::string& name, float value) override;
        float getParameter(const std::string& name) const override;
        std::vector<std::string> getParameterNames() const override;
        std::pair<float, float> getParameterRange(const std::string& name) const override;

    private:
        static const int numCombs = 4;
        static const int numAllpass = 2;

        float combFeedback_ = 0.805f;
        float allpassFeedback_ = 0.7f;
        float wetLevel_ = 0.7f;
        float dryLevel_ = 0.3f;

        std::vector<std::vector<float>> combBuffers_;
        unsigned int combIndices_[numCombs] = {0};
        std::vector<std::vector<float>> allpassBuffers_;
        unsigned int allpassIndices_[numAllpass] = {0};

        double combTimes_[numCombs] = {0.0297, 0.0371, 0.0411, 0.0437};
        double allpassTimes_[numAllpass] = {0.005, 0.0017};

        void initializeBuffers();
}; 