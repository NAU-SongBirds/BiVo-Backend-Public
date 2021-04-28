/** @file audio_analysis.h
 * @brief Audio Analysis for flagging if the audio passed in meets the
 * characteristics that may mean a bird call is present. The general algorithm
 * is to perform a Fast Fourier Transform on the array of samples passed in.
 * Then the magnitude squared is taken to find the power spectral density. This
 * is then compared against [something] to determine if the audio is
 * "interesting".
 *
 * @authors Kevin Imlay
 * @date 3-9-21
 */

#ifndef MODULES_AUDIO_ANALYSIS_AUDIO_ANALYSIS_H_
#define MODULES_AUDIO_ANALYSIS_AUDIO_ANALYSIS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "arm_math.h"

/* Analysis Configuration */
struct AnlysConfig {
		int fftSize;
		int sampleScaler;
		int powerThreshold;
		int freqLower;
		int freqUpper;
};

/* Function Prototypes */
bool analyzeAudio(int16_t *audioSamples, uint32_t bufferSize,
                  uint16_t samplingRate, struct AnlysConfig config);
void audioAnalysis_deinit( void );
void audioAnalysis_init(void);

#endif /* MODULES_AUDIO_ANALYSIS_AUDIO_ANALYSIS_H_ */
