#include "intf.h"
#include "camera_one.h"
#include "camera_two.h"

intf_flag_t intf_select(intf_t *p_intf, intf_type_t hw)
{
    switch(hw)
    {
        case INTF_CAM_ONE:
            p_intf->init    = &cam1_init;
            p_intf->capture = &cam1_capture;
            p_intf->start   = &cam1_start;
            p_intf->stop    = &cam1_stop;
            break;

        case INTF_CAM_TWO:
            p_intf->init    = &cam2_init; 
            p_intf->capture = &cam2_capture;
            p_intf->start   = &cam2_start;
            p_intf->stop    = &cam2_stop;
            break;

        default:
            return INTF_ERR;
    }

    return INTF_OK;
}
