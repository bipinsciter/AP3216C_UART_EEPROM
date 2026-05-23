#ifndef _AP3216C_H
#define _AP3216C_H

#include "hal_conf.h"

#define AP3216C_ADDR      (0x1E<<1)
#define REG_SYSTEM_CONG   0x00
#define REG_PS_DATA_L     0x0E
#define REG_PS_DATA_H     0x0F

void ap3216c_init(void);
uint16_t ap3216c_read_proximity(void);
void AP3216C_WriteByte(uint8_t reg, uint8_t value);
uint8_t AP3216C_ReadByte(uint8_t addr);

#endif
