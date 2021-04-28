/** @file standard_mode.c
 * @brief Standard mode is essentially a stand-by mode for recording segments to
 * show case BiVo's functionality.
 * @authors Kevin Imlay
 * @date 4-20-21
 */

#include "standard_mode.h"

/** Settings for the microphone */
struct MicConfig mic_config = {
		.clk_prescalar = 29,
		.down_sample_rate = 32,
		.mic_gain = 7
	};

/** Settings for the audio analysis */
struct AnlysConfig anlys_config = {
		.fftSize = 256,
		.freqLower = 0,
		.freqUpper = 9950,
		.powerThreshold = 20,
		.sampleScaler = 50
	};

/** @brief Initialize the modules needed for the operation of the standard mode.
 * Needed modules:
 * 	serial communication
 * 	microphone
 * 	audio analysis
 */
void initMode(void) {
	// initialize modules
	serialUsbDriver_init( );
	micDriver_init( mic_config );
}

/** @brief De-initialize the modules used for the operation of the standard
 * mode.
 */
void deinitMode(void) {
	audioAnalysis_deinit();
	// nothing more right now, as we will never leave this mode in this version of
	// BiVo
}

/** @brief Run the standard operational mode.
 * First begins with handshake from desktop application. Then falls into the
 * operation of waiting for the command from the app to record a segment,
 * records a segment and analyzes it, and forwards it if it passes analysis.
 * Otherwise, keeps recording segments and analyzing until analysis passes and
 * the segment is forwarded. Then waits for command from app again and repeats
 * indefinitely.
 *
 * @note in this version, handshake happens for every segment as a quick fix for
 * matlab code not keeping track of if board is connected bewteen calls for
 * segment.
 */
void run_standard_mode(void) {
	// initialize variables
	int sampleRate = BASE_CLK_RATE/((mic_config.clk_prescalar+1)*mic_config.down_sample_rate);
	int bufferSize = AUDIO_SEG_LEN*sampleRate;
	int16_t *buffer = (int16_t*) calloc(bufferSize, sizeof(int16_t));
	char end_segment_msg[5] = {'-','e','n','d','-'};

	// initialize the mode
	initMode();

	// loop now, on receiving command and sending audio
	while (true) {
		// wait for command from app to record and send a segment. Doesn't check for
		// if a correct command is sent, just a command
		handshakeApp(); // handshake here because of difficulties making app not handshake every segment call
		waitOnRecordMessage();

		// record segment and send back
		startRecording(buffer, bufferSize);

		while (true) {
			EMU_EnterEM1();

			// when done recording
			if (!isRecording()) {
				// pass into audio analysis
				// if passed analysis, send
				if (analyzeAudio(buffer, bufferSize, sampleRate, anlys_config)) {
					// send
					transmit_HalfWord(buffer, bufferSize);
					transmit_Byte(end_segment_msg, 5);
					break;
				}

				// analysis didn't pass, record a new segment
				else {
					startRecording(buffer, bufferSize);
				}
			}
		}
	}

	// exiting mode, de-initialize the mode
	free(buffer);
	deinitMode();
}
