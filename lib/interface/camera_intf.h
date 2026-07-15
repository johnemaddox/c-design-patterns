/*H************************************************************
 *
 * Camera Interface
 *
 *  For easy switching cameras between HW or sim HW for testing.
 *
 * @author John E Maddox
 *
 * @version 2.0.0
 *
 *************************************************************H*/
#ifndef CAMERA_INTF_H
#define CAMERA_INTF_H

#include "intf.h"
#include <stdint.h>

typedef enum
{
    CAM_INIT,
    CAM_CAPTURE,
    CAM_START,
    CAM_STOP
} cam_flag_t;

typedef struct
{
    cam_flag_t flag;
    uint32_t   extra; // Payload / metadata associated with the event (e.g. number of captures)
} cam_state_t;

typedef struct
{
    intf_flag_t intf_flag;
    cam_state_t cam_state;
} camera_state_t;

// Camera operations struct
typedef struct
{
    camera_state_t (*init)(intf_t *self);
    camera_state_t (*capture)(intf_t *self, uint32_t num_of_captures);
    camera_state_t (*start)(intf_t *self);
    camera_state_t (*stop)(intf_t *self);
} camera_ops_t;

// Safe interface wrappers to handle null checks centrally
camera_state_t camera_init(intf_t *self);
camera_state_t camera_capture(intf_t *self, uint32_t num_of_captures);
camera_state_t camera_start(intf_t *self);
camera_state_t camera_stop(intf_t *self);

#endif // CAMERA_INTF_H
