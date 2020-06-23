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

#include <stdint.h>

typedef struct {

} ADS8698;

extern ADS8698 ads8698;

void ads8698_tick(void);
void ads8698_init(void);

#endif