#include "leds.hpp"


constexpr inline uint32_t scaleColor(uint32_t color, uint8_t brightness)
{
    // convert to seperate rgb variables
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8)  & 0xFF;
    uint8_t b = color & 0xFF;

    // scale colors
    r = (r * brightness) / 255;
    g = (g * brightness) / 255;
    b = (b * brightness) / 255;

  return rgb(r, g, b);
}


using namespace actors;
void LEDs::setPixelColor(uint16_t n, color_t c)
{
    pixel_color[n] = c;
    strip_.setPixelColor(n, scaleColor(c, pixel_brightness[n]));
}

void LEDs::fill(color_t c, bool keep_brightness)
{
    // set all pixels to same color
    for (int i = 0; i < num_leds; i++)
    {
        pixel_color[i] = c;

        if (keep_brightness)
            strip_.setPixelColor(i, scaleColor(c, pixel_brightness[i]));
    }

    if (!keep_brightness)
        strip_.fill(c);
}

void LEDs::setBrightness(uint8_t b)
{
    for (int i = 0; i < num_leds; i++)
    {
        pixel_brightness[i] = b;
    }
}
void LEDs::re_render()
{
    for (int i = 0; i < num_leds; i++)
    {
        strip_.setPixelColor(i, scaleColor(pixel_color[i], pixel_brightness[i]));
    }
    strip_.show();
}


LEDs::LEDs(uint16_t numLeds, uint8_t pin, uint8_t ww_pin)
    : strip_(numLeds, pin, NEO_RGB + NEO_KHZ800), num_leds(numLeds)
{
    // catch out-of-bounds
    if (numLeds > MAX_PIXEL_N)
    {
        Serial.printf(
            "spezified numLeds of %d exceeds predefined limit of %d\n",
            numLeds,
            MAX_PIXEL_N
        );

        for(;;);
    }
}

void LEDs::begin()
{
    analogWriteFreq(20000);
    pinMode(ww_pin, OUTPUT);
    strip_.begin();
    strip_.show(); // clear LEDs
    strip_.setBrightness(255);
}

void LEDs::set_brightness(uint8_t brightness, bool show)
{
    setBrightness(constrain(brightness, 0, 255));
    if (show)
        strip_.show();
}

void LEDs::set_pixel_brightness(uint16_t pixel, uint8_t brightness)
{
    pixel_brightness[pixel] = brightness;
}

void LEDs::brightness_up()
{
    for (int i = 0; i < num_leds; i ++)
    {
        set_pixel_brightness(i, constrain((int)pixel_brightness[i] + 8, 0, 255));
        Serial.println(constrain((int)pixel_brightness[i] + 8, 1, 255));
    }
}

void LEDs::brightness_down()
{
    for (int i = 0; i < num_leds; i ++)
    {
        set_pixel_brightness(i, constrain((int)pixel_brightness[i] - 8, 1, 255));
        Serial.println(constrain((int)pixel_brightness[i] - 8, 1, 255));
    }
}

void LEDs::anim_brightness(
    uint8_t start,
    uint8_t end,
    double time,
    anim_t(anim),
    double resolution
)
{
    if (end == start)
        return;

    bool is_reversed = end < start;
    uint16_t d = time / resolution;
    uint8_t anim_range = abs((int)end-start);

    double anim_progress, brightness;
    for (int i = 0; i < resolution; i++)
    {
        // fade in-out
        anim_progress = anim((double)i/resolution) * anim_range;

        if (is_reversed)
        {
            brightness = start - anim_progress;
        }
        else
        {
            brightness = start + anim_progress;
        }

        set_brightness((uint8_t)brightness);
        strip_.show();

        delay(d);
    }
    set_brightness(end);
    strip_.show();
}
void LEDs::anim_brightness(
    uint8_t start,
    uint8_t end,
    double time,
    double resolution
)
{
    anim_brightness(start, end, time, &animations::linear, resolution);
}

void LEDs::anim_pixel_brightness(
    double start,
    double end,
    double time,
    anim_t(anim),
    double resolution,
    bool override_brightness
)
{
    if (end == start)
        return;

    if (0 > start || start > 1)
    {
        Serial.printf("apb start oor: %f\n", start);
        start = constrain(start, 0, 1);
    }
    if (0 > end || end > 1)
    {
        Serial.printf("apb end oor: %f\n", end);
        end = constrain(end, 0, 1);
    }

    bool is_reversed = end < start;
    uint16_t d = time / resolution;
    double anim_range = abs(end-start);

    double anim_progress, brightness;
    for (int i = 0; i < resolution; i++)
    {
        // fade in-out
        anim_progress = anim((double)i/resolution) * anim_range;

        if (is_reversed)
        {
            anim_progress = start - anim_progress;
        }
        else
        {
            anim_progress = start + anim_progress;
        }

        // scale each pixel color
        for (int p = 0; p < num_leds; p++)
        {
            brightness = pixel_brightness[p] * anim_progress;
            strip_.setPixelColor(p, scaleColor(pixel_color[p], brightness));
        }
        strip_.show();

        delay(d);
    }
    if (override_brightness)
        set_brightness(end);

    strip_.show();
}


void LEDs::toggle_ww()
{
    ww_status = !ww_status;

    if (ww_status)
    {
        Serial.println("WW ON");
        digitalWrite(ww_pin, HIGH);
        // analogWrite(ww_pin, last_brightness);
    }
    else
    {
        Serial.println("WW OFF");
        digitalWrite(ww_pin, LOW);
        // analogWrite(ww_pin, 0);
    }
}


void LEDs::save_segment_colors()
{
    // save color from first pixel from each segment
    for (int s = 0; s < 4; s++)
    {
        saved_colors[s] = pixel_color[segment_positions[s][1]];
        Serial.printf("segment %d: %X \n", s, saved_colors[s]);
    }
}

void LEDs::reload_segment_colors(bool show)
{
    uint8_t s;
    for (uint8_t ns = 0; ns < 4; ns++)
    {
        s = 3 - ns;
        set_segment_color(s, saved_colors[s], false);
    }

    if (show)
        strip_.show();
}

void LEDs::set_all(uint32_t color, bool show)
{
    fill(color);

    if (show)
        strip_.show();
}

void LEDs::set_pixels(uint8_t start, uint8_t end, uint32_t color, bool show)
{
    for (int i = start; i <= end; i++)
    {
        setPixelColor(i, color);
    }

    if (show)
        strip_.show();
}

void LEDs::set_segment_color(SEGMENTS segment, color_t color, bool show)
{
    for (
        int i = segment_positions[segment][0];
        i <= segment_positions[segment][1];
        i++
    )
    {
        setPixelColor(i, color);
    }
    
    if (show)
        strip_.show();
}
void LEDs::set_segment_color(uint8_t segment, color_t color, bool show)
{
    set_segment_color(segment_from_int(segment), color, show);
}

void LEDs::set_segment_brightness(SEGMENTS segment, uint8_t brightness, bool show)
{
    for (
        int i = segment_positions[segment][0];
        i <= segment_positions[segment][1];
        i++
    )
    {
        set_pixel_brightness(i, brightness);
    }
    
    if (show)
        render_segment(segment);
}
void LEDs::set_segment_brightness(uint8_t segment, uint8_t brightness, bool show)
{
    set_segment_brightness(segment_from_int(segment), brightness, show);
}

void LEDs::render_segment(SEGMENTS segment)
{
    for (
        int i = segment_positions[segment][0];
        i <= segment_positions[segment][1];
        i++
    )
    {
        strip_.setPixelColor(i, scaleColor(pixel_color[i], pixel_brightness[i]));
    }
    strip_.show();
}

void LEDs::clear()
{
    strip_.clear();
}


void LEDs::set_temperature(float tempC)
{
    if (tempC >= 21.0 && tempC <= 23.0)
    {
        set_all(Color(0, 255, 0));
        return;
    }

    tempC = constrain(tempC, 16.0, 30.0);

    // Hue: blue (160°) → red (0°)
    uint16_t hue = map(tempC * 10, 160, 300, 160, 0) * 256;

    // Slightly reduced saturation looks better indoors
    uint8_t sat = 200;
    uint8_t val = 255;

    // Convert HSV → RGB
    uint32_t rawColor = strip_.ColorHSV(hue, sat, val);

    // APPLY GAMMA CORRECTION (this is the missing piece)
    set_all(strip_.gamma32(rawColor));
}


void LEDs::set_remote(ir_protocols::SmallRemote::Buttons button)
{
    bool match_button = false;
    if (segment_control)
    {
        switch (button)
        {
            case ir_protocols::SmallRemote::Buttons::On:
                set_segment_brightness(current_segment, 255);
                break;

            case ir_protocols::SmallRemote::Buttons::Off:
                set_segment_brightness(current_segment, 0);
                break;

            case ir_protocols::SmallRemote::Buttons::Brighter:
                if (current_segment < 3)
                    current_segment++;
                match_button = true;
                break;
        
            case ir_protocols::SmallRemote::Buttons::Dimmer:
                if (current_segment > 0)
                    current_segment--;
                match_button = true;
                break;

            case ir_protocols::SmallRemote::Buttons::Action1:
                set_segment_control(false);
                break;

            default:
                match_button = true;
                break;
        }

        if (match_button)
        {
            // try to match button to color
            color_t btn_color = button_to_color(button);
            if (btn_color)
            {
                saved_colors[current_segment] = btn_color;
            }
            clear();
            set_segment_color(current_segment, saved_colors[current_segment]);
        }
    }
    else
    {
        switch (button)
        {
            case ir_protocols::SmallRemote::Buttons::On:
                on();
                break;

            case ir_protocols::SmallRemote::Buttons::Off:
                off();
                break;

            case ir_protocols::SmallRemote::Buttons::Brighter:
                brightness_up();
                re_render();
                break;

            case ir_protocols::SmallRemote::Buttons::Dimmer:
                brightness_down();
                re_render();
                break;

            case ir_protocols::SmallRemote::Buttons::Action1:
                set_segment_control(true);
                break;

            case ir_protocols::SmallRemote::Buttons::Action3:
                void_fade();
                break;

            default:
                // try to match button to color
                color_t btn_color = button_to_color(button);

                // check if button color is non-zero
                if (btn_color)
                {
                    set_all(btn_color);
                    set_brightness(255);
                }
                save_segment_colors();
                break;
        }
    }
}


void LEDs::on(bool instant)
{
    anim_pixel_brightness(0, 1, instant ? 1 : 500, &animations::Fade::InOut, 100, false);
    save_segment_colors();
}

void LEDs::off(bool instant)
{
    save_segment_colors();
    anim_pixel_brightness(1, 0, instant ? 1 : 500, &animations::Fade::InOut, 100, false);
    strip_.clear();
}


uint32_t LEDs::Color(uint8_t r, uint8_t g, uint8_t b)
{
    return rgb(r, g, b);
}


void LEDs::void_fade()
{
    save_segment_colors();
    for (int fade = -16; fade < num_leds + 16; fade++)
    {
        for (int i = 0; i < num_leds; i++)
        {
            uint8_t b = constrain((i - fade) * 16, 0, 255);
            strip_.setPixelColor(i, scaleColor(pixel_color[i], pixel_brightness[i] * b / 255));
        }
        strip_.show();
        delay(30);
    }
}

void LEDs::set_segment_control(bool state)
{
    segment_control = state;
    Serial.printf("segment control: %d \n", segment_control);

    // enter / exit hooks
    if (segment_control)
    {
        save_segment_colors();
        clear();
        set_segment_color(current_segment, saved_colors[current_segment]);
    }
    else
    {
        reload_segment_colors();
    }
}

void LEDs::toggle_segment_control()
{
    set_segment_control(!segment_control);
}
