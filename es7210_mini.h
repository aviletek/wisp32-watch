/*
 * Minimal ES7210 microphone ADC driver for wisp32-watch.
 *
 * The register map and initialisation sequence are derived from the ES7210
 * driver by Espressif Systems (Shanghai) CO LTD (espressif/esp-bsp),
 * licensed under the Apache License, Version 2.0.
 * This file has been modified from that original.
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ES7210_I2C_ADDR 0x40

bool es7210_mic_array_init(uint8_t gain);

bool es7210_power_down(void);

#ifdef __cplusplus
}
#endif
