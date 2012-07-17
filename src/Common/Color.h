#ifndef _GB_COLOR_H_
#define _GB_COLOR_H_

#include <stdint.h>

/**
 * Represents a 32-bit color.
 */
struct Color
{
    uint32_t R;     //!< Red component.
    uint32_t G;     //!< Green component.
    uint32_t B;     //!< Blue component.
    uint32_t A;     //!< Alpha component.

    Color(uint32_t r, uint32_t g, uint32_t b);
    Color(uint32_t r, uint32_t g, uint32_t b, uint32_t a);
    Color();
    
    /**
     * Gets the 32-bit integer representing this color.
     */
    uint32_t getColor();
};

#endif
