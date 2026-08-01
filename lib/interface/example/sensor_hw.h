#ifndef SENSOR_HW_H
#define SENSOR_HW_H

#include "sensor_intf.h"

// Inject operations and context for physical sensor hardware
intf_flag_t sensor_hw_create(intf_t *self);

#endif // SENSOR_HW_H
