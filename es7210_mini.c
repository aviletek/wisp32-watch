/*
 * Minimal ES7210 microphone ADC driver for wisp32-watch.
 *
 * The register map and initialisation sequence are derived from the ES7210
 * driver by Espressif Systems (Shanghai) CO LTD (espressif/esp-bsp),
 * licensed under the Apache License, Version 2.0.
 * This file has been modified from that original.
 */

#include "es7210_mini.h"
#include "esp32-hal-i2c.h"

#define REG_RESET        0x00
#define REG_MAINCLK      0x02
#define REG_LRCK_DIVH    0x04
#define REG_LRCK_DIVL    0x05
#define REG_POWER_DOWN   0x06
#define REG_OSR          0x07
#define REG_TIME_CTRL0   0x09
#define REG_TIME_CTRL1   0x0A
#define REG_SDP_IF1      0x11
#define REG_SDP_IF2      0x12
#define REG_ADC34_HPF2   0x20
#define REG_ADC34_HPF1   0x21
#define REG_ADC12_HPF2   0x22
#define REG_ADC12_HPF1   0x23
#define REG_ANALOG       0x40
#define REG_MIC12_BIAS   0x41
#define REG_MIC34_BIAS   0x42
#define REG_MIC1_GAIN    0x43
#define REG_MIC2_GAIN    0x44
#define REG_MIC3_GAIN    0x45
#define REG_MIC4_GAIN    0x46
#define REG_MIC1_POWER   0x47
#define REG_MIC2_POWER   0x48
#define REG_MIC3_POWER   0x49
#define REG_MIC4_POWER   0x4A
#define REG_MIC12_POWER  0x4B
#define REG_MIC34_POWER  0x4C

static bool wr(uint8_t reg, uint8_t val) {
  uint8_t buf[2] = { reg, val };
  return i2cWrite(0, ES7210_I2C_ADDR, buf, 2, 1000) == 0;
}

bool es7210_mic_array_init(uint8_t gain) {
  bool ok = true;
  ok &= wr(REG_RESET, 0xFF);
  ok &= wr(REG_RESET, 0x32);
  ok &= wr(REG_TIME_CTRL0, 0x30);
  ok &= wr(REG_TIME_CTRL1, 0x30);
  ok &= wr(REG_ADC12_HPF1, 0x2A);
  ok &= wr(REG_ADC12_HPF2, 0x0A);
  ok &= wr(REG_ADC34_HPF1, 0x2A);
  ok &= wr(REG_ADC34_HPF2, 0x0A);
  ok &= wr(REG_SDP_IF1, 0x00 | 0x60);
  ok &= wr(REG_SDP_IF2, 0x00);
  ok &= wr(REG_ANALOG, 0xC3);
  ok &= wr(REG_MIC12_BIAS, 0x70);
  ok &= wr(REG_MIC34_BIAS, 0x70);
  ok &= wr(REG_MIC1_GAIN, gain | 0x10);
  ok &= wr(REG_MIC2_GAIN, gain | 0x10);
  ok &= wr(REG_MIC3_GAIN, gain | 0x10);
  ok &= wr(REG_MIC4_GAIN, gain | 0x10);
  ok &= wr(REG_MIC1_POWER, 0x08);
  ok &= wr(REG_MIC2_POWER, 0x08);
  ok &= wr(REG_MIC3_POWER, 0x08);
  ok &= wr(REG_MIC4_POWER, 0x08);
  ok &= wr(REG_OSR, 0x20);
  ok &= wr(REG_MAINCLK, 0x01 | (0x01 << 6) | (0x01 << 7));
  ok &= wr(REG_LRCK_DIVH, 0x01);
  ok &= wr(REG_LRCK_DIVL, 0x00);
  ok &= wr(REG_POWER_DOWN, 0x04);
  ok &= wr(REG_MIC12_POWER, 0x0F);
  ok &= wr(REG_MIC34_POWER, 0x0F);
  ok &= wr(REG_RESET, 0x71);
  ok &= wr(REG_RESET, 0x41);
  return ok;
}

bool es7210_power_down(void) {
  return wr(REG_RESET, 0xFF);
}
