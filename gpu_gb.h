#ifndef GPU_GB_H_
#define GPU_GB_H_

#include "SDL/SDL.h"

void gpuInit();
void gpuStep(int cycles, SDL_Surface* screen);

#endif
