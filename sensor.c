/*
 ## Cypress FX3 Camera Kit source file (sensor.c)
 ## ===========================
 ##
 ##  Copyright Cypress Semiconductor Corporation, 2010-2012,
 ##  All Rights Reserved
 ##  UNPUBLISHED, LICENSED SOFTWARE.
 ##
 ##  CONFIDENTIAL AND PROPRIETARY INFORMATION
 ##  WHICH IS THE PROPERTY OF CYPRESS.
 ##
 ##  Use of this file is governed
 ##  by the license agreement included in the file
 ##
 ##     <install>/license/license.txt
 ##
 ##  where <install> is the Cypress software
 ##  installation root directory path.
 ##
 ## ===========================
*/

/* This file implements the I2C based driver for the MT9M114 image sensor used
   in the FX3 HD 720p camera kit.

   Please refer to the Aptina MT9M114 sensor datasheet for the details of the
   I2C commands used to configure the sensor.
 */

#include <cyu3system.h>
#include <cyu3os.h>
#include <cyu3dma.h>
#include <cyu3error.h>
#include <cyu3uart.h>
#include <cyu3i2c.h>
#include <cyu3types.h>
#include <cyu3gpio.h>
#include <cyu3utils.h>
#include "sensor.h"

static void Step1_ov5640_Reset (void);

/* This function inserts a delay between successful I2C transfers to prevent
   false errors due to the slave being busy.
 */
static void
SensorI2CAccessDelay (
        CyU3PReturnStatus_t status)
{
    /* Add a 10us delay if the I2C operation that preceded this call was successful. */
    if (status == CY_U3P_SUCCESS)
        CyU3PBusyWait (10);
}

/* Write to an I2C slave with two bytes of data. */
CyU3PReturnStatus_t
SensorWrite2B (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t highData,
        uint8_t lowData)
{
    CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;
    CyU3PI2cPreamble_t  preamble;
    uint8_t buf[2];

    /* Validate the I2C slave address. */
    if ((slaveAddr != SENSOR_ADDR_WR) && (slaveAddr != I2C_MEMORY_ADDR_WR))
    {
        CyU3PDebugPrint (4, "I2C Slave address is not valid!\n");
        return 1;
    }

    /* Set the parameters for the I2C API access and then call the write API. */
    preamble.buffer[0] = slaveAddr;
    preamble.buffer[1] = highAddr;
    preamble.buffer[2] = lowAddr;
    preamble.length    = 3;             /*  Three byte preamble. */
    preamble.ctrlMask  = 0x0000;        /*  No additional start and stop bits. */

    buf[0] = highData;
    buf[1] = lowData;

    apiRetStatus = CyU3PI2cTransmitBytes (&preamble, buf, 2, 0);
    SensorI2CAccessDelay (apiRetStatus);

    return apiRetStatus;
}

/* Write to an I2C slave with two bytes of data. */
CyU3PReturnStatus_t
SensorWrite1B (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t Data)
{
    CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;
    CyU3PI2cPreamble_t  preamble;
    uint8_t buf[2];

    /* Validate the I2C slave address. */
    if ((slaveAddr != SENSOR_ADDR_WR) && (slaveAddr != I2C_MEMORY_ADDR_WR))
    {
        CyU3PDebugPrint (4, "I2C Slave address is not valid!\n");
        return 1;
    }

    /* Set the parameters for the I2C API access and then call the write API. */
    preamble.buffer[0] = slaveAddr;
    preamble.buffer[1] = highAddr;
    preamble.buffer[2] = lowAddr;
    preamble.length    = 3;             /*  Three byte preamble. */
    preamble.ctrlMask  = 0x0000;        /*  No additional start and stop bits. */

    buf[0] = Data;

    apiRetStatus = CyU3PI2cTransmitBytes (&preamble, buf, 1, 0);
    SensorI2CAccessDelay (apiRetStatus);

    return apiRetStatus;
}

CyU3PReturnStatus_t
SensorWrite (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t count,
        uint8_t *buf)
{
    CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;
    CyU3PI2cPreamble_t preamble;

    /* Validate the I2C slave address. */
    if ((slaveAddr != SENSOR_ADDR_WR) && (slaveAddr != I2C_MEMORY_ADDR_WR))
    {
        CyU3PDebugPrint (4, "I2C Slave address is not valid!\n");
        return 1;
    }

    if (count > 64)
    {
        CyU3PDebugPrint (4, "ERROR: SensorWrite count > 64\n");
        return 1;
    }

    /* Set up the I2C control parameters and invoke the write API. */
    preamble.buffer[0] = slaveAddr;
    preamble.buffer[1] = highAddr;
    preamble.buffer[2] = lowAddr;
    preamble.length    = 3;
    preamble.ctrlMask  = 0x0000;

    apiRetStatus = CyU3PI2cTransmitBytes (&preamble, buf, count, 0);
    SensorI2CAccessDelay (apiRetStatus);

    return apiRetStatus;
}

CyU3PReturnStatus_t
SensorRead2B (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t *buf)
{
    CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;
    CyU3PI2cPreamble_t preamble;

    if ((slaveAddr != SENSOR_ADDR_RD) && (slaveAddr != I2C_MEMORY_ADDR_RD))
    {
        CyU3PDebugPrint (4, "I2C Slave address is not valid!\n");
        return 1;
    }

    preamble.buffer[0] = slaveAddr & I2C_SLAVEADDR_MASK;        /*  Mask out the transfer type bit. */
    preamble.buffer[1] = highAddr;
    preamble.buffer[2] = lowAddr;
    preamble.buffer[3] = slaveAddr;
    preamble.length    = 4;
    preamble.ctrlMask  = 0x0004;                                /*  Send start bit after third byte of preamble. */

    apiRetStatus = CyU3PI2cReceiveBytes (&preamble, buf, 2, 0);
    SensorI2CAccessDelay (apiRetStatus);

    return apiRetStatus;
}

CyU3PReturnStatus_t
SensorRead1B (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t *buf)
{
    CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;
    CyU3PI2cPreamble_t preamble;

    if ((slaveAddr != SENSOR_ADDR_RD) && (slaveAddr != I2C_MEMORY_ADDR_RD))
    {
        CyU3PDebugPrint (4, "I2C Slave address is not valid!\n");
        return 1;
    }

    preamble.buffer[0] = slaveAddr & I2C_SLAVEADDR_MASK;        /*  Mask out the transfer type bit. */
    preamble.buffer[1] = highAddr;
    preamble.buffer[2] = lowAddr;
    preamble.buffer[3] = slaveAddr;
    preamble.length    = 4;
    preamble.ctrlMask  = 0x0004;                                /*  Send start bit after third byte of preamble. */

    apiRetStatus = CyU3PI2cReceiveBytes (&preamble, buf, 1, 0);
    SensorI2CAccessDelay (apiRetStatus);

    return apiRetStatus;
}

CyU3PReturnStatus_t
SensorRead (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t count,
        uint8_t *buf)
{
    CyU3PReturnStatus_t apiRetStatus = CY_U3P_SUCCESS;
    CyU3PI2cPreamble_t preamble;

    /* Validate the parameters. */
    if ((slaveAddr != SENSOR_ADDR_RD) && (slaveAddr != I2C_MEMORY_ADDR_RD))
    {
        CyU3PDebugPrint (4, "I2C Slave address is not valid!\n");
        return 1;
    }
    if ( count > 64 )
    {
        CyU3PDebugPrint (4, "ERROR: SensorWrite count > 64\n");
        return 1;
    }

    preamble.buffer[0] = slaveAddr & I2C_SLAVEADDR_MASK;        /*  Mask out the transfer type bit. */
    preamble.buffer[1] = highAddr;
    preamble.buffer[2] = lowAddr;
    preamble.buffer[3] = slaveAddr;
    preamble.length    = 4;
    preamble.ctrlMask  = 0x0004;                                /*  Send start bit after third byte of preamble. */

    apiRetStatus = CyU3PI2cReceiveBytes (&preamble, buf, count, 0);
    SensorI2CAccessDelay (apiRetStatus);

    return apiRetStatus;
}

/*
 * Reset the MT9M114 sensor using GPIO.
 */
void
SensorReset (
        void)
{
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

/* MT9M114 sensor initialization sequence.
   Adapted from MT9M114-REV2.ini file provided by Aptina.

   Step1_Post_Reset       : Post-Reset Settings
   Step2_PLL_Timing       : PLL and Timing settings
   Step3_Recommended      : Patch, Errata and Sensor optimization Setting
   Step4_APGA             : APGA
   Step5_AWB_CCM          : AWB & CCM
   Step7_PIPE_Preference  : Color Pipe preference settings
   Step8_Features         : Ports, special features etc.
   Select Video Resolution
   SensorChangeConfig     : Update sensor configuration.
*/
void
SensorInit (
        void)
{
	uint8_t buf;

	Step1_ov5640_Reset ();
	if (SensorRead1B (SENSOR_ADDR_RD, 0x31, 0x03, &buf) == CY_U3P_SUCCESS)
	{
		CyU3PDebugPrint (4, "buf:%d\r\n",buf);
	}
	else
	{
		CyU3PDebugPrint (4, "read reg fail\r\n");
	}
}

/*
 * Verify that the sensor can be accessed over the I2C bus from FX3.
 */
uint8_t
SensorI2cBusTest (
        void)
{
    /* The sensor ID register can be read here to verify sensor connectivity. */
    uint8_t buf[2];

    /* Reading sensor ID */
    if (SensorRead2B (SENSOR_ADDR_RD, 0x00, 0x00, buf) == CY_U3P_SUCCESS)
    {
        if ((buf[0] == 0x24) && (buf[1] == 0x81))
        {
            return CY_U3P_SUCCESS;
        }
    }
    return 1;
}

/*
   The procedure is adapted from Aptina's sensor initialization scripts. Please
   refer to the MT9M114 sensor datasheet for details.
 */
void
SensorScaling_VGA (
        void)
{
    uint8_t buf[2];

    /* PLL */
    SensorWrite2B (SENSOR_ADDR_WR, 0x09, 0x8E, 0x10, 0x00);
    buf[0] = 0x01;
    SensorWrite (SENSOR_ADDR_WR, 0xC9, 0x7E, 1, buf);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x80, 0x03, 0x3B);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x82, 0x0D, 0x00);

    /* 720P */
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x84, 0x80, 0x40);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x00, 0x00, 0x04);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x02, 0x00, 0x04);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x04, 0x03, 0xCB);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x06, 0x05, 0x0B);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x08, 0x01, 0x81); /* pixclk config */
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x0A, 0xD4, 0x52); /* pixclk config */
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x0C, 0x00, 0x01);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x0E, 0x00, 0xDB);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x10, 0x05, 0xB2);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x12, 0x03, 0xEF);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x14, 0x06, 0x35);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x16, 0x00, 0x60);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x18, 0x03, 0xC3);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x26, 0x00, 0x20);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x34, 0x00, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x54, 0x00, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x56, 0x00, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x58, 0x05, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x5A, 0x03, 0xC0);
    buf[0] = 0x03;
    SensorWrite (SENSOR_ADDR_WR, 0xC8, 0x5C, 1, buf);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x68, 0x02, 0x80);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x6A, 0x01, 0xE0);
    buf[0] = 0x00;
    SensorWrite (SENSOR_ADDR_WR, 0xC8, 0x78, 1, buf);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x8C, 0x0F, 0xCD);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x8E, 0x0F, 0xCD);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x14, 0x00, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x16, 0x00, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x18, 0x02, 0x7F);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x1A, 0x01, 0xDF);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x1C, 0x00, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x1E, 0x00, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x20, 0x00, 0x7F);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x22, 0x00, 0x5F);

    SensorChangeConfig();
}

/*
   The procedure is adapted from Aptina's sensor initialization scripts. Please
   refer to the MT9M114 sensor datasheet for details.
 */
void
SensorScaling_HD720p_30fps (
        void)
{
    uint8_t buf[2];

    SensorWrite2B (SENSOR_ADDR_WR, 0x09, 0x8E, 0x10, 0x00);

    buf[0] = 0x01;
    SensorWrite (SENSOR_ADDR_WR, 0xC9, 0x7E, 1, buf);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x80, 0x01, 0x20);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x82, 0x07, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x84, 0x80, 0x40);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x00, 0x00, 0x04);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x02, 0x00, 0x04);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x04, 0x03, 0xCB);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x06, 0x05, 0x0B);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x08, 0x02, 0xDC);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x0A, 0x6C, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x0C, 0x00, 0x01);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x0E, 0x00, 0xDB);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x10, 0x05, 0xB3);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x12, 0x03, 0xEE);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x14, 0x06, 0x36);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x16, 0x00, 0x60);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x18, 0x03, 0xC3);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x26, 0x00, 0x20);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x34, 0x00, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x54, 0x00, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x56, 0x00, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x58, 0x05, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x5A, 0x03, 0xC0);

    buf[0] = 0x03;
    SensorWrite (SENSOR_ADDR_WR, 0xC8, 0x5C, 1, buf);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x68, 0x05, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x6A, 0x02, 0xD0);

    buf[0] = 0x00;
    SensorWrite (SENSOR_ADDR_WR, 0xC8, 0x78, 1, buf);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x8C, 0x1E, 0x02);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC8, 0x8E, 0x1E, 0x02);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x14, 0x00, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x16, 0x00, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x18, 0x04, 0xFF);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x1A, 0x02, 0xCF);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x1C, 0x00, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x1E, 0x00, 0x00);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x20, 0x00, 0xFF);
    SensorWrite2B (SENSOR_ADDR_WR, 0xC9, 0x22, 0x00, 0x8F);

    SensorChangeConfig();
}

/*
   The procedure is adapted from Aptina's sensor initialization scripts. Please
   refer to the MT9M114 sensor datasheet for details.
 */
void
SensorChangeConfig (
        void)
{
    uint8_t buf[2];

    /* 1. set next state is change_config */
    buf[0] = 0x28;  /* sys_state_enter_config_change */
    SensorWrite (SENSOR_ADDR_WR, 0xDC, 0x00, 1, buf);

    /* 2. poll command register */
    while (SensorRead2B(SENSOR_ADDR_RD, 0x00, 0x80, buf))
    {
        if (!(buf[1]& 0x02))
            break;
    }

    /* 3. issue command */
    SensorWrite2B (SENSOR_ADDR_WR, 0x00, 0x80, 0x80, 0x02);

    /* 4. waiting for firmware updating new configuration */
    while (SensorRead2B(SENSOR_ADDR_RD, 0x00, 0x80, buf))
    {
        if (!(buf[1]& 0x02))
            break;
    }

    /* 5. error handling */
    SensorRead2B (SENSOR_ADDR_RD, 0x00, 0x80, buf);

    /* 6. refresh command */
    SensorWrite2B (SENSOR_ADDR_WR, 0x00, 0x80, 0x80, 0x04);
    while (SensorRead2B(SENSOR_ADDR_RD, 0x00, 0x80, buf))
    {
        if (!(buf[1]& 0x04))
            break;
    }

    SensorRead2B (SENSOR_ADDR_RD, 0xDC, 0x01, buf);
}

/*
   Get the current brightness setting from the MT9M114 sensor.
 */
uint8_t
SensorGetBrightness (
        void)
{
    uint8_t buf[2];

    SensorRead2B (SENSOR_ADDR_RD, 0xCC, 0x0A, buf);
    return (uint8_t)buf[1];
}

/* TODO #2-2 sensor function to read gain register */
/* Copy the SensorGetBrightness function (right aobve this comment),
 * paste it below this comment and rename it to SensorGetGain.
 * Change the register address accessed to 0xCC12.
 */

/*
   Update the brightness setting for the MT9M114 sensor.
 */
void
SensorSetBrightness (
        uint8_t input)
{
    SensorWrite2B (SENSOR_ADDR_WR, 0xCC, 0x0A, 0x00, input);
}

/* TODO #2-3 sensor function to write gain register */
/* Copy the SensorSetBrightness function (right above),
 * paste it below this comment and rename it to SensorSetGain.
 * Change the register address accessed to 0xCC12.
 */
static void Step1_ov5640_Reset (void)
{
	SensorWrite1B (SENSOR_ADDR_WR, 0x31, 0x03, 0x11);
	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x08, 0x82);
	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x08, 0x42);
	SensorWrite1B (SENSOR_ADDR_WR, 0x31, 0x03, 0x03);
	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x17, 0xff);
	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x18, 0xff);
	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x34, 0x1a);
	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x37, 0x13);
	SensorWrite1B (SENSOR_ADDR_WR, 0x31, 0x08, 0x01);
	SensorWrite1B (SENSOR_ADDR_WR, 0x36, 0x30, 0x36);
	SensorWrite1B (SENSOR_ADDR_WR, 0x36, 0x31, 0x0e);
	SensorWrite1B (SENSOR_ADDR_WR, 0x36, 0x32, 0xe2);
	SensorWrite1B (SENSOR_ADDR_WR, 0x36, 0x33, 0x12);
	SensorWrite1B (SENSOR_ADDR_WR, 0x36, 0x21, 0xe0);
	SensorWrite1B (SENSOR_ADDR_WR, 0x37, 0x04, 0xa0);
	SensorWrite1B (SENSOR_ADDR_WR, 0x37, 0x03, 0x5a);
	SensorWrite1B (SENSOR_ADDR_WR, 0x37, 0x15, 0x78);
	SensorWrite1B (SENSOR_ADDR_WR, 0x37, 0x17, 0x01);
	SensorWrite1B (SENSOR_ADDR_WR, 0x37, 0x0b, 0x60);
	SensorWrite1B (SENSOR_ADDR_WR, 0x37, 0x05, 0x1a);
	SensorWrite1B (SENSOR_ADDR_WR, 0x39, 0x05, 0x02);
	SensorWrite1B (SENSOR_ADDR_WR, 0x39, 0x06, 0x10);
	SensorWrite1B (SENSOR_ADDR_WR, 0x39, 0x01, 0x0a);
	SensorWrite1B (SENSOR_ADDR_WR, 0x37, 0x31, 0x12);
	SensorWrite1B (SENSOR_ADDR_WR, 0x36, 0x00, 0x08);
	SensorWrite1B (SENSOR_ADDR_WR, 0x36, 0x01, 0x33);
	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x2d, 0x60);
	SensorWrite1B (SENSOR_ADDR_WR, 0x36, 0x20, 0x52);
	SensorWrite1B (SENSOR_ADDR_WR, 0x37, 0x1b, 0x20);
	SensorWrite1B (SENSOR_ADDR_WR, 0x47, 0x1c, 0x50);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3a, 0x13, 0x43);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3a, 0x18, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3a, 0x19, 0xf8);
	SensorWrite1B (SENSOR_ADDR_WR, 0x36, 0x35, 0x13);
	SensorWrite1B (SENSOR_ADDR_WR, 0x36, 0x36, 0x03);
	SensorWrite1B (SENSOR_ADDR_WR, 0x36, 0x34, 0x40);
	SensorWrite1B (SENSOR_ADDR_WR, 0x36, 0x22, 0x01);

	SensorWrite1B (SENSOR_ADDR_WR, 0x3c, 0x01, 0x34);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3c, 0x04, 0x28);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3c, 0x05, 0x98);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3c, 0x06, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3c, 0x07, 0x08);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3c, 0x08, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3c, 0x09, 0x1c);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3c, 0x0a, 0x9c);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3c, 0x0b, 0x40);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x10, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x11, 0x10);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x12, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x37, 0x08, 0x64);
	SensorWrite1B (SENSOR_ADDR_WR, 0x40, 0x01, 0x02);
	SensorWrite1B (SENSOR_ADDR_WR, 0x40, 0x05, 0x1a);
	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x00, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x04, 0xff);
	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x0e, 0x58);
	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x2e, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x43, 0x00, 0x30);
	SensorWrite1B (SENSOR_ADDR_WR, 0x50, 0x1f, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x44, 0x0e, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x50, 0x00, 0xa7);

	SensorWrite1B (SENSOR_ADDR_WR, 0x3a, 0x0f, 0x30);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3a, 0x10, 0x28);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3a, 0x1b, 0x30);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3a, 0x1e, 0x26);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3a, 0x11, 0x60);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3a, 0x1f, 0x14);

	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x00, 0x23);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x01, 0x14);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x02, 0x0f);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x03, 0x0f);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x04, 0x12);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x05, 0x26);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x06, 0x0c);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x07, 0x08);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x08, 0x05);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x09, 0x05);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x0a, 0x08);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x0b, 0x0d);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x0c, 0x08);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x0d, 0x03);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x0e, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x0f, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x10, 0x03);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x11, 0x09);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x12, 0x07);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x13, 0x03);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x14, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x15, 0x01);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x16, 0x03);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x17, 0x08);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x18, 0x0d);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x19, 0x08);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x1a, 0x05);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x1b, 0x06);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x1c, 0x08);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x1d, 0x0e);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x1e, 0x29);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x1f, 0x17);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x20, 0x11);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x21, 0x11);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x22, 0x15);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x23, 0x28);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x24, 0x46);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x25, 0x26);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x26, 0x08);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x27, 0x26);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x28, 0x64);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x29, 0x26);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x2a, 0x24);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x2b, 0x22);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x2c, 0x24);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x2d, 0x24);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x2e, 0x06);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x2f, 0x22);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x30, 0x40);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x31, 0x42);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x32, 0x24);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x33, 0x26);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x34, 0x24);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x35, 0x22);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x36, 0x22);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x37, 0x26);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x38, 0x44);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x39, 0x24);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x3a, 0x26);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x3b, 0x28);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x3c, 0x42);
	SensorWrite1B (SENSOR_ADDR_WR, 0x58, 0x3d, 0xce);

	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x80, 0xff);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x81, 0xf2);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x82, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x83, 0x14);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x84, 0x25);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x85, 0x24);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x86, 0x09);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x87, 0x09);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x88, 0x09);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x89, 0x75);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x8a, 0x54);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x8b, 0xe0);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x8c, 0xb2);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x8d, 0x42);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x8e, 0x3d);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x8f, 0x56);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x90, 0x46);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x91, 0xf8);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x92, 0x04);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x93, 0x70);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x94, 0xf0);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x95, 0xf0);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x96, 0x03);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x97, 0x01);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x98, 0x04);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x99, 0x12);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x9a, 0x04);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x9b, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x9c, 0x06);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x9d, 0x82);
	SensorWrite1B (SENSOR_ADDR_WR, 0x51, 0x9e, 0x38);

	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x80, 0x01);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x81, 0x08);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x82, 0x14);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x83, 0x28);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x84, 0x51);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x85, 0x65);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x86, 0x71);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x87, 0x7d);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x88, 0x87);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x89, 0x91);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x8a, 0x9a);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x8b, 0xaa);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x8c, 0xb8);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x8d, 0xcd);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x8e, 0xdd);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x8f, 0xea);
	SensorWrite1B (SENSOR_ADDR_WR, 0x54, 0x90, 0x1d);

	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x81, 0x1e);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x82, 0x5b);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x83, 0x08);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x84, 0x0a);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x85, 0x7e);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x86, 0x88);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x87, 0x7c);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x88, 0x6c);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x89, 0x10);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x8a, 0x01);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x8b, 0x98);

	SensorWrite1B (SENSOR_ADDR_WR, 0x55, 0x80, 0x06);
	SensorWrite1B (SENSOR_ADDR_WR, 0x55, 0x83, 0x40);
	SensorWrite1B (SENSOR_ADDR_WR, 0x55, 0x84, 0x10);
	SensorWrite1B (SENSOR_ADDR_WR, 0x55, 0x89, 0x10);
	SensorWrite1B (SENSOR_ADDR_WR, 0x55, 0x8a, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x55, 0x8b, 0xf8);
	SensorWrite1B (SENSOR_ADDR_WR, 0x50, 0x1d, 0x40);

	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x00, 0x08);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x01, 0x30);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x02, 0x10);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x03, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x04, 0x08);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x05, 0x30);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x06, 0x08);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x07, 0x16);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x09, 0x08);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x0a, 0x30);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x0b, 0x04);
	SensorWrite1B (SENSOR_ADDR_WR, 0x53, 0x0c, 0x06);
	SensorWrite1B (SENSOR_ADDR_WR, 0x50, 0x25, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x08, 0x02);

	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x35, 0x21);
	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x36, 0x69);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3c, 0x07, 0x07);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x20, 0x47);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x21, 0x01);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x14, 0x31);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x15, 0x31);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x00, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x01, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x02, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x03, 0xfa);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x04, 0x0a);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x05, 0x3f);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x06, 0x06);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x07, 0xa9);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x08, 0x05);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x09, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x0a, 0x02);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x0b, 0xd0);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x0c, 0x07);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x0d, 0x64);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x0e, 0x02);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x0f, 0xe4);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x13, 0x04);
	SensorWrite1B (SENSOR_ADDR_WR, 0x36, 0x18, 0x00);
	SensorWrite1B (SENSOR_ADDR_WR, 0x36, 0x12, 0x29);
	SensorWrite1B (SENSOR_ADDR_WR, 0x37, 0x09, 0x52);
	SensorWrite1B (SENSOR_ADDR_WR, 0x37, 0x0c, 0x03);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3a, 0x02, 0x02);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3a, 0x03, 0xe0);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3a, 0x14, 0x02);
	SensorWrite1B (SENSOR_ADDR_WR, 0x3a, 0x15, 0xe0);
	SensorWrite1B (SENSOR_ADDR_WR, 0x40, 0x04, 0x02);
	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x02, 0x1c);
	SensorWrite1B (SENSOR_ADDR_WR, 0x30, 0x06, 0xc3);
	SensorWrite1B (SENSOR_ADDR_WR, 0x47, 0x13, 0x03);
	SensorWrite1B (SENSOR_ADDR_WR, 0x44, 0x07, 0x04);
	SensorWrite1B (SENSOR_ADDR_WR, 0x46, 0x0b, 0x37);
	SensorWrite1B (SENSOR_ADDR_WR, 0x46, 0x0c, 0x20);
	SensorWrite1B (SENSOR_ADDR_WR, 0x48, 0x37, 0x16);
	SensorWrite1B (SENSOR_ADDR_WR, 0x38, 0x24, 0x04);
	SensorWrite1B (SENSOR_ADDR_WR, 0x50, 0x01, 0x83);
	SensorWrite1B (SENSOR_ADDR_WR, 0x35, 0x03, 0x00);

	SensorWrite1B (SENSOR_ADDR_WR, 0x47, 0x40, 0x21);
}


