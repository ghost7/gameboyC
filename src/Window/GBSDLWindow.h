#ifndef _GB_SDL_WINDOW_H_
#define _GB_SDL_WINDOW_H_

#include "SDL.h"

#include "GBWindow.h"

/**
 * Game Boy window implemented using SDL. SDL will take care
 * of displaying graphics and handling input.
 */
class GBSDLWindow : public ::GBWindow
{
public:
    GBSDLWindow() { initialized = false; }
    ~GBSDLWindow();

    bool init(CpuBase *cpu, LcdInterface *lcd);
    void loop();
    std::string getErrorMessage();
private:
    /**
     * Shut down SDL.
     */
    void cleanUp();

    /* Has the window been initialized? */
    bool initialized;

    /* The SDL screen */
    SDL_Surface *screen;

    /* The Cpu */
    CpuBase *gbCpu;
    
    /* The LCD */
    LcdInterface *gbLcd;

    /* Stores any error. */
    std::string error;
};

#endif
