#pragma once
#include "Effect.h"
#include <cmath>

class OverdriveEffect : public Effect 
{
    public:
        OverdriveEffect();
        ~OverdriveEffect() override = default;

        float processSample(float inputSample) override;
        void processBlock(float* input, float* output, unsigned int nFrames) override;
        void setParameter(const std::string& name, float value) override;
        float getParameter(const std::string& name) const override;
        std::vector<std::string> getParameterNames() const override;
        std::pair<float, float> getParameterRange(const std::string& name) const override;

    private:
        float gain_ = 100.0f;      
        float level_ = 1.0f;      
        float threshold_ = 0.2f;  
        float mix_ = 1.0f;        
        float tone_ = 0.5f;  
        float lastLow_ = 0.0f;  

        float softClip(float x) const;
}; 