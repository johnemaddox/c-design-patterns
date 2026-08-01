#ifndef SENSOR_SIM_H
#define SENSOR_SIM_H

#include "sensor_intf.h"

// Inject operations and context for simulated sensor hardware
intf_flag_t sensor_sim_create(intf_t *self);

#endif // SENSOR_SIM_H
