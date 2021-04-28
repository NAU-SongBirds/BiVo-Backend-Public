/** @file audio_analysis.c
 * @brief Function of audio analysis to flag audio, and helper functions.
 *
 * @authors Kevin Imlay
 * @date 3-9-21
 */

#include "audio_analysis.h"

/** @brief Perform audio analysis on the audio data given.
 */
bool analyzeAudio(int16_t *audioSamples, uint32_t bufferSize,
                  uint16_t sampleRate, struct AnlysConfig config) {
	// copy samples because FFT computes in place
	q15_t *copyArray = malloc( config.fftSize * sizeof(q15_t) );
	// output of FFT, twice as large as copy array
	q15_t *fftOutput = malloc( config.fftSize * 2 * sizeof(q15_t) );
	// holds magnitude of FFT (used against thresh)
	q15_t *magnitudeOutput = malloc( config.fftSize * sizeof(q15_t) );
	// frequency bin size of the psd
	uint16_t psd_bin_size = sampleRate / config.fftSize;
	// result of the analysis
	bool analysis_result = false;
	// rfft config
	arm_rfft_instance_q15 rfft_instance;
	arm_rfft_init_q15(&rfft_instance, config.fftSize, 0, 1);

	// loop through the entire length of the buffer to analyze
	for (int copyOffset = 0;
			copyOffset < (bufferSize/config.fftSize)*config.fftSize;
			copyOffset += config.fftSize ) {

		// copy into copy array to avoid corrupting the segment's data
		for (int copyToIdx = 0; copyToIdx < config.fftSize; copyToIdx++) {
			copyArray[copyToIdx] = audioSamples[copyOffset + copyToIdx] * config.sampleScaler;
		}

		// perform Fast Fourier Transform
		arm_rfft_q15(&rfft_instance, copyArray, fftOutput);

		// find magnitude of frequencies to find power density spectrum
		arm_cmplx_mag_q15(fftOutput, magnitudeOutput, config.fftSize*2);

		// compare to threshold and return if within frequency range and above
		// threshold. (marking the segment as potential to have bird vocalization)
		for (int testIdx=config.freqLower/psd_bin_size;
				testIdx<=config.freqUpper/psd_bin_size && testIdx<config.fftSize;
				testIdx++) {
			if (magnitudeOutput[testIdx] >= config.powerThreshold) {
				// potentially contains bird vocalizations
				analysis_result = true;
				break;
			}
		}

		// if analysis marks segment, break to return
		if (analysis_result) {
			break;
		}
	}

	// free allocated memory and return result
	return analysis_result;
}
