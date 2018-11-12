// eink_ws42.c
// Date: 2018-11-01
// Created by: Mateusz Mamala

#include "eink_ws42.h"
#include <assert.h>

extern void eink_ws42_port_init(void);
extern void eink_ws42_port_reset(void);
extern void eink_ws42_port_transfer_data(uint8_t* data, size_t data_length);
extern void eink_ws42_port_set_command_mode(void);
extern void eink_ws42_port_set_data_mode(void);
extern bool eink_ws42_port_is_busy(void);
extern void eink_ws42_port_delay_ms(uint32_t delay_value);

#define COMMAND_PANEL_SETTING					0x00
#define COMMAND_POWER_SETTING 					0x01
#define COMMAND_POWER_OFF						0x02
#define COMMAND_POWER_ON					0x04
#define COMMAND_BOOSTER_SOFT_START 				0x06
#define COMMAND_DEEP_SLEEP						0x07
#define COMMAND_DATA_START_TRANSMISSION_1		0x10
#define COMMAND_DISPLAY_REFRESH					0x12
#define COMMAND_DATA_START_TRANSMISSION_2		0x13
#define COMMAND_VCOM_LUT						0x20
#define COMMAND_LUTWW							0x21
#define COMMAND_LUTBW							0x22
#define COMMAND_LUTWB							0x23
#define COMMAND_LUTBB							0x24
#define COMMAND_PLL_CONTROL						0x30
#define COMMAND_VCOM_AND_DATA_INTERVAL_SETTING	0x50
#define COMMAND_RESOLUTION_SETTING				0x61
#define COMMAND_VCM_DC_SETTING					0x82

#define EINK_WIDTH		400
#define EINK_HEIGHT		300

const uint8_t vcom0_lut[];
const uint8_t ww_lut[];
const uint8_t bw_lut[];
const uint8_t bb_lut[];
const uint8_t wb_lut[];

static void send_command(const eink_ws42_conf_t* conf, uint8_t command);
static void send_data(const eink_ws42_conf_t* conf, uint8_t data);
static void set_lut(const eink_ws42_conf_t* conf);

void eink_ws42_init(const eink_ws42_conf_t* conf) {
	assert(conf);
	assert(conf->port_init);
	assert(conf->port_reset);
	assert(conf->port_is_busy);
	assert(conf->port_delay_ms);
	assert(conf->port_transfer_data);
	assert(conf->port_set_command_mode);
	assert(conf->port_set_data_mode);

	conf->port_init();
	conf->port_reset();

	// Power settings
	send_command(conf, COMMAND_POWER_SETTING);
	send_data(conf, 0x03);	// VDS_EN, VDG_EN
	send_data(conf, 0x00);	// VCOM_HV, VGHL_LV[1:0]
	send_data(conf, 0x26);	// VDH[5:0]
	send_data(conf, 0x26);	// VDL[5:0]
	send_data(conf, 0x03);	// VDHR[5:0]

	// Booster soft start
	send_command(conf, COMMAND_BOOSTER_SOFT_START);
	send_data(conf, 0x17);	// BT_PHA[7:0]
	send_data(conf, 0x17);	// BT_PHB[7:0]
	send_data(conf, 0x17);	// BT_PHC[5:0]

	// Power on
	send_command(conf, COMMAND_POWER_ON);
	// Wait for busy pin
	while (conf->port_is_busy()) {}

	// Panel setting
	send_command(conf, COMMAND_PANEL_SETTING);
	send_data(conf, 0xBF);
	send_data(conf, 0x0B);

	// PLL control
	send_command(conf, COMMAND_PLL_CONTROL);
	send_data(conf, 0x3C);	// M = 7, N = 4, Frame Rate = 50Hz
}

void eink_ws42_test(const eink_ws42_conf_t* conf) {
	assert(conf);
	
	send_command(conf, COMMAND_RESOLUTION_SETTING);
	send_data(conf, EINK_WIDTH >> 8);
	send_data(conf, EINK_WIDTH & 0xFF);
	send_data(conf, EINK_HEIGHT >> 8);
	send_data(conf, EINK_HEIGHT & 0xFF);

	send_command(conf, COMMAND_VCM_DC_SETTING);
	send_data(conf, 0x12);

	send_command(conf, COMMAND_VCOM_AND_DATA_INTERVAL_SETTING);
	send_data(conf, 0x97);

	set_lut(conf);

	send_command(conf, COMMAND_DISPLAY_REFRESH);
	conf->port_delay_ms(100);
	while (conf->port_is_busy()) {}	
}

void eink_ws42_display_frame(const eink_ws42_conf_t* conf, const uint8_t* frame_data) {	
	assert(conf);
	assert(frame_data);
	
	send_command(conf, COMMAND_RESOLUTION_SETTING);
	send_data(conf, EINK_WIDTH >> 8);
	send_data(conf, EINK_WIDTH & 0xFF);
	send_data(conf, EINK_HEIGHT >> 8);
	send_data(conf, EINK_HEIGHT & 0xFF);

	send_command(conf, COMMAND_VCM_DC_SETTING);
	send_data(conf, 0x12);

	send_command(conf, COMMAND_VCOM_AND_DATA_INTERVAL_SETTING);
	send_data(conf, 0x97);

	send_command(conf, COMMAND_DATA_START_TRANSMISSION_1);
	for (uint32_t i = 0; i < EINK_WIDTH * EINK_HEIGHT / 8; i++) {
		send_data(conf, 0xFF); // Set whole dispaly white
	}
	conf->port_delay_ms(2);
	send_command(conf, COMMAND_DATA_START_TRANSMISSION_2);
	send_data(conf, 0x00);
	send_data(conf, 0xFF);
	send_data(conf, 0x00);
	send_data(conf, 0xFF);
	for (uint32_t i = 4; i < EINK_WIDTH * EINK_HEIGHT / 8; i++) {
		send_data(conf, frame_data[i]); // Send frame data
	}

	set_lut(conf);

	send_command(conf, COMMAND_DISPLAY_REFRESH);
	conf->port_delay_ms(100);
	while (conf->port_is_busy()) {}	
}

void eink_ws42_enter_deep_sleep(const eink_ws42_conf_t* conf) {
	assert(conf);
	send_command(conf, COMMAND_VCOM_AND_DATA_INTERVAL_SETTING);
	send_data(conf, 0x17);
	send_command(conf, COMMAND_VCM_DC_SETTING);
	send_command(conf, COMMAND_PANEL_SETTING);
	conf->port_delay_ms(100);
	
	send_command(conf, COMMAND_POWER_SETTING);
	send_data(conf, 0x00);	
	send_data(conf, 0x00);
	send_data(conf, 0x00);
	send_data(conf, 0x00);
	send_data(conf, 0x00);
	conf->port_delay_ms(100);

	send_command(conf, COMMAND_POWER_OFF);
	while (conf->port_is_busy()) {}
	send_command(conf, COMMAND_DEEP_SLEEP);
	send_data(conf, 0xA5);
}
static void send_command(const eink_ws42_conf_t* conf, uint8_t command) {
	conf->port_set_command_mode();
	conf->port_transfer_data(&command, 1);
}

static void send_data(const eink_ws42_conf_t* conf, uint8_t data) {
	conf->port_set_data_mode();
	conf->port_transfer_data(&data, 1);
}

static void set_lut(const eink_ws42_conf_t* conf) {
	send_command(conf, COMMAND_VCOM_LUT);
	for (uint8_t i = 0; i < 44; i++) {
		send_data(conf, vcom0_lut[i]);
	}

	send_command(conf, COMMAND_LUTWW);
	for (uint8_t i = 0; i < 42; i++) {
                send_data(conf, ww_lut[i]);
        }

	send_command(conf, COMMAND_LUTBW);
        for (uint8_t i = 0; i < 42; i++) {
                send_data(conf, bw_lut[i]);
        }

	send_command(conf, COMMAND_LUTBB);
        for (uint8_t i = 0; i < 42; i++) {
                send_data(conf, bb_lut[i]);
        }

	send_command(conf, COMMAND_LUTWB);
        for (uint8_t i = 0; i < 42; i++) {
                send_data(conf, wb_lut[i]);
        }
}

const uint8_t vcom0_lut[] = {
	0x00, 0x17, 0x00, 0x00, 0x00, 0x02,
	0x00, 0x17, 0x17, 0x00, 0x00, 0x02,
	0x00, 0x0A, 0x01, 0x00, 0x00, 0x01,
	0x00, 0x0E, 0x0E, 0x00, 0x00, 0x02,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00
};

const uint8_t ww_lut[] = {
	0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
	0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
	0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
	0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t bw_lut[] = {
	0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
	0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
	0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
	0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t bb_lut[] = {
	0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
	0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
	0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
	0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t wb_lut[] = {
	0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
	0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
	0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
	0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
