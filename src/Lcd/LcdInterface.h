#ifndef _LCD_INTERFACE_
#define _LCD_INTERFACE_

#include <stdint.h>

/**
 * This defines the LCD screen of the game boy.
 */
class LcdInterface
{
public:
    /**
     * Initialize the LCD with an array of pixels. The LCD will "draw" on these
     * pixels
     *
     * @param pixels 160x144 array of pixels.
     */
    virtual void init(uint32_t* pixels) = 0;

    /**
     * Advance all LCD timings by a given amount of cycles.
     *
     * @paran cycles Number of cycles to advance by.
     */
    virtual void step(int cycles) = 0;
    
    /**
     * Notifies the client code that the LCD is dirty. This means that the LCD 
     * has modified the array of pixels and the screen can be updated.
     *
     * @return True if the screen should be updated, false otherwise.
     */
    virtual bool isDirty() = 0;

    /**
     * After the client has updated the screen, this function should be called
     * to notify the LCD that it is no longer in a dirty state.
     */
    virtual void clean() = 0;
};

#endif
