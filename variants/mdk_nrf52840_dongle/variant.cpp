/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.
  Copyright (c) 2016 Sandeep Mistry All right reserved.
  Copyright (c) 2018, Adafruit Industries (adafruit.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "variant.h"
#include "wiring_constants.h"
#include "wiring_digital.h"
#include "nrf.h"

const uint32_t g_ADigitalPinMap[] =
{
  // D0 .. D13
  0,  // D0  is P0.25 (UART RX)
  1,  // D1  is P0.24 (UART TX) 
  2,  // D2  is P1.02
  3,  // D3  is P0.06 LED 
  4,  // D4  is P0.29 Button
  5,  // D5  is P0.27
  6,  // D6  is P1.09 (DotStar Clock)
  7,  // D7  is P1.08
  8,  // D8  is P0.08 (DotStar Data)
  9,  // D9  is P0.07
  10,  // D10 is P0.05
  11,  // D11 is P0.26
  12,  // D12 is P0.11
  13,  // D13 is P0.12

  // D14 .. D20 (aka A0 .. A7)
  14,  // D14 is P0.04 (A0)
  15,  // D15 is P0.30 (A1)
  16,  // D16 is P0.28 (A2)
  17,  // D17 is P0.31 (A3)
  18,  // D18 is P0.02 (A4)
  19,  // D19 is P0.03 (A5)

  // Extra Analog pin #20
  20,  // D20 is P0.05 (A6/D10)

  // D21 .. D22 (aka I2C pins)
  21,  // D21 is P0.16 (SDA)
  22,  // D22 is P0.14 (SCL)

  // D23 .. D25 (aka SPI pins)
  23,  // D23 is P0.20 (SPI MISO)
  24,  // D24 is P0.15 (SPI MOSI)
  25,  // D25 is P0.13 (SPI SCK )

  // QSPI pins (not exposed via any header / test point)
  26,  // D26 is P0.19 (QSPI CLK)
  27,  // D27 is P0.23 (QSPI CS)
  28,  // D28 is P0.21 (QSPI Data 0)
  29,  // D29 is P0.22 (QSPI Data 1)
  30,  // D30 is P1.00 (QSPI Data 2)
  31,  // D31 is P0.17 (QSPI Data 3)

  // Thus, there are 32 defined pins

  // The remaining pins are not usable:
  //
  //
  // The following pins were never listed as they were considered unusable
  //  0,      // P0.00 is XL1   (attached to 32.768kHz crystal)
  //  1,      // P0.01 is XL2   (attached to 32.768kHz crystal)
  // 18,      // P0.18 is RESET (attached to switch)
  // 
  // The remaining pins are not connected (per schematic)
  //  9,      // P0.09 is not connected per schematic
  // 10,      // P0.10 is not connected per schematic
  // 33,      // P1.01 is not connected per schematic
  // 35,      // P1.03 is not connected per schematic
  // 36,      // P1.04 is not connected per schematic
  // 37,      // P1.05 is not connected per schematic
  // 38,      // P1.06 is not connected per schematic
  // 39,      // P1.07 is not connected per schematic
  // 43,      // P1.11 is not connected per schematic
  // 44,      // P1.12 is not connected per schematic
  // 45,      // P1.13 is not connected per schematic
  // 46,      // P1.14 is not connected per schematic
  // 47,      // P1.15 is not connected per schematic
};

void initVariant()
{
  // LED1 & LED2
  pinMode(PIN_LED1, OUTPUT);
  ledOff(PIN_LED1);

  pinMode(PIN_LED2, OUTPUT);
  ledOff(PIN_LED2);

  pinMode(PIN_LED3, OUTPUT);
  ledOff(PIN_LED3);
}

