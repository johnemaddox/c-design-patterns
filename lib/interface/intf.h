/*H************************************************************
 *
 * Generic Hardware Interface
 *
 *  Provides a generic mechanism for polymorphism (virtual method tables)
 *  in C for embedded systems. This enables easy switching between physical
 *  hardware drivers and simulated models for unit testing.
 *
 *  The design pattern completely decouples the interface library from
 *  concrete devices. Device classes (e.g. Cameras, Motors) define their
 *  own custom operations tables and safe wrapper functions, casting
 *  the generic void* pointer to their respective operations structure.
 *
 *  To optimize memory footprint:
 *   - The operations structure (containing callback pointers) should be
 *     declared const and placed in Flash/ROM.
 *   - The runtime context (containing private device states) is allocated
 *     by the driver in RAM and pointed to by the context pointer.
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
