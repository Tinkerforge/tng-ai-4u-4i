/* tng-ai-4u-4i
 * Copyright (C) 2019-2020 Olaf Lüke <olaf@tinkerforge.com>
 *
 * main.c: Initialization for TNG-AI-4U-4I
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

#include "configs/config.h"

#include "bricklib2/tng/tng.h"
#include "bricklib2/logging/logging.h"
#include "ads8698.h"

int main(void) {
	logging_init();
	logd("Start TNG-AI-4U-AI\n\r");

	ads8698_init();

	while(true) {
		tng_tick();
		ads8698_tick();
	}
}
