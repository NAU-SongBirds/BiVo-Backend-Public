/** @file gen_com.c
 * @brief Communication Driver
 *
 * This general communication driver implements the communication schema and is
 * designed to be abstracted from the medium-specific communication needs so
 * changing from USB to something else requires minimal modification to this
 * driver.
 *
 * @author Kevin Imlay
 * @date 2-16-21
 */

#include "gen_com.h"

/** Message Strings */
char handshake_request[5] = "hands";
char handshake_response[5] = "cnfrm";
char handshake_ack[5] = "ackng";
char record_command[5] = "recrd";

/** Initialize the general communication module.
 * At this point, only makes sure the serial communication module is
 * initialized.
 */
void genCom_init(void) {
	// init usb com
	serialUsbDriver_init( );
}

/** @brief Handshake with the desktop application.
 * Blocks until handshake operation is complete. Three-step process: get message
 * from computer, echo that message back, and then receive an acknowledge
 * message from the computer again.
 */
void handshakeApp(void) {
	int com_buffer_size = 5;
	char com_buffer[com_buffer_size];

	// wait for handshake from desktop application
	while (stringCompare(handshake_request, com_buffer, com_buffer_size) != 0) {
		receive_Byte(com_buffer, com_buffer_size);
	}

	// echo back as handshake
	transmit_Byte(handshake_response, com_buffer_size);

	// receive confirmation from app
	while (stringCompare(handshake_ack, com_buffer, com_buffer_size) != 0) {
		receive_Byte(com_buffer, com_buffer_size);
	}
}

/** @brief Block until record message is received.
 * Blocks until the record command is received from the desktop application.
 */
void waitOnRecordMessage(void) {
	int com_buffer_size = 5;
	char com_buffer[com_buffer_size];

	// wait until record message received
	while (stringCompare(record_command, com_buffer, com_buffer_size) != 0) {
		receive_Byte(com_buffer, com_buffer_size);
	}
}

/** Compares two strings to each other up to a specified length. Identicial to
 * strcmp but terminates on length instead of null character.
 *
 * @param str1 First string to compare
 * @param str2 Second string to compare to the first string.
 * @param len The number of characters to compare
 * @return 0 if strings are equal, not 0 otherwise.
 */
int stringCompare(char *str1, char *str2, int len) {
	int sum = 0;

	for (int index=0; index<len; index++) {
		sum = str1[index] - str2[index];
	}

	return sum;
}
