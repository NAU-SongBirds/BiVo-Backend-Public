/** @file serial_usb_drv.c
 * @brief Serial USB Driver
 *
 * This driver manages the specific functionalities needed to run a serial
 * link to the desktop application over a USB cable using the debug chip's
 * virtual communication port. The Thunderboard has two micro-USB ports, the
 * VCOM connects to the port on the debug side of the board.
 *
 * This code is adapted in part from the leuart_echo peripheral example from by
 * Silicon Labs and the LDMA example code by Silicon Labs.
 * github.com/SiliconLabs/peripheral_examples/tree/master/series1/leuart/leuart_echo.
 * https://docs.silabs.com/mcu/5.8/efm32gg12/group-LDMA#ga0b27c0375b6e3f6a862f9317a7d04fba.
 *
 * @authors Kevin Imlay
 * @date 2-16-21
 *
 * @todo Figure out how to include parameters for the RX and TX callback functions.
 * @todo Assess if possible to send and receive concurrently.
 *
 * @note Do not try to send messages too quickly. Messages may be truncated if
 * transmission to the board begins before the board begins it's receive
 * transmission operation. I recommend allowing at least 1 ms delay from the
 * a received transmission from the board to sending the next transmission to
 * the board.
 */

#include "serial_usb_drv.h"

/** @brief Static flag representing if the serial communication has been
 * initialized.
 *
 * Used to prevent the user from calling a read or write operation without first
 * initializing the communication.
 */
static bool _initializedFlag = false;

/** @brief Sets up the GPIO peripheral for the USART peripheral to communicate
 * with the SEGGAR J-lINK chip.
 *
 * Enables the CMU clock for the GPIO peripheral, enables the GPIO pins needed
 * for the TX and RX channels for the USART to VCOM, and enables the board
 * controller to communicate with the VCOM.
 */
static void setupGpio(void) {
	// enable clock for GPIO peripheral
	CMU_ClockEnable( cmuClock_GPIO, true );

	// Configure VCOM transmit pin to board controller as an output
	GPIO_PinModeSet( BSP_BCC_TXPORT, BSP_BCC_TXPIN, gpioModePushPull, 1 );

	// Configure VCOM receive pin from board controller as an input
	GPIO_PinModeSet( BSP_BCC_RXPORT, BSP_BCC_RXPIN, gpioModeInput, 0 );

	// Enable VCOM connection to board controller
	GPIO_PinModeSet( BSP_BCC_ENABLE_PORT, BSP_BCC_ENABLE_PIN, gpioModePushPull,
	                 1 );
}

/** @brief Sets up the USART with the needed specifications for communication
 * with the VCOM.
 *
 * Enables the CMU clock for the USART peripheral, initiates the USART peripheral,
 * clears the RX and TX interrupt request queues, and finally enables the
 * USART-VCOM connection.
 */
static void setupUsart(void) {
	// enable clock for USART peripheral
	CMU_ClockEnable( cmuClock_USART0, true );

	// Default asynchronous initializer (115.2 Kbps, 8N1, no flow control)
	USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;

	// Configure and enable USART
	USART_InitAsync( USART0, &init );

	// Enable RX and TX for USART-VCOM connection
	USART0->ROUTELOC0 = BSP_BCC_RX_LOCATION | BSP_BCC_TX_LOCATION;
	USART0->ROUTEPEN |= USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
}

/** @brief Initializes the USART and GPIO peripherals for using the SEGGER JLINK
 * chip as a a virtual communication (VCOM) port over the DBG USB port and initializes
 * pointers to input and output buffers.
 *
 * This initialization must be run before any other functions can be called that
 * control the serial communication.
 */
void serialUsbDriver_init(void) {
	// setup GPIO pins and ports for USART communication with SEGGER J-LINK debug
	// to use as a VCOM port over USB cable
	setupGpio( );

	// setup USART for communication with debug
	setupUsart( );

	// set initialized flag
	_initializedFlag = true;
}

/** Transmit half a word at a time over the serial connection.
 * This is useful to send audio samples, as samples are half words.
 *
 * @param buffer Buffer of half words to transmit over the serial connection.
 * @param size The number of half words to transmit.
 */
enum USB_Ecode transmit_HalfWord(int16_t* buffer, uint32_t size) {
	// check if initialized
	if (!_initializedFlag) {
		return SERIAL_USB_NOT_INITIALIZED;
	}

	// send each sample
	for (int i=0; i<size; i++)
	{
		// wait for tx buffer to empty
		while (((USART0->STATUS>>6) & 0x1) == 0);

		// fill tx buffer to send
		USART0->TXDOUBLE = buffer[i];
	}
	return SERIAL_USB_OK;
}

/** Transmit a byte at a time over the serial connection.
 * This is useful to send strings.
 *
 * @param buffer Buffer of bytes (or byte sized) to transmit over the serial
 * connection.
 * @param size The number of bytes to transmit.
 */
enum USB_Ecode transmit_Byte(int8_t* buffer, uint32_t size) {
	// check if initialized
	if (!_initializedFlag) {
		return SERIAL_USB_NOT_INITIALIZED;
	}

	// send each sample
	for (int i=0; i<size; i++)
	{
		// wait for tx buffer to empty
		while (((USART0->STATUS>>6) & 0x1) == 0);

		// fill tx buffer to send
		USART0->TXDATA = buffer[i];
	}
	return SERIAL_USB_OK;
}

/** Receive a byte at a time over the serial connection.
 * This is useful to receive strings.
 *
 * @param buffer Buffer of bytes (or byte sized) to store the received message
 * into.
 * @param size The number of bytes to receive.
 */
enum USB_Ecode receive_Byte(int8_t* buffer, uint32_t size) {
	// check if initialized
	if (!_initializedFlag) {
		return SERIAL_USB_NOT_INITIALIZED;
	}

	// send each sample
	for (int i=0; i<size; i++)
	{
		// wait for rx buffer to fill
		while (((USART0->STATUS>>7) & 0x1) == 0);

		// put into buffer
		buffer[i] = USART0->RXDATA;
	}
	return SERIAL_USB_OK;
}
