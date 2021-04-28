# This script's purpose is to produce discrete smaples of a waveform generated
# by summing sine waves. This will help verify that FFT's are working correctly
# because we will know ahead of time the frequency and amplitude that compose
# the waveform being analyzed.
#
# Author: Kevin Imlay
# Date: 3-9-21
#
# Help from: https://numpy.org/doc/stable/reference/generated/numpy.sin.html


# Get the value of the sinusoid at the given time for the specified frequency,
# phase shift, and amplitude
import numpy as np
import matplotlib.pylab as plt

class Sinusoid:
    def __init__(self, freq=1, amp=1, phase=0, sTime=0, eTime=1):
        self.frequency = freq
        self.amplitude = amp
        self.phaseShift = phase
        self.startTime = sTime
        self.endTime = eTime

def getSinusoidVal(frequency, amplitude, phaseShift, time):
    return amplitude * np.sin(time * frequency * 2 * np.pi \
        - phaseShift * 2 * np.pi)


# Get discrete samples along the sinusoid produced from the specified frequency,
# phase shift, and amplitude. Samples are take for the number of seconds
# specified and at the rate specified.
def getSinusoid(frequency, amplitude, phaseShift, numSeconds, \
    samplesPerSeconds):
    timeList = list()
    samplesList = list()

    # generate time list for getting the sinusoid value at the sampling times
    for time in range(int(numSeconds*samplesPerSeconds)):
        timeList.append(time/samplesPerSeconds)

    # get sinusoid and return
    for sampleTime in timeList:
        samplesList.append(getSinusoidVal(frequency, amplitude,  phaseShift, \
            sampleTime))

    # return samples
    return samplesList


# Get the sum of the sinusoids given as a waveform.
def generateWaveform(sinusoidList, length, sampleRate):
    waveformList = list()

    # generate waveforms
    for wave in sinusoidList:
        # get attributes
        frequency = wave.frequency
        amplitude = wave.amplitude
        phaseShift = wave.phaseShift
        startTime = wave.startTime
        endTime = wave.endTime

        # get samples
        sinusoid = getSinusoid(frequency, amplitude,  phaseShift, length, \
            sampleRate)

        # zero samples out of time range
        sinusoidTimeFiltered = list()
        for sample in range(len(sinusoid)):
            # if out of time range, set to 0.0
            if 1 / sampleRate * sample < startTime \
                or 1 / sampleRate * sample > endTime:
                sinusoidTimeFiltered.append(0.0)
            else:
                sinusoidTimeFiltered.append(sinusoid[sample])

        # add to list of waveforms for summing later
        waveformList.append(sinusoidTimeFiltered)

    # sum waveforms
    summedWaveforms = [0.0] * int(sampleRate * length)
    for waveform in waveformList:
        for sampleIndex in range(len(summedWaveforms)):
            summedWaveforms[sampleIndex] += waveform[sampleIndex]

    # return summed sinewaves as waveform
    return summedWaveforms


# Run program
if __name__=="__main__":
    # define settings
    SAMPLES_PER_SECONDS = 11025
    NUM_SECONDS = 4

    # create sinusoids to sum for waveform
    # frequency in Hz, phase shift in decimal seconds, times in decimal seconds
    # Note: amplitude will be divided into 2^8 bins for converting to PCM format
    # so anything above 1.0 will be cut off at 127 and anything below -1.0 will
    # be cut off at -128. This is realistic to how we will be sending 8 bits
    # converted from 16 bit samples
    sinusoids = [ \
        Sinusoid(freq=392.00, amp=0.5, phase=0.0, sTime=0.0, eTime=4.0),
        #Sinusoid(freq=440.00, amp=1, phase=0.0, sTime=0.5, eTime=1.0),
        #Sinusoid(freq=493.88, amp=1, phase=0.0, sTime=1.0, eTime=1.5),
        #Sinusoid(freq=523.25, amp=1, phase=0.0, sTime=1.5, eTime=2.0),
        #Sinusoid(freq=587.33, amp=1, phase=0.0, sTime=2.0, eTime=2.5),
        #Sinusoid(freq=659.25, amp=1, phase=0.0, sTime=2.5, eTime=3.0),
        #Sinusoid(freq=739.99, amp=1, phase=0.0, sTime=3.0, eTime=3.5),
        Sinusoid(freq=2*392.00, amp=0.5, phase=0.0, sTime=0.0, eTime=4.0),
        ]

    # get waveform
    waveform = generateWaveform(sinusoids, NUM_SECONDS, \
        SAMPLES_PER_SECONDS)

    # save waveform in the PCM format we will be dealing with
    # (8 bit signed integer)
    # this one is unscaled to make it plot nice
    cutoffData = list()
    for sampleIndex in range(len(waveform)):
        # if negative
        if waveform[sampleIndex] < 0.0:
            cutoffData.append(np.maximum(waveform[sampleIndex], -1))
        # else, is positive
        else:
            cutoffData.append(np.minimum(waveform[sampleIndex], 1))

    # now scaled and byte form
    pcmData = list()
    count = 0
    for sample in cutoffData:
        #if count > 11025:
        #    break
        #count = count + 1
        # if negative
        if sample < 0.0:
            pcmData.append(min(int((-sample)*128)+128, 255))
        # else, is positive
        else:
            pcmData.append(min(int(sample*128), 127))
    #print(pcmData)

    # and save to output file
    with open("waveform.txt", "ab") as file:
        file.write(bytearray(pcmData))


    # graph waveform
    x = np.linspace(0, NUM_SECONDS, SAMPLES_PER_SECONDS*NUM_SECONDS)
    plt.plot(x, np.array(waveform))
    plt.plot(x, np.array(cutoffData))
    plt.xlabel('Time [seconds]')
    plt.ylabel('amplitude')
    plt.axis('tight')
    plt.show()
