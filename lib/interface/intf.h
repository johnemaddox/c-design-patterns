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
    INTF_OK,
    INTF_ERR
} intf_flag_t;

typedef struct intf_t
{
    const void *ops;
    void       *context; // Pointer to driver-specific runtime state/context
} intf_t;

#endif // INTF_H
