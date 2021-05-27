/*
 ## Cypress FX3 Camera Kit header file (sensor.h)
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

/* This file defines the parameters and the interface for the MT9M114 image
   sensor driver.
 */

#ifndef _INCLUDED_SENSOR_H_
#define _INCLUDED_SENSOR_H_

#include <cyu3types.h>


#define I2C_SLAVEADDR_MASK 0xFE         /* Mask to get actual I2C slave address value without direction bit. */

/* GPIO 22 on FX3 is used to reset the Image sensor. */
#define SENSOR_RESET_GPIO 22

/* Function    : SensorWrite2B
   Description : Write two bytes of data to image sensor over I2C interface.
   Parameters  :
                 slaveAddr - I2C slave address for the sensor.
                 highAddr  - High byte of memory address being written to.
                 lowAddr   - Low byte of memory address being written to.
                 highData  - High byte of data to be written.
                 lowData   - Low byte of data to be written.
 */
extern CyU3PReturnStatus_t
SensorWrite2B (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t highData,
        uint8_t lowData);

extern CyU3PReturnStatus_t
SensorWrite1B (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t Data);

/* Function    : SensorWrite
   Description : Write arbitrary amount of data to image sensor over I2C interface.
   Parameters  :
                 slaveAddr - I2C slave address for the sensor.
                 highAddr  - High byte of memory address being written to.
                 lowAddr   - Low byte of memory address being written to.
                 count     - Size of write data in bytes. Limited to a maximum of 64 bytes.
                 buf       - Pointer to buffer containing data.
 */
extern CyU3PReturnStatus_t
SensorWrite (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t count,
        uint8_t *buf);

/* Function    : SensorRead2B
   Description : Read 2 bytes of data from image sensor over I2C interface.
   Parameters  :
                 slaveAddr - I2C slave address for the sensor.
                 highAddr  - High byte of memory address being written to.
                 lowAddr   - Low byte of memory address being written to.
                 buf       - Buffer to be filled with data. MSB goes in byte 0.
 */
extern CyU3PReturnStatus_t
SensorRead2B (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t *buf);

extern CyU3PReturnStatus_t
SensorRead1B (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t *buf);

/* Function    : SensorRead
   Description : Read arbitrary amount of data from image sensor over I2C interface.
   Parameters  :
                 slaveAddr - I2C slave address for the sensor.
                 highAddr  - High byte of memory address being written to.
                 lowAddr   - Low byte of memory address being written to.
                 count     = Size of data to be read in bytes. Limited to a max of 64.
                 buf       - Buffer to be filled with data.
 */
extern CyU3PReturnStatus_t
SensorRead (
        uint8_t slaveAddr,
        uint8_t highAddr,
        uint8_t lowAddr,
        uint8_t count,
        uint8_t *buf);

/* Function    : SensorInit
   Description : Initialize the MT9M114 sensor.
   Parameters  : None
 */
extern void
SensorInit (
        void);

/* Function    : SensorReset
   Description : Reset the MT9M114 image sensor using FX3 GPIO.
   Parameters  : None
 */

extern void
SensorReset (
        void);

/* Function    : SensorChangeConfig
   Description : Update sensor configuration based on selected video parameters.
   Parameters  : None
 */
extern void
SensorChangeConfig (
        void);

// /* Function     : SensorScaling_HD720p_30fps
//    Description  : Configure the MT9M114 sensor for 720p 30 fps video stream.
//    Parameters   : None
//  */
// extern void
// SensorScaling_HD720p_30fps (
//         void);

// /* Function     : SensorScaling_VGA
//    Description  : Configure the MT9M114 sensor for VGA video stream.
//    Parameters   : None
//  */
// extern void
// SensorScaling_VGA (
//         void);

// /* Function    : SensorI2cBusTest
//    Description : Test whether the MT9M114 sensor is connected on the I2C bus.
//    Parameters  : None
//  */
// extern uint8_t
// SensorI2cBusTest (
//         void);

/* TODO #2-4 Add gain control function definitions */
/* Copy the SensorGetBrightness and SensorSetBrightness function definitions from above
 * and paste them below this comment.
 * Rename the functions to SensorGetGain and SensorSetGain respectively.
 */

#endif /* _INCLUDED_SENSOR_H_ */

/*[]*/

