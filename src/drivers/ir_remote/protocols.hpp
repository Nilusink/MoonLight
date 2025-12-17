/**
 * @file protocols.hpp
 * @author Nilusink
 * @brief IR-remote button definitions
 * @version 0.1
 * @date 2025-12-14
 * 
 * @copyright Copyright (c) Nilusink 2025
 * 
 */
#pragma once
#include <Arduino.h>


namespace ir_protocols
{
    struct SmallRemote {
        static constexpr const char* name = "SMALL_REMOTE";
        static constexpr const char* protocol = "NEC";

        enum class Buttons : uint32_t {
            On       = 0xFFE01F,
            Off      = 0xFF609F,
            Brighter = 0xFFA05F,
            Dimmer   = 0xFF20DF,

            Red      = 0xFF906F,
            Green    = 0xFF10EF,
            Blue     = 0xFF50AF,
            White    = 0xFFD02F,

            R1       = 0xFFB04F,
            G1       = 0xFF30CF,
            BL1      = 0xFF708F,
            Action1  = 0xFFF00F,

            R2       = 0xFFA857,
            G2       = 0xFF28D7,
            BL2      = 0xFF6897,
            Action2  = 0xFFE817,

            R3       = 0xFF9867,
            G3       = 0xFF18E7,
            BL3      = 0xFF58A7,
            Action3  = 0xFFD827,

            R4       = 0xFF8877,
            G4       = 0xFF08F7,
            BL4      = 0xFF48B7,
            Action4  = 0xFFC837,

            repeat   = 0xFFFFFFFF
        };
    };
}
