/* tng-ai-4u-4
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * ads8698.h: Driver for ADS8698 ADC 
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

#ifndef ADS8698_H
#define ADS8698_H

#include "configs/config.h"

#include <stdint.h>

#define ADS8698_COMMAND_NOOP            0x00
#define ADS8698_COMMAND_STANDBY         0x82
#define ADS8698_COMMAND_POWER_DOWN      0x83
#define ADS8698_COMMAND_RESET           0x85
#define ADS8698_COMMAND_AUTO_SEQUENCE   0xA0
#define ADS8698_COMMAND_MANUAL_CH0      0xC0
#define ADS8698_COMMAND_MANUAL_CH1      0xC4
#define ADS8698_COMMAND_MANUAL_CH2      0xC8
#define ADS8698_COMMAND_MANUAL_CH3      0xCC
#define ADS8698_COMMAND_MANUAL_CH4      0xD0
#define ADS8698_COMMAND_MANUAL_CH5      0xD4
#define ADS8698_COMMAND_MANUAL_CH6      0xD8
#define ADS8698_COMMAND_MANUAL_CH7      0xDC
#define ADS8698_COMMAND_MANUAL_AUX      0xE0

#define ADS8698_PROGRAM_AUTO_SEQ_EN     0x01
#define ADS8698_PROGRAM_CHANNEL_DIS     0x02
#define ADS8698_PROGRAM_FEATURE_SELECT  0x03
#define ADS8698_PROGRAM_INPUT_RANGE_CH0 0x05
#define ADS8698_PROGRAM_INPUT_RANGE_CH1 0x06
#define ADS8698_PROGRAM_INPUT_RANGE_CH2 0x07
#define ADS8698_PROGRAM_INPUT_RANGE_CH3 0x08
#define ADS8698_PROGRAM_INPUT_RANGE_CH4 0x09
#define ADS8698_PROGRAM_INPUT_RANGE_CH5 0x0A
#define ADS8698_PROGRAM_INPUT_RANGE_CH6 0x0B
#define ADS8698_PROGRAM_INPUT_RANGE_CH7 0x0C

#define ADS8698_VOLTAGE_NUM 4
#define ADS8698_CURRENT_NUM 4

typedef struct {
    uint64_t timestamp;
    int32_t voltage[ADS8698_VOLTAGE_NUM];
    int32_t current[ADS8698_CURRENT_NUM];

    uint8_t voltage_range_config[ADS8698_VOLTAGE_NUM];
    uint8_t current_range_config[ADS8698_CURRENT_NUM];

    SPI_HandleTypeDef spi;
} ADS8698;

extern ADS8698 ads8698;

void ads8698_tick(void);
void ads8698_init(void);

#endif