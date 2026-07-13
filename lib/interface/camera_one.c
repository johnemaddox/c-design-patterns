#include "camera_one.h"

intf_state_t cam1_init(void)
{
    intf_state_t rtn;

    rtn.intf_flag      = INTF_OK;
    rtn.cam_state.flag = CAM_ONE_INIT;

    return rtn;
}

intf_state_t cam1_capture(int num_of_captures)
{
    intf_state_t rtn;

    rtn.intf_flag       = INTF_OK;
    rtn.cam_state.flag  = CAM_ONE_CAPTURE;
    rtn.cam_state.extra = num_of_captures;

    return rtn;
}

intf_state_t cam1_start(void)
{
    intf_state_t rtn;

    rtn.intf_flag      = INTF_OK;
    rtn.cam_state.flag = CAM_ONE_START;

    return rtn;
}

intf_state_t cam1_stop(void)
{
    intf_state_t rtn;

    rtn.intf_flag      = INTF_OK;
    rtn.cam_state.flag = CAM_ONE_STOP;

    return rtn;
}
