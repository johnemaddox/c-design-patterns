/*H************************************************************
 *
 * Interface
 *
 *  For easy switching between HW or simulated HW for testing.
 *
 * @author John E Maddox
 *
 * @version 2.0.0
 *
 *************************************************************H*/

#ifndef INTF_H
#define INTF_H

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

typedef enum
{
    INTF_OK,
    INTF_ERR
} intf_flag_t;

typedef struct
{
    intf_flag_t intf_flag;
    cam_state_t cam_state;
} intf_state_t;

// Forward declare interface instance struct
typedef struct intf_t intf_t;

// Operations struct (stored in Flash/ROM by using the const keyword in intf_t below)
typedef struct
{
    intf_state_t (*init)(intf_t *self);
    intf_state_t (*capture)(intf_t *self, uint32_t num_of_captures);
    intf_state_t (*start)(intf_t *self);
    intf_state_t (*stop)(intf_t *self);
} intf_ops_t;

// Interface instance struct (stored in RAM)
struct intf_t
{
    const intf_ops_t *ops;
    void             *context; // Pointer to driver-specific runtime state/context
};

// Safe interface wrappers to handle null checks centrally
intf_state_t intf_init(intf_t *self);
intf_state_t intf_capture(intf_t *self, uint32_t num_of_captures);
intf_state_t intf_start(intf_t *self);
intf_state_t intf_stop(intf_t *self);

#endif // INTF_H
