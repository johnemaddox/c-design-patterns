/*H************************************************************
 *
 * Interface
 *
 *  For easy switching between HW or simulated HW for testing.
 *
 *  - Add new HW in intf_type_t.
 *  - Add or modify interface functions in intf_t.
 *
 * @author John E Maddox
 *
 * @version 1.0.0
 *
*************************************************************H*/

#ifndef INTF_H
#define INTF_H

#include "camera.h"

typedef enum
{
    INTF_OK,
    INTF_ERR
} intf_flag_t;

typedef enum
{
    INTF_CAM_ONE,
    INTF_CAM_TWO,
    INTF_CAM_THREE
} intf_type_t;

typedef struct 
{
    intf_flag_t intf_flag;
    cam_state_t cam_state;
} intf_state_t;

// This is a template for all of the interfaces.
typedef struct
{
    intf_state_t (*init)(void);
    intf_state_t (*capture)(int num_of_captures);
    intf_state_t (*start)(void);
    intf_state_t (*stop)(void);
} intf_t;

// set a cam_intf_h struct for the selected camera hardware.
intf_flag_t intf_select(intf_t *p_intf, intf_type_t hw_type);

#endif // INTF_H
