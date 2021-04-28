/** @file gen_com.h
 * @brief Communication Driver function prototypes and structures.
 *
 *
 *
 * @author Kevin Imlay
 * @date 4-21-21
 */

#ifndef MODULES_GEN_COM_INC_GEN_COM_H_
#define MODULES_GEN_COM_INC_GEN_COM_H_

#include <stdio.h>
#include "serial_usb_drv.h"

/** Function Prototypes */
void handshakeApp(void);
void waitOnRecordMessage(void);

#endif /* MODULES_GEN_COM_INC_GEN_COM_H_ */
