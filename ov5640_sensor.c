// Copyright <2021> [Copyright rossihwang@gmail.com]

#include "./ov5640_sensor.h"

static void configure() {
  ov5640_write_byte (0x3103, 0x11);
	ov5640_write_byte (0x3008, 0x82);
	ov5640_write_byte (0x3008, 0x42);
	ov5640_write_byte (0x3103, 0x03);
	ov5640_write_byte (0x3017, 0xff);
	ov5640_write_byte (0x3018, 0xff);
	ov5640_write_byte (0x3034, 0x1a);
	ov5640_write_byte (0x3037, 0x13);
	ov5640_write_byte (0x3108, 0x01);
	ov5640_write_byte (0x3630, 0x36);
	ov5640_write_byte (0x3631, 0x0e);
	ov5640_write_byte (0x3632, 0xe2);
	ov5640_write_byte (0x3633, 0x12);
	ov5640_write_byte (0x3621, 0xe0);
	ov5640_write_byte (0x3704, 0xa0);
	ov5640_write_byte (0x3703, 0x5a);
	ov5640_write_byte (0x3715, 0x78);
	ov5640_write_byte (0x3717, 0x01);
	ov5640_write_byte (0x370b, 0x60);
	ov5640_write_byte (0x3705, 0x1a);
	ov5640_write_byte (0x3905, 0x02);
	ov5640_write_byte (0x3906, 0x10);
	ov5640_write_byte (0x3901, 0x0a);
	ov5640_write_byte (0x3731, 0x12);
	ov5640_write_byte (0x3600, 0x08);
	ov5640_write_byte (0x3601, 0x33);
	ov5640_write_byte (0x302d, 0x60);
	ov5640_write_byte (0x3620, 0x52);
	ov5640_write_byte (0x371b, 0x20);
	ov5640_write_byte (0x471c, 0x50);
	ov5640_write_byte (0x3a13, 0x43);
	ov5640_write_byte (0x3a18, 0x00);
	ov5640_write_byte (0x3a19, 0xf8);
	ov5640_write_byte (0x3635, 0x13);
	ov5640_write_byte (0x3636, 0x03);
	ov5640_write_byte (0x3634, 0x40);
	ov5640_write_byte (0x3622, 0x01);

	ov5640_write_byte (0x3c01, 0x34);
	ov5640_write_byte (0x3c04, 0x28);
	ov5640_write_byte (0x3c05, 0x98);
	ov5640_write_byte (0x3c06, 0x00);
	ov5640_write_byte (0x3c07, 0x08);
	ov5640_write_byte (0x3c08, 0x00);
	ov5640_write_byte (0x3c09, 0x1c);
	ov5640_write_byte (0x3c0a, 0x9c);
	ov5640_write_byte (0x3c0b, 0x40);
	ov5640_write_byte (0x3810, 0x00);
	ov5640_write_byte (0x3811, 0x10);
	ov5640_write_byte (0x3812, 0x00);
	ov5640_write_byte (0x3708, 0x64);
	ov5640_write_byte (0x4001, 0x02);
	ov5640_write_byte (0x4005, 0x1a);
	ov5640_write_byte (0x3000, 0x00);
	ov5640_write_byte (0x3004, 0xff);
	ov5640_write_byte (0x300e, 0x58);
	ov5640_write_byte (0x302e, 0x00);
	ov5640_write_byte (0x4300, 0x30);
	ov5640_write_byte (0x501f, 0x00);
	ov5640_write_byte (0x440e, 0x00);
	ov5640_write_byte (0x5000, 0xa7);

	ov5640_write_byte (0x3a0f, 0x30);
	ov5640_write_byte (0x3a10, 0x28);
	ov5640_write_byte (0x3a1b, 0x30);
	ov5640_write_byte (0x3a1e, 0x26);
	ov5640_write_byte (0x3a11, 0x60);
	ov5640_write_byte (0x3a1f, 0x14);

	ov5640_write_byte (0x5800, 0x23);
	ov5640_write_byte (0x5801, 0x14);
	ov5640_write_byte (0x5802, 0x0f);
	ov5640_write_byte (0x5803, 0x0f);
	ov5640_write_byte (0x5804, 0x12);
	ov5640_write_byte (0x5805, 0x26);
	ov5640_write_byte (0x5806, 0x0c);
	ov5640_write_byte (0x5807, 0x08);
	ov5640_write_byte (0x5808, 0x05);
	ov5640_write_byte (0x5809, 0x05);
	ov5640_write_byte (0x580a, 0x08);
	ov5640_write_byte (0x580b, 0x0d);
	ov5640_write_byte (0x580c, 0x08);
	ov5640_write_byte (0x580d, 0x03);
	ov5640_write_byte (0x580e, 0x00);
	ov5640_write_byte (0x580f, 0x00);
	ov5640_write_byte (0x5810, 0x03);
	ov5640_write_byte (0x5811, 0x09);
	ov5640_write_byte (0x5812, 0x07);
	ov5640_write_byte (0x5813, 0x03);
	ov5640_write_byte (0x5814, 0x00);
	ov5640_write_byte (0x5815, 0x01);
	ov5640_write_byte (0x5816, 0x03);
	ov5640_write_byte (0x5817, 0x08);
	ov5640_write_byte (0x5818, 0x0d);
	ov5640_write_byte (0x5819, 0x08);
	ov5640_write_byte (0x581a, 0x05);
	ov5640_write_byte (0x581b, 0x06);
	ov5640_write_byte (0x581c, 0x08);
	ov5640_write_byte (0x581d, 0x0e);
	ov5640_write_byte (0x581e, 0x29);
	ov5640_write_byte (0x581f, 0x17);
	ov5640_write_byte (0x5820, 0x11);
	ov5640_write_byte (0x5821, 0x11);
	ov5640_write_byte (0x5822, 0x15);
	ov5640_write_byte (0x5823, 0x28);
	ov5640_write_byte (0x5824, 0x46);
	ov5640_write_byte (0x5825, 0x26);
	ov5640_write_byte (0x5826, 0x08);
	ov5640_write_byte (0x5827, 0x26);
	ov5640_write_byte (0x5828, 0x64);
	ov5640_write_byte (0x5829, 0x26);
	ov5640_write_byte (0x582a, 0x24);
	ov5640_write_byte (0x582b, 0x22);
	ov5640_write_byte (0x582c, 0x24);
	ov5640_write_byte (0x582d, 0x24);
	ov5640_write_byte (0x582e, 0x06);
	ov5640_write_byte (0x582f, 0x22);
	ov5640_write_byte (0x5830, 0x40);
	ov5640_write_byte (0x5831, 0x42);
	ov5640_write_byte (0x5832, 0x24);
	ov5640_write_byte (0x5833, 0x26);
	ov5640_write_byte (0x5834, 0x24);
	ov5640_write_byte (0x5835, 0x22);
	ov5640_write_byte (0x5836, 0x22);
	ov5640_write_byte (0x5837, 0x26);
	ov5640_write_byte (0x5838, 0x44);
	ov5640_write_byte (0x5839, 0x24);
	ov5640_write_byte (0x583a, 0x26);
	ov5640_write_byte (0x583b, 0x28);
	ov5640_write_byte (0x583c, 0x42);
	ov5640_write_byte (0x583d, 0xce);

	ov5640_write_byte (0x5180, 0xff);
	ov5640_write_byte (0x5181, 0xf2);
	ov5640_write_byte (0x5182, 0x00);
	ov5640_write_byte (0x5183, 0x14);
	ov5640_write_byte (0x5184, 0x25);
	ov5640_write_byte (0x5185, 0x24);
	ov5640_write_byte (0x5186, 0x09);
	ov5640_write_byte (0x5187, 0x09);
	ov5640_write_byte (0x5188, 0x09);
	ov5640_write_byte (0x5189, 0x75);
	ov5640_write_byte (0x518a, 0x54);
	ov5640_write_byte (0x518b, 0xe0);
	ov5640_write_byte (0x518c, 0xb2);
	ov5640_write_byte (0x518d, 0x42);
	ov5640_write_byte (0x518e, 0x3d);
	ov5640_write_byte (0x518f, 0x56);
	ov5640_write_byte (0x5190, 0x46);
	ov5640_write_byte (0x5191, 0xf8);
	ov5640_write_byte (0x5192, 0x04);
	ov5640_write_byte (0x5193, 0x70);
	ov5640_write_byte (0x5194, 0xf0);
	ov5640_write_byte (0x5195, 0xf0);
	ov5640_write_byte (0x5196, 0x03);
	ov5640_write_byte (0x5197, 0x01);
	ov5640_write_byte (0x5198, 0x04);
	ov5640_write_byte (0x5199, 0x12);
	ov5640_write_byte (0x519a, 0x04);
	ov5640_write_byte (0x519b, 0x00);
	ov5640_write_byte (0x519c, 0x06);
	ov5640_write_byte (0x519d, 0x82);
	ov5640_write_byte (0x519e, 0x38);

	ov5640_write_byte (0x5480, 0x01);
	ov5640_write_byte (0x5481, 0x08);
	ov5640_write_byte (0x5482, 0x14);
	ov5640_write_byte (0x5483, 0x28);
	ov5640_write_byte (0x5484, 0x51);
	ov5640_write_byte (0x5485, 0x65);
	ov5640_write_byte (0x5486, 0x71);
	ov5640_write_byte (0x5487, 0x7d);
	ov5640_write_byte (0x5488, 0x87);
	ov5640_write_byte (0x5489, 0x91);
	ov5640_write_byte (0x548a, 0x9a);
	ov5640_write_byte (0x548b, 0xaa);
	ov5640_write_byte (0x548c, 0xb8);
	ov5640_write_byte (0x548d, 0xcd);
	ov5640_write_byte (0x548e, 0xdd);
	ov5640_write_byte (0x548f, 0xea);
	ov5640_write_byte (0x5490, 0x1d);

	ov5640_write_byte (0x5381, 0x1e);
	ov5640_write_byte (0x5382, 0x5b);
	ov5640_write_byte (0x5383, 0x08);
	ov5640_write_byte (0x5384, 0x0a);
	ov5640_write_byte (0x5385, 0x7e);
	ov5640_write_byte (0x5386, 0x88);
	ov5640_write_byte (0x5387, 0x7c);
	ov5640_write_byte (0x5388, 0x6c);
	ov5640_write_byte (0x5389, 0x10);
	ov5640_write_byte (0x538a, 0x01);
	ov5640_write_byte (0x538b, 0x98);

	ov5640_write_byte (0x5580, 0x06);
	ov5640_write_byte (0x5583, 0x40);
	ov5640_write_byte (0x5584, 0x10);
	ov5640_write_byte (0x5589, 0x10);
	ov5640_write_byte (0x558a, 0x00);
	ov5640_write_byte (0x558b, 0xf8);
	ov5640_write_byte (0x501d, 0x40);

	ov5640_write_byte (0x5300, 0x08);
	ov5640_write_byte (0x5301, 0x30);
	ov5640_write_byte (0x5302, 0x10);
	ov5640_write_byte (0x5303, 0x00);
	ov5640_write_byte (0x5304, 0x08);
	ov5640_write_byte (0x5305, 0x30);
	ov5640_write_byte (0x5306, 0x08);
	ov5640_write_byte (0x5307, 0x16);
	ov5640_write_byte (0x5309, 0x08);
	ov5640_write_byte (0x530a, 0x30);
	ov5640_write_byte (0x530b, 0x04);
	ov5640_write_byte (0x530c, 0x06);
	ov5640_write_byte (0x5025, 0x00);
	ov5640_write_byte (0x3008, 0x02);

	ov5640_write_byte (0x3035, 0x21);
	ov5640_write_byte (0x3036, 0x69);
	ov5640_write_byte (0x3c07, 0x07);
	ov5640_write_byte (0x3820, 0x47);
	ov5640_write_byte (0x3821, 0x01);
	ov5640_write_byte (0x3814, 0x31);
	ov5640_write_byte (0x3815, 0x31);
	ov5640_write_byte (0x3800, 0x00);
	ov5640_write_byte (0x3801, 0x00);
	ov5640_write_byte (0x3802, 0x00);
	ov5640_write_byte (0x3803, 0xfa);
	ov5640_write_byte (0x3804, 0x0a);
	ov5640_write_byte (0x3805, 0x3f);
	ov5640_write_byte (0x3806, 0x06);
	ov5640_write_byte (0x3807, 0xa9);
	ov5640_write_byte (0x3808, 0x05);
	ov5640_write_byte (0x3809, 0x00);
	ov5640_write_byte (0x380a, 0x02);
	ov5640_write_byte (0x380b, 0xd0);
	ov5640_write_byte (0x380c, 0x07);
	ov5640_write_byte (0x380d, 0x64);
	ov5640_write_byte (0x380e, 0x02);
	ov5640_write_byte (0x380f, 0xe4);
	ov5640_write_byte (0x3813, 0x04);
	ov5640_write_byte (0x3618, 0x00);
	ov5640_write_byte (0x3612, 0x29);
	ov5640_write_byte (0x3709, 0x52);
	ov5640_write_byte (0x370c, 0x03);
	ov5640_write_byte (0x3a02, 0x02);
	ov5640_write_byte (0x3a03, 0xe0);
	ov5640_write_byte (0x3a14, 0x02);
	ov5640_write_byte (0x3a15, 0xe0);
	ov5640_write_byte (0x4004, 0x02);
	ov5640_write_byte (0x3002, 0x1c);
	ov5640_write_byte (0x3006, 0xc3);
	ov5640_write_byte (0x4713, 0x03);
	ov5640_write_byte (0x4407, 0x04);
	ov5640_write_byte (0x460b, 0x37);
	ov5640_write_byte (0x460c, 0x20);
	ov5640_write_byte (0x4837, 0x16);
	ov5640_write_byte (0x3824, 0x04);
	ov5640_write_byte (0x5001, 0x83);
	ov5640_write_byte (0x3503, 0x00);

	ov5640_write_byte (0x4740, 0x21);

}

void ov5640_init() {
    configure();
  //   uint8_t buf;
  //   if (SensorRead1B (SENSOR_ADDR_RD, 0x31, 0x03, &buf) == CY_U3P_SUCCESS) {
	// 	CyU3PDebugPrint (4, "buf:%d\r\n",buf);
	// } else {
	// 	CyU3PDebugPrint (4, "read reg fail\r\n");
	// }
}

void ov5640_reset() {
    CyU3PReturnStatus_t apiRetStatus;

    /* Drive the GPIO low to reset the sensor. */
    apiRetStatus = CyU3PGpioSetValue (SENSOR_RESET_GPIO, CyFalse);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint (4, "GPIO Set Value Error, Error Code = %d\n", apiRetStatus);
        return;
    }

    /* Wait for some time to allow proper reset. */
    CyU3PThreadSleep (10);

    /* Drive the GPIO high to bring the sensor out of reset. */
    apiRetStatus = CyU3PGpioSetValue (SENSOR_RESET_GPIO, CyTrue);
    if (apiRetStatus != CY_U3P_SUCCESS)
    {
        CyU3PDebugPrint (4, "GPIO Set Value Error, Error Code = %d\n", apiRetStatus);
        return;
    }

    /* Delay the allow the sensor to power up. */
    CyU3PThreadSleep (10);
    return;
}

void ov5640_write_byte(uint16_t reg, uint8_t data) {
  CyU3PReturnStatus_t status = CY_U3P_SUCCESS;
  CyU3PI2cPreamble_t preamble;
  uint8_t buf[1];

  preamble.buffer[0] = SENSOR_ADDR_WR;
  preamble.buffer[1] = (uint8_t)(reg >> 8);
  preamble.buffer[2] = (uint8_t)reg;
  preamble.length = 3;
  preamble.ctrlMask = 0x0000;

  buf[0] = data;

  status = CyU3PI2cTransmitBytes(&preamble, buf, 1, 0);
  ov5640_access_delay(status);
}

uint8_t ov5640_read_byte(uint16_t reg) {
  CyU3PReturnStatus_t status = CY_U3P_SUCCESS;
  CyU3PI2cPreamble_t preamble;

  preamble.buffer[0] = SENSOR_ADDR_WR;
  preamble.buffer[1] = (uint8_t)(reg >> 8);
  preamble.buffer[2] = (uint8_t)reg;
  preamble.buffer[3] = SENSOR_ADDR_RD;
  preamble.length = 4;
  preamble.ctrlMask = 0x0004;   /*  Send start bit after third byte of preamble. */

  uint8_t buf[1];
  status = CyU3PI2cReceiveBytes(&preamble, buf, 1, 0);
  ov5640_access_delay(status);

  return buf[0];
}

CyBool_t ov5640_self_test() {
  uint16_t chip_id;
  chip_id = ov5640_read_byte(CHIP_ID_HIGHT_BYTE);
  chip_id <<= 8;
  chip_id |= ov5640_read_byte(CHIP_ID_LOW_BYTE);
  if (chip_id == 0x5640) {
    CyU3PDebugPrint(4, "self-test succeed\n");
    return CyTrue;
  } else {
    CyU3PDebugPrint(4, "self-test failed\n");
    return CyFalse;
  }
}

void ov5640_access_delay(CyU3PReturnStatus_t status) {
  /* Add a 10us delay if the I2C operation that preceded this call was successful. */
  if (status == CY_U3P_SUCCESS) {
    CyU3PBusyWait (10);
  }  
}