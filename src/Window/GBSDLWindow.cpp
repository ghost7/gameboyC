#include "GBSDLWindow.h"

bool GBSDLWindow::init(CpuBase *cpu)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        error = "SDL could not initialize.";
        return false;
    }
    
    // Game Boy screen is 160x144. Using 32 bit color for now. 
    // Does this have to change?
    screen = SDL_SetVideoMode(160, 144, 32, SDL_HWSURFACE);
    if (screen == NULL)
    {
        SDL_Quit();
        error = "SDL_SetVideoMode failed.";
        return false;
    }

    gbCpu = cpu;
    initialized = true;
    return true;
}

void GBSDLWindow::loop()
{
    bool running = true;
    SDL_Event event;

    while (running)
    {
        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    // TODO Handle key presses.
                    break;
            }
        }
        // int cycles = gbCpu->step();
        // TODO advance LCD by "cycles"
        // TODO SDL_Flip(), only when necessary, have the LCD tell
        // when it is time to update the screen.
    }
}

std::string GBSDLWindow::getErrorMessage()
{
   return error; 
}

GBSDLWindow::~GBSDLWindow()
{
    if (initialized)
    {
        cleanUp();
    }
}

void GBSDLWindow::cleanUp()
{
    SDL_FreeSurface(screen);
    
    SDL_Quit();
}
