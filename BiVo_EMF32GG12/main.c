/******************************************************************************
 *
 * BiVo EFM32GG12
 *
 * Authors: Anqi Wang, Daniel Mercado, Yasmin Vega, Kevin Imlay
 *
 * Website: https://ceias.nau.edu/capstone/projects/CS/2021/Songbird-F20/
 *
 * Contact: Kevin Imlay - kgimlay@gmail.com
 *
 * Team SongBird - Northern Arizona University 2020-2021 Senior Capstone Project
 *
 ******************************************************************************/

/** @file main.c
 * @brief Main entrance to BiVo sensor code.
 *
 * @authors Anqi Wang
 * @authors Daniel Mercado
 * @authors Yasmin Vega
 * @authors Kevin Imlay
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "em_chip.h"
#include "gen_com.h"


/** @brief Initialize the system for operation.
 */
void initSys(void) {
	CHIP_Init();
	genCom_init();
}


/** @brief Main function.
 * Initialize the system, then ender standard mode of operation.
 */
int main(void) {
	// initialize the system
	initSys();

	// handshake with application before proceeding
	// handshakeApp(); // moved to standard mode as quick fix to front end issue

	// start the standard mode
	run_standard_mode();
}
