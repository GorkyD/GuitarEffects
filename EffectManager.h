#pragma once
#include "Effect.h"
#include <vector>
#include <memory>
#include <RtAudio.h>

class EffectManager 
{
    public:
        EffectManager();
        ~EffectManager();

        void addEffect(std::shared_ptr<Effect> effect);
        void removeEffect(size_t index);
        void clearEffects();
        std::shared_ptr<Effect> getEffect(size_t index);
        size_t getEffectCount() const { return effects_.size(); }


        enum class ProcessingMode 
        {
            Serial, 
            Parallel 
        };

        void setProcessingMode(ProcessingMode mode) { processingMode_ = mode; }
        ProcessingMode getProcessingMode() const { return processingMode_; }


        void processBlock(float* input, float* output, unsigned int nFrames);

        static int audioCallback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void* userData);
        
        bool startStream();
        void stopStream();

        void moveEffectUp(size_t index);
        void moveEffectDown(size_t index);

    private:
        std::vector<std::shared_ptr<Effect>> effects_;
        ProcessingMode processingMode_ = ProcessingMode::Serial;
        std::unique_ptr<RtAudio> audio_;
        bool isStreamRunning_ = false;
}; 