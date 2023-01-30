#include <Windows.h>
#include <iostream>
#include <fstream> 
#include <mmsystem.h>

int main() {
    // Open a waveform audio input device
    HWAVEIN hWaveIn;
    WAVEFORMATEX wfx;
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 1;
    wfx.nSamplesPerSec = 44100;
    wfx.wBitsPerSample = 16;
    wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) / 8;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.cbSize = 0;

    // Open the waveform audio input device
    waveInOpen(&hWaveIn, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    // Allocate memory for the audio buffer
    const int bufferSize = 10000;
    char* buffer = new char[bufferSize];

    // Prepare a wave header for the audio buffer
    WAVEHDR waveHeader;
    waveHeader.lpData = buffer;
    waveHeader.dwBufferLength = bufferSize;
    waveHeader.dwBytesRecorded = 0;
    waveHeader.dwUser = 0;
    waveHeader.dwFlags = 0;
    waveHeader.dwLoops = 0;
    waveHeader.lpNext = 0;
    waveHeader.reserved = 0;

    // Prepare the audio buffer for recording
    waveInPrepareHeader(hWaveIn, &waveHeader, sizeof(WAVEHDR));

    // Start recording
    std::cout << "Recording started" << std::endl;
    waveInAddBuffer(hWaveIn, &waveHeader, sizeof(WAVEHDR));
    waveInStart(hWaveIn);

    // Wait for the recording to finish
    Sleep(5000); // 5 seconds recording time

    // Stop recording
    waveInStop(hWaveIn);

    // Unprepare the audio buffer
    waveInUnprepareHeader(hWaveIn, &waveHeader, sizeof(WAVEHDR));

    // Save the recorded audio to a file
    std::ofstream file("recording.wav", std::ios::binary);
    file.write(buffer, bufferSize);
    file.close();

    std::cout << "Recording saved to recording.wav" << std::endl;

    // Clean up
    delete[] buffer;
    waveInClose(hWaveIn);

    return 0;
}
