#ifndef CAMERA_ONE_H
#define CAMERA_ONE_H

#include "intf.h"

// these match the interface template in cam_intf_h
intf_state_t cam1_init(void);
intf_state_t cam1_capture(int num_of_captures);
intf_state_t cam1_start(void);
intf_state_t cam1_stop(void);

#endif // CAMERA_ONE_H
