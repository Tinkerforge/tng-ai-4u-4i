/* tng-ai-4u-4i
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config.h: All configurations for TNG-AI-4U-4I
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef CONFIG_GENERAL_H
#define CONFIG_GENERAL_H

#define TNG_MODULE_NAME "TNG AI 4U 4I"
#define TNG_DEVICE_IDENTIFIER 203

#define SYSTEM_TIMER_MAIN_CLOCK_MHZ_48 // For fast divide by 48
#define SYSTEM_TIMER_MAIN_CLOCK_MHZ 48
#define SYSTEM_TIMER_USE_64BIT_US
#define SYSTEM_TIMER_IS_RAMFUNC
#define SYSTEM_TIMER_FREQUENCY 1000 // Use 1 kHz system timer

#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0
#define HARDWARE_VERSION_REVISION 0

#define FIRMWARE_VERSION_MAJOR 1
#define FIRMWARE_VERSION_MINOR 0
#define FIRMWARE_VERSION_REVISION 0

#define UARTBB_TX_PIN GPIO_PIN_12
#define UARTBB_TX_PORTB

#include "bricklib2/tng/config_stm32f0_128kb.h"

#endif
