// main.cpp - nRF51 Waveshare EInk 4.2"
// Date: 2018-11-01
// Created by: Mateusz Mamala

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "eink_ws42.h"
#include "eink_ws42_port.h"

extern const uint8_t img_data[];

int main(void) {
	APP_ERROR_CHECK(NRF_LOG_INIT(NULL));

	NRF_LOG_INFO("Hello world from nRF51!");

	eink_ws42_init(&eink_ws42_nrf_port);
	eink_ws42_test(&eink_ws42_nrf_port);

	eink_ws42_nrf_port.port_delay_ms(3000);
	eink_ws42_display_frame(&eink_ws42_nrf_port, img_data);	

	eink_ws42_nrf_port.port_delay_ms(3000);

	eink_ws42_enter_deep_sleep(&eink_ws42_nrf_port);
	for (;;) {
		NRF_LOG_PROCESS();
	}
}
