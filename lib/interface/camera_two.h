#ifndef CAMERA_TWO_H
#define CAMERA_TWO_H

#include "intf.h"

// these match the interface template in cam_intf_h
intf_state_t cam2_init(void);
intf_state_t cam2_capture(int num_of_captures);
intf_state_t cam2_start(void);
intf_state_t cam2_stop(void);

#endif // CAMERA_TWO_H
