#ifndef __GPT_I2C_H__
#define __GPT_I2C_H__

#include <i2c.h>

int gpt_i2c_board_init(void);
uint8_t i2c_reg_bus_read(uint8_t bus, uint8_t addr, uint8_t reg);
uint8_t i2c_reg_bus_write(uint8_t bus, uint8_t addr, uint8_t reg, uint8_t val);
void set_extcr_lo_reg(int device_num);

#endif
