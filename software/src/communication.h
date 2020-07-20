/* tng-ai-4u-4i
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/tng/tng.h"

// Default functions
TNGHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants

#define AI_4U_4I_COPY_STATUS_OK 0
#define AI_4U_4I_COPY_STATUS_DEVICE_IDENTIFIER_INCORRECT 1
#define AI_4U_4I_COPY_STATUS_MAGIC_NUMBER_INCORRECT 2
#define AI_4U_4I_COPY_STATUS_LENGTH_MALFORMED 3
#define AI_4U_4I_COPY_STATUS_CRC_MISMATCH 4

// Function and callback IDs and structs
#define FID_GET_VALUES 1
#define FID_GET_VOLTAGES 2
#define FID_GET_CURRENTS 3
#define FID_GET_SELECTED_VOLTAGE 4
#define FID_GET_SELECTED_CURRENT 5


typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetValues;

typedef struct {
	TFPMessageHeader header;
	uint64_t timestamp;
	int32_t voltages[4];
	int32_t currents[4];
} __attribute__((__packed__)) GetValues_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetVoltages;

typedef struct {
	TFPMessageHeader header;
	uint64_t timestamp;
	int32_t voltages[4];
} __attribute__((__packed__)) GetVoltages_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetCurrents;

typedef struct {
	TFPMessageHeader header;
	uint64_t timestamp;
	int32_t currents[4];
} __attribute__((__packed__)) GetCurrents_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
} __attribute__((__packed__)) GetSelectedVoltage;

typedef struct {
	TFPMessageHeader header;
	uint64_t timestamp;
	int32_t voltage;
} __attribute__((__packed__)) GetSelectedVoltage_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t channel;
} __attribute__((__packed__)) GetSelectedCurrent;

typedef struct {
	TFPMessageHeader header;
	uint64_t timestamp;
	int32_t current;
} __attribute__((__packed__)) GetSelectedCurrent_Response;


// Function prototypes
TNGHandleMessageResponse get_values(const GetValues *data, GetValues_Response *response);
TNGHandleMessageResponse get_voltages(const GetVoltages *data, GetVoltages_Response *response);
TNGHandleMessageResponse get_currents(const GetCurrents *data, GetCurrents_Response *response);
TNGHandleMessageResponse get_selected_voltage(const GetSelectedVoltage *data, GetSelectedVoltage_Response *response);
TNGHandleMessageResponse get_selected_current(const GetSelectedCurrent *data, GetSelectedCurrent_Response *response);

// Callbacks


#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 0
#define COMMUNICATION_CALLBACK_LIST_INIT \


#endif
