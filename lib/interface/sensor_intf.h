/*H************************************************************
 *
 * Sensor Interface
 *
 *  A concrete sensor device interface built on top of the generic 
 *  hardware interface library (intf.h). Enables runtime switching
 *  between physical sensor hardware and simulation drivers.
 *
 *  Usage Example:
 *   1. Declare an interface handle: intf_t my_sensor;
 *   2. Instantiate the driver (e.g. HW or SIM):
 *      sensor_hw_create(&my_sensor);
 *   3. Access operations through safe wrappers:
 *      sensor_state_t st = sensor_init(&my_sensor);
 *      float value;
 *      sensor_read(&my_sensor, &value);
 *
 * @author John E Maddox
 *
 * @version 2.0.0
 *
 * *************************************************************H*/
#ifndef SENSOR_INTF_H
#define SENSOR_INTF_H

#include "intf.h"
#include <stdint.h>

typedef enum
{
    SENS_INIT,
    SENS_READ,
    SENS_CALIBRATE,
    SENS_ENABLE,
    SENS_DISABLE
} sens_flag_t;

typedef struct
{
    sens_flag_t flag;
    float       value; // Payload (e.g. read temperature/pressure value, or progress)
} sens_state_t;

typedef struct
{
    intf_flag_t  intf_flag;
    sens_state_t sens_state;
} sensor_state_t;

// Sensor operations struct
typedef struct
{
    sensor_state_t (*init)(intf_t *self);
    sensor_state_t (*read)(intf_t *self, float *value);
    sensor_state_t (*calibrate)(intf_t *self);
    sensor_state_t (*enable)(intf_t *self);
    sensor_state_t (*disable)(intf_t *self);
} sensor_ops_t;

// Safe interface wrappers to handle null checks centrally
sensor_state_t sensor_init(intf_t *self);
sensor_state_t sensor_read(intf_t *self, float *value);
sensor_state_t sensor_calibrate(intf_t *self);
sensor_state_t sensor_enable(intf_t *self);
sensor_state_t sensor_disable(intf_t *self);

#endif // SENSOR_INTF_H
