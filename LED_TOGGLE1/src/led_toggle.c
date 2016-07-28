/**
 * \file
 *
 * \brief SAM LED Toggle Example
 *
 * Copyright (C) 2012-2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage SAM LED Toggle Example
 * See \ref appdoc_main "here" for project documentation.
 * \copydetails appdoc_preface
 *
 *
 * \page appdoc_preface Overview
 * This application demonstrates a simple example to turn on the board LED when
 * a button is pressed, using a variety of methods and modules within the device.
 */

/**
 * \page appdoc_main SAM LED Toggle Example
 *
 * Overview:
 * - \ref appdoc_sam0_led_toggle_app_intro
 * - \ref appdoc_sam0_led_toggle_app_usage
 * - \ref appdoc_sam0_led_toggle_app_config
 * - \ref appdoc_sam0_led_toggle_app_compinfo
 * - \ref appdoc_sam0_led_toggle_app_contactinfo
 *
 * \section appdoc_sam0_led_toggle_app_intro Introduction
 * This application demonstrates a simple example to turn on the board LED when
 * a button is pressed, using a variety of methods and modules within the device.
 *
 * This application has been tested on following boards:
 * - SAM D20/D21/R21/D11/L21 Xplained Pro
 *
 * \section appdoc_sam0_led_toggle_app_usage Usage
 * When run, press the board button to turn on the board LED, release to turn
 * the LED off. If the application settings are altered, the application must be
 * recompiled and re-run on the device.
 *
 * \section appdoc_sam0_led_toggle_app_config Configuration
 * The table \ref appdoc_sam0_led_toggle_app_conftable "below" shows the
 * possible configurations of this example.
 *
 * \anchor appdoc_sam0_led_toggle_app_conftable
 * <table>
 *  <caption>Example Configurations</caption>
 * 	<tr>
 * 		<th>USE_INTERRUPTS</th>
 * 		<th>USE_EIC</th>
 * 		<th>Result</th>
 * 	</tr>
 * 	<tr>
 * 		<td>false</td>
 * 		<td>false</td>
 * 		<td>Polled via PORT driver</td>
 * 	</tr>
 * 	<tr>
 * 		<td>false</td>
 * 		<td>true</td>
 * 		<td>Polled via EIC driver</td>
 * 	</tr>
 * 	<tr>
 * 		<td>true</td>
 * 		<td>false</td>
 * 		<td>Asynchronous using SysTick handler</td>
 * 	</tr>
 * 	<tr>
 * 		<td>true</td>
 * 		<td>true</td>
 * 		<td>Asynchronous using EIC handler</td>
 * 	</tr>
 * </table>
 *
 * \note When the button is connected to the non-maskable pin, the USE_EIC_NMI
 *       in conf_example.h should be defined to true, else be false.
 *
 * \section appdoc_sam0_led_toggle_app_compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for ARM.
 * Other compilers may or may not work.
 *
 * \section appdoc_sam0_led_toggle_app_contactinfo Contact Information
 * For further information, visit
 * <a href="http://www.atmel.com">http://www.atmel.com</a>.
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include "conf_example.h"

/** Updates the board LED to the current button state. */
static void update_led_state(void) {
	static bool ledState = false;
	
	port_pin_toggle_output_level(LED_0_PIN);
	//port_pin_set_output_level(LED_0_PIN,ledState);
	
	
	
	
	if (ledState)
		ledState=false;
	else if (!ledState)
		ledState=true;
}


void SysTick_Handler(void)
{
	update_led_state();
}

/** Configures the SysTick module to fire a SysTick interrupt every 999 system
 *  clock source cycles.
 */
static void configure_systick_handler(void)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = 8000000/10;
	SysTick->VAL  = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

int main(void) {
	system_init();
	
	configure_systick_handler();
	
	
	//update_led_state();
		
}
