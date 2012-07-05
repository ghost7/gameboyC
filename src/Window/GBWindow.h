#ifndef _GB_WINDOW_H_
#define _GB_WINDOW_H_

#include <string>
#include "../Cpu/CpuBase.h"

/**
 * Window that will be displatyed for a Gameboy Game.
 */
class GBWindow
{
public:
    /**
     * Initialize the window with a cpu.
     */
    virtual bool init(CpuBase *cpu) = 0;

    /**
     * Window loop. Keeps the game running until the game exits, or 
     * the user exits.
     */
    virtual void loop() = 0;

    /**
     * Trying to avoid exceptions, so any error that occurs, will have a
     * a message. This function gets the message as a string.
     */
    virtual std::string getErrorMessage() = 0;
};

#endif
