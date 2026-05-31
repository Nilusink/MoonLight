#include <Adafruit_NeoPixel.h>
#include "drivers/ir_remote/protocols.hpp"
#include "drivers/animations.hpp"
#include "colors.hpp"


#define MAX_PIXEL_N 64


namespace actors
{
    class LEDs
    {
    private:
        Adafruit_NeoPixel strip_;

        uint16_t num_leds;
        uint8_t ww_pin;
        bool ww_status = false;
        bool segment_control = false;
        uint8_t current_segment = 0;

        // pixel color data
        color_t pixel_color[MAX_PIXEL_N] = {0};
        uint8_t pixel_brightness[MAX_PIXEL_N] = {0};

        color_t saved_colors[4] = {0};
        const uint8_t segment_positions[4][2] = {
            {0,  5},
            {0,  7},
            {8,  12},
            {13, 34}
        };

    protected:
        void set_segment_control(bool state);
        void toggle_segment_control();
        // void _reload();
        void fill(color_t c, bool keep_brightness = false);
        void setPixelColor(uint16_t n, color_t c);
        void setBrightness(uint8_t b);
        void re_render();


    public:
        enum SEGMENTS : uint8_t
        {
            TV      = 0,
            DESK    = 1,
            HEATER  = 2,
            BED     = 3
        };
        SEGMENTS segment_from_int(uint8_t v)
        {
            if (v > BED)
                return BED;
            return static_cast<SEGMENTS>(v);
        }

        LEDs(uint16_t numLeds, uint8_t pin, uint8_t ww_pin);

        void begin();

        void set_brightness(uint8_t brightness, bool show = true);
        void set_pixel_brightness(uint16_t pixel, uint8_t brightness);
        void brightness_up();
        void brightness_down();
        /**
         * @brief animate brightnes from start to end over time. SAVES COLORS!
         * 
         * @param start starting brightness
         * @param end end brightness
         * @param time time to animate
         * @param anim fade animation, must take and return double (0-1)
         * @param resolution steps for animation
         */
        void anim_brightness(
            uint8_t start,
            uint8_t end,
            double time,
            anim_t(anim) = &animations::linear,
            double resolution=100
        );
        void anim_brightness(
            uint8_t start,
            uint8_t end,
            double time,
            double resolution
        );

        /**
         * @brief scale pixel brightness. IMPORTANT: uses last set pixel brightnes and scales it
         * 
         * @param start 
         * @param end 
         * @param time 
         * @param resolution 
         */
        void anim_pixel_brightness(
            double start,
            double end,
            double time,
            anim_t(anim) = &animations::linear,
            double resolution = 100,
            bool override_brightness = true
        );

        void toggle_ww();

        /**
         * @brief safe colors to re-apply later
         * 
         */
        void save_segment_colors();
        void reload_segment_colors(bool show=true);

        void set_all(color_t color, bool show=true);
        void set_pixels(uint8_t start, uint8_t end, color_t color, bool show=true);
        void clear();

        void set_segment_color(SEGMENTS segment, color_t color, bool show=true);
        void set_segment_color(uint8_t segment, color_t color, bool show=true);

        void set_segment_brightness(SEGMENTS segment, uint8_t brightness, bool show=true);
        void set_segment_brightness(uint8_t segment, uint8_t brightness, bool show=true);

        void render_segment(SEGMENTS segment);

        void set_temperature(float tempC);

        void set_remote(ir_protocols::SmallRemote::Buttons button);

        void void_fade();

        void on();
        void off();

        color_t Color(uint8_t r, uint8_t g, uint8_t b);
    };
}