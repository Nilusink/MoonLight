#include <Adafruit_NeoPixel.h>
#include "drivers/ir_remote/protocols.hpp"


namespace actors
{
    class LEDs
    {
    private:
        Adafruit_NeoPixel strip_;
        uint8_t last_brightness = 20;
        uint32_t last_color = 0;

        uint16_t num_leds;
        uint8_t ww_pin;
        bool ww_status = false;

    public:
        LEDs(uint16_t numLeds, uint8_t pin, uint8_t ww_pin);

        void begin();

        void set_brightness(uint8_t brightness);
        void brightness_up();
        void brightness_down();

        void toggle_ww();

        void set_all(uint32_t color);

        void set_temperature(float tempC);

        void set_remote(ir_protocols::SmallRemote::Buttons button);

        void void_fade();

        void off();

        uint32_t Color(uint8_t r, uint8_t g, uint8_t b);

    };
}