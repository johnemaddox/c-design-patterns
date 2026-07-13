#include "camera_two.h"
#include <stdio.h>

intf_state_t cam2_init(void)
{
    intf_state_t rtn;

    rtn.intf_flag = INTF_OK;
    rtn.cam_state.flag  = CAM_TWO_INIT;

    return rtn;
}

intf_state_t cam2_capture(int num_of_captures)
{
    intf_state_t rtn;

    rtn.intf_flag       = INTF_OK;
    rtn.cam_state.flag  = CAM_TWO_CAPTURE;
    rtn.cam_state.extra = num_of_captures;

    return rtn;
}

intf_state_t cam2_start(void)
{
    intf_state_t rtn;

    rtn.intf_flag       = INTF_OK;
    rtn.cam_state.flag  = CAM_TWO_START;

    return rtn;
}

intf_state_t cam2_stop(void)
{
    intf_state_t rtn;

    rtn.intf_flag       = INTF_OK;
    rtn.cam_state.flag  = CAM_TWO_STOP;

    return rtn;
}
