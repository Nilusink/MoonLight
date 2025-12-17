#pragma once
#include <Adafruit_NeoPixel.h>
#include "drivers/ir_remote/protocols.hpp"


/**
 * @brief convert to single 32bit rgb value
 * 
 */
constexpr uint32_t rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

namespace actors
{
    typedef uint32_t color_t;

    using namespace ir_protocols;
    inline color_t button_to_color(SmallRemote::Buttons b)
    {
        switch (b)
        {
            case SmallRemote::Buttons::Red:   return rgb(255, 0, 0);
            case SmallRemote::Buttons::R1:    return rgb(255, 40, 0);
            case SmallRemote::Buttons::R2:    return rgb(255, 0, 0);
            case SmallRemote::Buttons::R3:    return rgb(255, 70, 0);
            case SmallRemote::Buttons::R4:    return rgb(255, 80, 10);

            case SmallRemote::Buttons::Green: return rgb(0, 255, 0);
            case SmallRemote::Buttons::G1:    return rgb(0, 255, 90);
            case SmallRemote::Buttons::G2:    return rgb(40, 255, 40);
            case SmallRemote::Buttons::G3:    return rgb(0, 180, 0);
            case SmallRemote::Buttons::G4:    return rgb(0, 220, 120);

            case SmallRemote::Buttons::Blue:  return rgb(0, 0, 255);
            case SmallRemote::Buttons::BL1:   return rgb(0, 0, 255);
            case SmallRemote::Buttons::BL2:   return rgb(0, 0, 255);
            case SmallRemote::Buttons::BL3:   return rgb(0, 0, 255);
            case SmallRemote::Buttons::BL4:   return rgb(0, 0, 255);

            case SmallRemote::Buttons::White: return rgb(255, 255, 255);
            default:                          return 0; // invalid
        }
    }
}
