/** @file usb_com.h
 * @brief Serial USB Driver function prototypes and structures.
 *
 * This driver manages the specific functionalities needed to run a serial
 * link to the desktop application over a USB cable using the debug chip's
 * virtual communication port. The Thunderboard has two micro-USB ports, the
 * VCOM connects to the port on the debug side of the board.
 *
 * To receive and send data without interrupting the board's processes too much,
 * LDMA is used. Once a transfer is completed, an interrupt handler is called
 * and the specified callback function is run.
 *
 * Currently, this driver only supports only one transfer operation at a time,
 * as in you cannot send and receive simultaneously. This is merely a design
 * choice for simplicity of program execution, though I have not tested if it is
 * possible to send and receive simultaneously.
 *
 * When defining callback functions for running at the end of a transfer
 * operation, do not attempt to start another transfer operation within. This
 * will always fail because the flag signaling a transfer is complete is set
 * after the callback function completes. Because of this, any attempt to start
 * a new transfer operation will always return as "busy".
 *
 * All transfer operations will return as "not initialized" if the driver has
 * not been initialized beforehand. This is because calling a transfer operation
 * without initializing will appear to the board as if nothing is happening
 * (no incoming transmission or outgoing transmission never ending).
 *
 * The Thunderboard has a much slower clock than (pretty much) all modern
 * personal computers. When communicating with the board, allow at least a 1 ms
 * delay after receiving a message from the board before sending to the board.
 * Sending too soon may cause the front end of the transmission to be truncated
 * and lost. There is no current method implemented to handle a scenario where
 * messages get out of sync such as this.
 *
 * @author Kevin Imlay
 * @date 2-7-21
 * @date 2-14-21
 * @date 2-16-21
 */

#ifndef MODULES_USB_COM_INC_USB_COM_H_
#define MODULES_USB_COM_INC_USB_COM_H_

#include "bsp.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_device.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_emu.h"
#include "em_ldma.h"
#include <stdio.h>

/** @enum Error codes the driver may respond with.
 *
 * See function descriptions for more details of why and what error can respond.
 */
enum USB_Ecode {
	SERIAL_USB_OK = 0, SERIAL_USB_NOT_INITIALIZED = 1, SERIAL_USB_BUSY = 2
};

/* Function Prototypes */
enum USB_Ecode transmit_HalfWord(int16_t* buffer, uint32_t size);
enum USB_Ecode transmit_Byte(int8_t* buffer, uint32_t size);
enum USB_Ecode receive_Byte(int8_t* buffer, uint32_t size);

#endif /* MODULES_USB_COM_INC_USB_COM_H_ */
