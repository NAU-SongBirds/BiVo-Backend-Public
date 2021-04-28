/** @file mic_drv.c
 * @brief Microphone Driver
 *
 * This code is adapted in part from the pdm_sterio_ldma peripheral example by
 * Silicon Labs.
 * https://github.com/SiliconLabs/peripheral_examples/tree/master/series2/pdm/pdm_stereo_interrupt.
 *
 * @authors Kevin Imlay
 * @date 3-19-21
 */

#include "mic_drv.h"

/** Right track from microphones */
static int16_t *_right_track = NULL;		// samples from the right microphone
static uint32_t _right_track_len = 0; 	// right microphone buffer length
static uint32_t _right_track_index = 0;	// index counter for iterating
/** Eventually, allow for left track, put here! **/

/** Operation variables */
static bool _is_recording = false;
static bool _initializedFlag = false;

/** @brief PDM Interrupt Handler.
 * When the FIFO is full, takes the samples out and puts them into the buffer.
 * When the buffer is filled, stops recording and resets the recording
 * flag.
 */
void PDM_IRQHandler(void) {
	uint32_t interruptFlags = PDM->IF;

	// if data is available in the FIFO
	if (interruptFlags & PDM_IF_DVL)
	{
		PDM->IFC = PDM_IF_DVL;

		// get the 4 samples in the FIFO
		while (!( PDM->STATUS & PDM_STATUS_EMPTY ))
		{
			_right_track[ _right_track_index ] = PDM->RXDATA & 0x0000FFFF;
			_right_track_index = _right_track_index + 1;

			// if the buffer is full, set flag and disable recording
			if (_right_track_index == _right_track_len - 1)
			{
				stopRecording( );
			}
		}
	}
}

/** @brief Initializes the board's PDM peripheral.
 *
 */
void initPdmMic(struct MicConfig config) {
	// select clock to run the PDM peripheral
	CMU->HFBUSCLKEN0 |= CMU_HFBUSCLKEN0_GPIO;
	CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_PDM;
	CMU->PDMCTRL |= CMU_PDMCTRL_PDMCLKSEL_HFRCO | CMU_PDMCTRL_PDMCLKEN;

	// Config GPIO and pin routing
	GPIO_PinModeSet( MIC_CLK_PORT, MIC_CLK_PIN, gpioModePushPull, 0 );
	GPIO_PinModeSet( MIC_DATA_PORT, MIC_DATA_PIN, gpioModeInput, 0 );
	GPIO_SlewrateSet( MIC_DATA_PORT, 7, 7 );

	// Enable microphone connection
	GPIO_PinOutSet( MIC_EN_PORT, MIC_EN_PIN );

	//(PDM_DATA0#3, PDM_CLK#3)
	PDM->ROUTELOC0 = ( PDM_DATA0_3 << _PDM_ROUTELOC0_DAT0LOC_SHIFT );
	PDM->ROUTELOC1 = ( PDM_CLK_3 << _PDM_ROUTELOC1_CLKLOC_SHIFT );
	PDM->ROUTEPEN = ( 1 << _PDM_ROUTEPEN_DAT0PEN_SHIFT )
	    | ( 1 << _PDM_ROUTEPEN_CLKPEN_SHIFT );

	// Prescale the clock
	while (PDM->SYNCBUSY != 0);
	PDM->CFG1 = ( config.clk_prescalar << _PDM_CFG1_PRESC_SHIFT );

	// Configure PDM
	PDM->CFG0 = PDM_CFG0_STEREOMODECH01_DISABLE | PDM_CFG0_CH0CLKPOL_NORMAL
	    | PDM_CFG0_FIFODVL_FOUR | PDM_CFG0_DATAFORMAT_RIGHT16 | PDM_CFG0_NUMCH_ONE
	    | PDM_CFG0_FORDER_FIFTH;

	// Configure down sample rate and gain
	while (PDM->SYNCBUSY != 0);
	PDM->CTRL = ( PDM_CTRL_OUTCLKEN | ( config.mic_gain << _PDM_CTRL_GAIN_SHIFT )
	    | ( config.down_sample_rate << _PDM_CTRL_DSR_SHIFT ) );

	// Enable module
	PDM->EN = PDM_EN_EN;

	// Enable Interrupts
	PDM->IEN = PDM_IEN_DVL;
	NVIC_ClearPendingIRQ( PDM_IRQn );
	NVIC_EnableIRQ( PDM_IRQn );
}

/** @brief Single-shot record an audio segment.
 * Sets pointers for sampling into, sets the recording flag, and starts
 * recording into the buffer.
 */
enum Mic_Ecode startRecording(int16_t *buffer, uint32_t size) {
	// check if initialized
	if (!_initializedFlag) {
		return MIC_NOT_INITIALIZED;
	}

	// set pointers and counters
	_right_track = buffer;
	_right_track_len = size;
	_right_track_index = 0;

	// Start filter
	while (PDM->SYNCBUSY != 0);
	_is_recording = true;
	PDM->CMD = PDM_CMD_START;

	return MIC_OK;
}

/** @brief Terminates the recording.
 * Stops recording and resets the recording flag.
 */
enum Mic_Ecode stopRecording(void) {
	// check if initialized
	if (!_initializedFlag) {
		return MIC_NOT_INITIALIZED;
	}

	// Stop filter
	while (PDM->SYNCBUSY != 0);
	PDM->CMD = PDM_CMD_STOP;
	_is_recording = false;

	return MIC_OK;
}

/** @brief Gets the status of recording.
 * Getter for the recording flag. Should be called to check if the recording is
 * complete.
 */
bool isRecording(void) {
	return _is_recording;
}

/** @brief Initialize the microphone driver.
 *
 */
void micDriver_init(struct MicConfig config) {
	// initialize the microphones
	initPdmMic( config );

	// set initialized flag
	_initializedFlag = true;
}
