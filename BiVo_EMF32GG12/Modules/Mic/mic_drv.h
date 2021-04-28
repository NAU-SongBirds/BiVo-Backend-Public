/*
 * mic_drv.h
 *
 *  Created on: Feb 11, 2021
 *      Author: kevinimlay
 */

#ifndef MODULES_MIC_MIC_DRV_H_
#define MODULES_MIC_MIC_DRV_H_

#include <stdint.h>
#include <stdbool.h>
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_ldma.h"
#include "em_pdm.h"
#include "serial_usb_drv.h"

/** Pins and Ports for on-board microphone */
#define MIC_CLK_PORT gpioPortB
#define MIC_CLK_PIN 12
#define MIC_CLK_PDM_LOC 1
#define PDM_CLK_3 3

#define MIC_DATA_PORT gpioPortB
#define MIC_DATA_PIN 11
#define MIC_DATA_PDM_LOC 3
#define PDM_DATA0_3 3

#define MIC_EN_PORT gpioPortA
#define MIC_EN_PIN 8

/** Sampling Stuff */
#define BASE_CLK_RATE 19104000	// derived experimentally, may vary on temperature

/** @struct Configuration Struct
 *
 */
struct MicConfig {
			int clk_prescalar;
			int down_sample_rate;
			int mic_gain;
	};

/** @enum Error codes the driver may respond with.
 *
 * See function descriptions for more details of why and what error can respond.
 */
enum Mic_Ecode {
	MIC_OK = 0, MIC_NOT_INITIALIZED = 1, MIC_BUSY = 2
};

/** Function Prototypes */
void micDriver_init(struct MicConfig config);
enum Mic_Ecode startRecording(int16_t *buffer, uint32_t size);
enum Mic_Ecode stopRecording(void);
bool isRecording(void);

#endif /* MODULES_MIC_MIC_DRV_H_ */
