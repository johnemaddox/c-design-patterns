/*H************************************************************
 *
 * Camera Common State Flags
 *
 *  Common state flags for error handling. This will need to be
 *  modified for practical purposes. The current state is useful
 *  for testing purposes. Different camera manufactures will likely
 *  have different error handling flags. These flags can be mapped
 *  to the flags added to cam_flag_t. Extra was added as a way to
 *  pass other values that may be needed for certain calls.
 *
 * @author John E Maddox
 *
 * @version 1.0.0
 *
*************************************************************H*/

#ifndef CAMERA_H
#define CAMERA_H

#include <stdint.h>

// put in actual status flags
typedef enum
{
    CAM_ONE_INIT,
    CAM_ONE_CAPTURE,
    CAM_ONE_START,
    CAM_ONE_STOP,

    CAM_TWO_INIT,
    CAM_TWO_CAPTURE,
    CAM_TWO_START,
    CAM_TWO_STOP
} cam_flag_t;

typedef struct
{
    cam_flag_t flag;
    uint32_t   extra;
} cam_state_t;

#endif // CAMERA_H
