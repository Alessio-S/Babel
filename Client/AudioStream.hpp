/*
** EPITECH PROJECT, 2024
** Babel
** File description:
** AudioStream
*/

#ifndef AUDIOSTREAM_HPP_
#define AUDIOSTREAM_HPP_

#include <portaudio.h>
#include <memory>
#include <stdexcept>
#include <functional>
#include <unistd.h>
#include <vector>

class AudioStream {
    public:
        AudioStream(std::function<void (AudioStream *, const float *, float *, unsigned long)> callback);
        ~AudioStream();
        void recordMicrophone();
        void stopRecord();
        std::vector<float> &getBuffer();
    protected:
    private:
        bool _recording;
        std::function<void (AudioStream *, const float *, float *, unsigned long)> _callback;
        PaStream *_stream;
        static PaStreamCallback callCallback;
        std::vector<float> _buffer;
};

#endif /* !AUDIOSTREAM_HPP_ */
