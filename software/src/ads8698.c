/* tng-ai-4u-4i
 * Copyright (C) 2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * ads8698.c: Driver for ADS8698 ADC
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

#include "ads8698.h"

#include "configs/config_ads8698.h"
#include "bricklib2/logging/logging.h"
#include "bricklib2/tng/tng_led.h"

ADS8698 ads8698;

int32_t ads8698_voltage_from_data(uint8_t *data) {
	int32_t adc_value = (data[0] << 10) | (data[1] << 2) | (data[2] >> 2);

//  return adc_value*20480/262144 - 10240
	return adc_value*5/64 - 10240;
}

int32_t ads8698_current_from_data(uint8_t *data) {
	int32_t adc_value = (data[0] << 10) | (data[1] << 2) | (data[2] >> 2);

	// Complete divider: 20645.16129032258
	// TODO: We do have more resolution here in theory!

//	return adc_value*512*10*1000/(124*262144) - 20645;
	return adc_value*625/3968 - 20645; // TODO
}

void ads8698_write_command_register(const uint8_t command) {
	uint8_t tx[5] = {command, 0x00, 0x00, 0x00, 0x00};
	uint8_t rx[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

	HAL_GPIO_WritePin(ADS8698_SPI_CS_PORT, ADS8698_SPI_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&ads8698.spi, tx, rx, 5, ADS8698_SPI_TIMEOUT); // TODO: Check status
	HAL_GPIO_WritePin(ADS8698_SPI_CS_PORT, ADS8698_SPI_CS_PIN, GPIO_PIN_SET);
}

uint8_t *ads8698_read_adc_register(void) {
	static uint8_t rx[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t tx[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	HAL_GPIO_WritePin(ADS8698_SPI_CS_PORT, ADS8698_SPI_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&ads8698.spi, tx, rx, 5, ADS8698_SPI_TIMEOUT); // TODO: Check status
	HAL_GPIO_WritePin(ADS8698_SPI_CS_PORT, ADS8698_SPI_CS_PIN, GPIO_PIN_SET);
	return &rx[2];
}

void ads8698_write_program_register(const uint8_t reg, const uint8_t data) {
	uint8_t tx[3] = {(reg << 1) | 1, data, 0x00};
	uint8_t rx[3] = {0x00, 0x00, 0x00};
	HAL_GPIO_WritePin(ADS8698_SPI_CS_PORT, ADS8698_SPI_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&ads8698.spi, tx, rx, 3, ADS8698_SPI_TIMEOUT); // TODO: Check status
	HAL_GPIO_WritePin(ADS8698_SPI_CS_PORT, ADS8698_SPI_CS_PIN, GPIO_PIN_SET);
}

uint8_t ads8698_read_program_register(const uint8_t reg) {
	uint8_t tx[3] = {(reg << 1) | 0, 0x00, 0x00};
	uint8_t rx[3] = {0x00, 0x00, 0x00};
	HAL_GPIO_WritePin(ADS8698_SPI_CS_PORT, ADS8698_SPI_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&ads8698.spi, tx, rx, 3, ADS8698_SPI_TIMEOUT); // TODO: Check status
	HAL_GPIO_WritePin(ADS8698_SPI_CS_PORT, ADS8698_SPI_CS_PIN, GPIO_PIN_SET);

	return rx[2];
}

#if 0
bool ads8698_check_range_configuration(void) {
	for(uint8_t i = 0; i < (ADS8698_VOLTAGE_NUM + ADS8698_CURRENT_NUM); i++) {
		uint8_t range_conf = ads8698_read_program_register(i + ADS8698_PROGRAM_INPUT_RANGE_CH0);
		logd("range conf %d: %b\n\r", i, range_conf);
	}
	return true;
}
#endif

void ads8698_set_range_configuration(void) {
	for(uint8_t i = 0; i < (ADS8698_VOLTAGE_NUM + ADS8698_CURRENT_NUM); i++) {
		uint8_t conf = 0;
		switch(i) {
			case 0: conf = ads8698.current_range_config[0]; break;
			case 1: conf = ads8698.voltage_range_config[1]; break;
			case 2: conf = ads8698.current_range_config[2]; break;
			case 3: conf = ads8698.voltage_range_config[3]; break;
			case 4: conf = ads8698.voltage_range_config[2]; break;
			case 5: conf = ads8698.current_range_config[3]; break;
			case 6: conf = ads8698.voltage_range_config[0]; break;
			case 7: conf = ads8698.current_range_config[1]; break;
		}
		ads8698_write_program_register(i + ADS8698_PROGRAM_INPUT_RANGE_CH0, conf);
	}
}

void ads8698_tick(void) {
	static uint32_t last_time = 0;

	ads8698_write_command_register(ADS8698_COMMAND_AUTO_SEQUENCE);

	for(uint8_t i = 0; i < (ADS8698_VOLTAGE_NUM + ADS8698_CURRENT_NUM); i++) {
		system_timer_sleep_us(100);
		uint8_t *adc_data = ads8698_read_adc_register();

		switch(i) {
			case 0: ads8698.current[0] = ads8698_current_from_data(adc_data); break;
			case 1: ads8698.voltage[1] = ads8698_voltage_from_data(adc_data); break;
			case 2: ads8698.current[2] = ads8698_current_from_data(adc_data); break;
			case 3: ads8698.voltage[3] = ads8698_voltage_from_data(adc_data); break;
			case 4: ads8698.voltage[2] = ads8698_voltage_from_data(adc_data); break;
			case 5: ads8698.current[3] = ads8698_current_from_data(adc_data); break;
			case 6: ads8698.voltage[0] = ads8698_voltage_from_data(adc_data); break;
			case 7: ads8698.current[1] = ads8698_current_from_data(adc_data); break;
		}
	}

	ads8698.timestamp = system_timer_get_us();
	system_timer_sleep_us(200);

	tng_led_channel_set(TNG_LED_VOLTAGE_CHANNEL_0, ads8698.voltage[0] > 0);
	tng_led_channel_set(TNG_LED_VOLTAGE_CHANNEL_1, ads8698.voltage[1] > 0);
	tng_led_channel_set(TNG_LED_VOLTAGE_CHANNEL_2, ads8698.voltage[2] > 0);
	tng_led_channel_set(TNG_LED_VOLTAGE_CHANNEL_3, ads8698.voltage[3] > 0);

	tng_led_channel_set(TNG_LED_CURRENT_CHANNEL_0, ads8698.current[0] > 12000);
	tng_led_channel_set(TNG_LED_CURRENT_CHANNEL_1, ads8698.current[1] > 12000);
	tng_led_channel_set(TNG_LED_CURRENT_CHANNEL_2, ads8698.current[2] > 12000);
	tng_led_channel_set(TNG_LED_CURRENT_CHANNEL_3, ads8698.current[3] > 12000);

	if(system_timer_is_time_elapsed_ms(last_time, 500)) {
		last_time = system_timer_get_ms();
		logd("Voltage: ch0 %dmV, ch2 %dmV, ch4 %dmV, ch6 %dmV\n\r", ads8698.voltage[0], ads8698.voltage[1], ads8698.voltage[2], ads8698.voltage[3]);
		logd("Current: ch1 %duA, ch3 %duA, ch5 %duA, ch7 %duA\n\r", ads8698.current[0], ads8698.current[1], ads8698.current[2], ads8698.current[3]);
	}
}

void ads8698_init_spi(void) {
	// Enable clocks
	__HAL_RCC_SPI1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// Configure CLK
	GPIO_InitTypeDef gpio_clk = {
		.Pin       = ADS8698_SPI_CLK_PIN,
		.Mode      = GPIO_MODE_AF_PP,
		.Pull      = GPIO_NOPULL,
		.Speed     = GPIO_SPEED_FREQ_HIGH,
		.Alternate = ADS8698_SPI_CLK_AF
	};
	HAL_GPIO_Init(ADS8698_SPI_CLK_PORT, &gpio_clk);

	// Configure MOSI
	GPIO_InitTypeDef gpio_mosi = {
		.Pin       = ADS8698_SPI_MOSI_PIN,
		.Mode      = GPIO_MODE_AF_PP,
		.Pull      = GPIO_NOPULL,
		.Speed     = GPIO_SPEED_FREQ_HIGH,
		.Alternate = ADS8698_SPI_MOSI_AF
	};
	HAL_GPIO_Init(ADS8698_SPI_MOSI_PORT, &gpio_mosi);

	// Configure MISO
	GPIO_InitTypeDef gpio_miso = {
		.Pin       = ADS8698_SPI_MISO_PIN,
		.Mode      = GPIO_MODE_AF_PP,
		.Pull      = GPIO_NOPULL,
		.Speed     = GPIO_SPEED_FREQ_HIGH,
		.Alternate = ADS8698_SPI_MISO_AF
	};
	HAL_GPIO_Init(ADS8698_SPI_MISO_PORT, &gpio_miso);

	// Configure CS
	GPIO_InitTypeDef gpio_cs = {
		.Pin       = ADS8698_SPI_CS_PIN,
		.Mode      = GPIO_MODE_OUTPUT_PP,
		.Pull      = GPIO_NOPULL,
		.Speed     = GPIO_SPEED_FREQ_HIGH,
	};
	HAL_GPIO_Init(ADS8698_SPI_CS_PORT, &gpio_cs);

	// Reset
	__HAL_RCC_SPI1_FORCE_RESET();
	__HAL_RCC_SPI1_RELEASE_RESET();

	ads8698.spi.Instance               = ADS8698_SPI_INSTANCE;
	ads8698.spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	ads8698.spi.Init.Direction         = SPI_DIRECTION_2LINES;
	ads8698.spi.Init.CLKPhase          = SPI_PHASE_2EDGE;
	ads8698.spi.Init.CLKPolarity       = SPI_POLARITY_LOW;
	ads8698.spi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	ads8698.spi.Init.CRCPolynomial     = 7;
	ads8698.spi.Init.DataSize          = SPI_DATASIZE_8BIT;
	ads8698.spi.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	ads8698.spi.Init.NSS               = SPI_NSS_SOFT; 
	ads8698.spi.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
	ads8698.spi.Init.TIMode            = SPI_TIMODE_DISABLE;
	ads8698.spi.Init.Mode              = SPI_MODE_MASTER;	
	HAL_SPI_Init(&ads8698.spi);

	// 15ms turn-on time according to datasheet
	system_timer_sleep_ms(150);

	for(uint8_t i = 0; i < ADS8698_VOLTAGE_NUM; i++) {
		ads8698.voltage_range_config[i] = 0b0000;
		ads8698.current_range_config[i] = 0b0010;
	}

	// Latch CS without moving clock to make sure ADS8698 is in sane state
	HAL_GPIO_WritePin(ADS8698_SPI_CS_PORT, ADS8698_SPI_CS_PIN, GPIO_PIN_RESET);
	system_timer_sleep_ms(1);
	HAL_GPIO_WritePin(ADS8698_SPI_CS_PORT, ADS8698_SPI_CS_PIN, GPIO_PIN_SET);
	system_timer_sleep_ms(1);

	// Write NOOP command to make ADS8698 wake up
	ads8698_write_command_register(ADS8698_COMMAND_NOOP);

	ads8698_set_range_configuration();
}

void ads8698_init(void) {
	ads8698_init_spi();
}