/* tng-ai-4u-4i
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/tng/usb_stm32/usb.h"

#include "ads8698.h"

TNGHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_VALUES: return get_values(message, response);
		case FID_GET_VOLTAGES: return get_voltages(message, response);
		case FID_GET_CURRENTS: return get_currents(message, response);
		case FID_GET_SELECTED_VOLTAGE: return get_selected_voltage(message, response);
		case FID_GET_SELECTED_CURRENT: return get_selected_current(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}


TNGHandleMessageResponse get_values(const GetValues *data, GetValues_Response *response) {
	response->header.length = sizeof(GetValues_Response);
	response->timestamp     = ads8698.timestamp;
	for(uint8_t i = 0; i < ADS8698_VOLTAGE_NUM; i++) {
		response->voltages[i] = ads8698.voltage[i];
		response->currents[i] = ads8698.current[i];
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

TNGHandleMessageResponse get_voltages(const GetVoltages *data, GetVoltages_Response *response) {
	response->header.length = sizeof(GetVoltages_Response);
	response->timestamp     = ads8698.timestamp;
	for(uint8_t i = 0; i < ADS8698_VOLTAGE_NUM; i++) {
		response->voltages[i] = ads8698.voltage[i];
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

TNGHandleMessageResponse get_currents(const GetCurrents *data, GetCurrents_Response *response) {
	response->header.length = sizeof(GetCurrents_Response);
	response->timestamp     = ads8698.timestamp;
	for(uint8_t i = 0; i < ADS8698_VOLTAGE_NUM; i++) {
		response->currents[i] = ads8698.current[i];
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

TNGHandleMessageResponse get_selected_voltage(const GetSelectedVoltage *data, GetSelectedVoltage_Response *response) {
	if(data->channel > ADS8698_VOLTAGE_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetSelectedVoltage_Response);
	response->timestamp     = ads8698.timestamp;
	response->voltage       = ads8698.voltage[data->channel];

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

TNGHandleMessageResponse get_selected_current(const GetSelectedCurrent *data, GetSelectedCurrent_Response *response) {
	if(data->channel > ADS8698_CURRENT_NUM) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetSelectedCurrent_Response);
	response->timestamp     = ads8698.timestamp;
	response->current       = ads8698.current[data->channel];

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}





void communication_tick(void) {
//	communication_callback_tick();
}

void communication_init(void) {
//	communication_callback_init();
}
