// eink_ws42_port.c
// Date: 2018-11-01
// Created by: Mateusz Mamala

#include "eink_ws42_port.h"
#include "nrf_drv_spi.h"
#include "custom_board.h"
#include "nrf.h"
#include "nrf_drv_gpiote.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"

static void eink_ws42_port_init(void);
static void eink_ws42_port_reset(void);
static void eink_ws42_port_transfer_data(uint8_t* data, size_t data_length);
static void eink_ws42_port_set_command_mode(void);
static void eink_ws42_port_set_data_mode(void);
static bool eink_ws42_port_is_busy(void);
static void eink_ws42_port_delay_ms(uint32_t delay_value);

const eink_ws42_conf_t eink_ws42_nrf_port = {
	.port_init = eink_ws42_port_init,
	.port_reset = eink_ws42_port_reset,
	.port_transfer_data = eink_ws42_port_transfer_data,
	.port_set_command_mode = eink_ws42_port_set_command_mode,
	.port_set_data_mode = eink_ws42_port_set_data_mode,
	.port_is_busy = eink_ws42_port_is_busy,
	.port_delay_ms = eink_ws42_port_delay_ms
};

static void eink_busy_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
static void spi_event_handler(nrf_drv_spi_evt_t const* p_event);
static bool is_initialized = false;
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(0);

static void eink_ws42_port_init(void) {
	if (is_initialized) {	
		return;
	}
	ret_code_t err_code;
	is_initialized = true;

	nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
	spi_config.ss_pin   = EINK_SPI_SS_PIN;
	spi_config.miso_pin = EINK_SPI_MISO_PIN;
	spi_config.mosi_pin = EINK_SPI_MOSI_PIN;
	spi_config.sck_pin  = EINK_SPI_SCK_PIN;

	err_code = nrf_drv_spi_init(&spi, &spi_config, spi_event_handler);
	APP_ERROR_CHECK(err_code);

	if (!nrf_drv_gpiote_is_init()) {
		nrf_drv_gpiote_init();
	}

	nrf_drv_gpiote_out_config_t out_config = GPIOTE_CONFIG_OUT_SIMPLE(true);
	err_code = nrf_drv_gpiote_out_init(EINK_RESET_PIN, &out_config);
	APP_ERROR_CHECK(err_code);

	err_code = nrf_drv_gpiote_out_init(EINK_DC_PIN, &out_config);
	APP_ERROR_CHECK(err_code);
	
	nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
	err_code = nrf_drv_gpiote_in_init(
			EINK_BUSY_PIN,
			&in_config,
			eink_busy_pin_handler);
	APP_ERROR_CHECK(err_code);
}

static void eink_ws42_port_reset(void) {
	// Reset pin low
	nrf_drv_gpiote_out_clear(EINK_RESET_PIN);
	// Wait 200 ms
	nrf_delay_ms(200);
	// Reset pin high
	nrf_drv_gpiote_out_set(EINK_RESET_PIN);
	// Wait 200 ms
	nrf_delay_ms(200);	
}

static void eink_ws42_port_transfer_data(uint8_t* data, size_t data_length) {
	nrf_drv_spi_transfer(&spi, data, data_length, 0, 0);	
}

static void eink_ws42_port_set_command_mode(void) {
	nrf_drv_gpiote_out_clear(EINK_DC_PIN);
}

static void eink_ws42_port_set_data_mode(void) {
	nrf_drv_gpiote_out_set(EINK_DC_PIN);
}

static bool eink_ws42_port_is_busy(void) {
	return !nrf_drv_gpiote_in_is_set(EINK_BUSY_PIN); 
}

static void eink_ws42_port_delay_ms(uint32_t delay_value) {
	nrf_delay_ms(200);
}

static void eink_busy_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
	
}

static void spi_event_handler(nrf_drv_spi_evt_t const* p_event) {
	
}
