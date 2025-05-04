#pragma once
#include <string>
#include <vector>
#include <memory>
#include <RtAudio.h>

class Effect 
{
    public:
        Effect(const std::string& name);
        virtual ~Effect() = default;

        virtual float processSample(float inputSample) = 0;
        virtual void processBlock(float* input, float* output, unsigned int nFrames) = 0;

        virtual void setParameter(const std::string& name, float value) = 0;
        virtual float getParameter(const std::string& name) const = 0;
        virtual std::vector<std::string> getParameterNames() const = 0;
        virtual std::pair<float, float> getParameterRange(const std::string& name) const { return {0.0f, 1.0f}; }

        const std::string& getName() const { return name_; }
        bool isEnabled() const { return enabled_; }
        void setEnabled(bool enabled) { enabled_ = enabled; }

    protected:
        std::string name_;
        bool enabled_ = true;
}; 