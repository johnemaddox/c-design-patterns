#ifndef CAMERA_SIM_H
#define CAMERA_SIM_H

#include "camera_intf.h"

// Inject operations and context for simulated camera hardware
intf_flag_t camera_sim_create(intf_t *self);

#endif // CAMERA_SIM_H
