#include "LcdComponent.h"

LcdComponent::LcdComponent(Memory* mem, uint32_t* pixels)
{
    memory = mem;
    ioPorts = memory->getIOMemory();
    lcdPixels = pixels;
}

void LcdComponent::setPixel(uint32_t* pixels, int x, int y, Color color)
{
    pixels[y * 160 + x] = color.getColor();
}

static Color colorCodeMap[] =
{
    Color(255, 255, 255),
    Color(192, 192, 192),
    Color(96, 96, 96),
    Color(0, 0, 0, 0)
};

Color LcdComponent::getColor(int colorCode)
{
    return colorCodeMap[colorCode];
}