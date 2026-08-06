#ifndef CAMERA_HW_H
#define CAMERA_HW_H

#include "camera_intf.h"

// Inject operations and context for physical camera hardware
intf_flag_t camera_hw_create(intf_handle_t *self);

#endif // CAMERA_HW_H
