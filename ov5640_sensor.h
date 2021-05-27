// Copyright <2021> [Copyright rossihwang@gmail.com]

#ifndef __OV5640_SENSOR_H__
#define __OV5640_SENSOR_H__

#include <cyu3types.h>
#include <cyu3error.h>
#include <cyu3system.h>
#include <cyu3gpio.h>
#include <cyu3i2c.h>
#include <cyu3utils.h>

#define SENSOR_ADDR_WR 0x78
#define SENSOR_ADDR_RD 0x79

#define SENSOR_RESET_GPIO 22

// Processing Unit
#define PU_GAIN_MIN             1             
#define PU_GAIN_MAX             64
#define PU_GAIN_RES             1
#define PU_GAIN_DEF             16

// Camera Terminal
#define CT_AE_MODE_RES         0x03  // return bitmap of the modes supported by this control
#define CT_AE_MODE_DEF         0x00  // Auto Mode

#define CT_EXP_ABS_MIN         1
#define CT_EXP_ABS_MAX         0x0ffff0
#define CT_EXP_ABS_RES         1
#define CT_EXP_ABS_DEF         1000

// Register
#define CHIP_ID_HIGHT_BYTE  0x300A
#define CHIP_ID_LOW_BYTE    0x300B

#define AEC_PK_MANUAL       0x3503
#define AEC_PK_EXPOSURE0    0x3500  // MSB
#define AEC_PK_EXPOSURE1    0x3501
#define AEC_PK_EXPOSURE2    0x3502

#define AEC_PK_REAL_GAIN0   0x350A  // MSB
#define AEC_PK_REAL_GAIN1   0x350B 

void ov5640_init();
void ov5640_reset();
void ov5640_write_byte(uint16_t reg, uint8_t data);
uint8_t ov5640_read_byte(uint16_t reg);
CyBool_t ov5640_self_test();
void ov5640_access_delay(CyU3PReturnStatus_t status);


#endif  // __OV5640_SENSOR_H__