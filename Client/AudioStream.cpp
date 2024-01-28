/*
** EPITECH PROJECT, 2024
** Babel
** File description:
** AudioStream
*/

#include "AudioStream.hpp"
#include <iostream>

AudioStream::AudioStream(std::function<void (AudioStream *, const float *, float *, unsigned long)> callback) : _recording(false), _callback(callback), _stream(nullptr)
{
    PaError error =  Pa_Initialize();
    if (error != paNoError)
        throw std::runtime_error("Can't initialize audio stream");
    PaStreamParameters inputParameters;
    inputParameters.device = Pa_GetDefaultInputDevice();
    const PaDeviceInfo *inputInfo = Pa_GetDeviceInfo(inputParameters.device);
    inputParameters.channelCount = 1;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = inputInfo->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = nullptr;
    PaStreamParameters outputParameters;
    outputParameters.device = Pa_GetDefaultOutputDevice();
    const PaDeviceInfo *outputInfo = Pa_GetDeviceInfo(outputParameters.device);
    outputParameters.channelCount = 1;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = outputInfo->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;
    PaError result = Pa_OpenStream(&_stream, &inputParameters, &outputParameters, 44100, 0, paClipOff, callCallback, this);
    if (result != paNoError)
        throw std::runtime_error("Can't initialize audio stream");
}

void AudioStream::recordMicrophone()
{
    PaError result = Pa_StartStream(_stream);
    if (result != paNoError)
        throw std::runtime_error("Can't start audio stream");
    _recording = true;
}

void AudioStream::stopRecord()
{   
    PaError result = Pa_StopStream(_stream);
    if (result != paNoError)
        throw std::runtime_error("Can't stop audio stream");
    _recording = false;
}

AudioStream::~AudioStream()
{
    if (_stream) {
        if (_recording)
            stopRecord();
        Pa_CloseStream(_stream);
    }
    Pa_Terminate();
}

int AudioStream::callCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    AudioStream *audioStream = (AudioStream *) userData;
    audioStream->_callback(audioStream, (float *) input, (float *) output, frameCount);
    return paContinue;
}

std::vector<float> &AudioStream::getBuffer()
{
    return _buffer;
}

void callback(AudioStream *stream, const float *inputBuffer, float *outputBuffer, unsigned long frameCount)
{
    std::vector<float> &buffer = stream->getBuffer();
    for (unsigned long i = 0; i < frameCount; ++i)
        buffer.push_back(inputBuffer[i]);
    auto it = buffer.begin();
    for (unsigned long i = 0; i < frameCount && it != buffer.end(); ++i) {
        outputBuffer[i] = *it;
        it = buffer.erase(it);
    }
}
