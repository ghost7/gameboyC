#include "Color.h"

Color::Color()
{
    *this = Color(0, 0, 0, 0);
}

Color::Color(uint32_t r, uint32_t g, uint32_t b)
{
    *this = Color(r, g, b, 255);
}

Color::Color(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
    R = r;
    G = g;
    B = b;
    a = a;
}

uint32_t Color::getColor()
{
    return (A << 24) | (R << 16) | (G << 8) | B;
}
