#pragma once
#include "Effect.h"
#include <vector>

class DelayEffect : public Effect 
{
    public:
        DelayEffect();
        ~DelayEffect() override = default;
        
        float processSample(float inputSample) override;
        void processBlock(float* input, float* output, unsigned int nFrames) override;
        void setParameter(const std::string& name, float value) override;
        float getParameter(const std::string& name) const override;
        std::vector<std::string> getParameterNames() const override;
        std::pair<float, float> getParameterRange(const std::string& name) const override {
            if (name == "delayTime") return {0.01f, 2.0f};
            if (name == "feedback") return {0.0f, 0.95f};
            return {0.0f, 1.0f};
        }

    private:
        float delayTime_ = 0.5f;
        float feedback_ = 0.5f;
        unsigned int delaySamples_ = 0;
        unsigned int delayWriteIndex_ = 0;
        std::vector<float> delayBuffer_;
}; 