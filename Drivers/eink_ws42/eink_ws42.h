// eink_ws42.h
// Date: 2018-11-01
// Created by: Mateusz Mamala

#ifndef __EINK_WS42_H
#define __EINK_WS42_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup Drivers Drivers
 * @{
 */

/**
 * @addtogroup Eink_Ws42
 * @{
 */

typedef struct {
	void (*port_init)(void);
	void (*port_reset)(void);
	void (*port_transfer_data)(uint8_t* data, size_t data_length);
	void (*port_set_command_mode)(void);
	void (*port_set_data_mode)(void);
	bool (*port_is_busy)(void);
	void (*port_delay_ms)(uint32_t delay_value);
} eink_ws42_conf_t;

void eink_ws42_init(const eink_ws42_conf_t* conf);
void eink_ws42_test(const eink_ws42_conf_t* conf);
void eink_ws42_enter_deep_sleep(const eink_ws42_conf_t* conf);
void eink_ws42_display_frame(const eink_ws42_conf_t* conf, const uint8_t* frame_data);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
