/**
 * @file animations.hpp
 * @author Nilusink
 * @brief basic animation controller
 * @version 0.1
 * @date 2025-12-17
 * 
 * @copyright Copyright (c) Nilusink 2025
 * 
 */
#pragma once
#include <Arduino.h>


#define anim_t(name) double (*name)(double x)


namespace animations
{
    double linear(double x);
    namespace Fade
    {
        double InOut(double x);
        double In(double x);
        double Out(double x);
    } // namespace Fade
    
}
