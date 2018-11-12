/*
 * custom_board.h
 *
 *  Created on: 15 lis 2017
 *      Author: Mateusz
 */

#ifndef CONFIG_CUSTOM_BOARD_H_
#define CONFIG_CUSTOM_BOARD_H_

#include "nrf_gpio.h"

#define HWFC           true

#define MS5611_SCL_PIN				22     // MS5611 SCL
#define MS5611_SDA_PIN				21     // MS5611 SDA

#define SSD1306BZ_SCL_PIN			15
#define SSD1306BZ_SDA_PIN			14
#define SSD1306BZ_RESET_PIN			23
#define SSD1306BZ_LCD_ON_PIN		4

#define EINK_RESET_PIN 12
#define EINK_DC_PIN 16
#define EINK_BUSY_PIN 14

#define EINK_SPI_SS_PIN 8
#define EINK_SPI_MISO_PIN 4
#define EINK_SPI_MOSI_PIN 5
#define EINK_SPI_SCK_PIN 6


// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_RC,            \
                                 .rc_ctiv       = 16,                                \
                                 .rc_temp_ctiv  = 2,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

#endif /* CONFIG_CUSTOM_BOARD_H_ */
