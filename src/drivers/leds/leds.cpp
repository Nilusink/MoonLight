#include "leds.hpp"


uint32_t scaleColor(uint32_t color, uint8_t brightness) {
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8)  & 0xFF;
  uint8_t b = color & 0xFF;

  r = (r * brightness) / 255;
  g = (g * brightness) / 255;
  b = (b * brightness) / 255;

  return Adafruit_NeoPixel::Color(r, g, b);
}


using namespace actors;
LEDs::LEDs(uint16_t numLeds, uint8_t pin, uint8_t ww_pin)
    : strip_(numLeds, pin, NEO_RGB + NEO_KHZ800), num_leds(numLeds)
{}

void LEDs::begin()
{
    analogWriteFreq(20000);
    pinMode(ww_pin, OUTPUT);
    strip_.begin();
    strip_.show(); // clear LEDs
}

void LEDs::set_brightness(uint8_t brightness)
{
    last_brightness = brightness;
    strip_.setBrightness(constrain(brightness, 0, 255));
    strip_.show();

    // set warm white status
    // if (ww_status)
    // {
    //     analogWrite(ww_pin, last_brightness);
    // }
}
void LEDs::brightness_up()
{
    uint8_t brightness = strip_.getBrightness();
    brightness = constrain((int)brightness + 8, 0, 255);
    set_brightness(brightness);
}
void LEDs::brightness_down()
{
    uint8_t brightness = strip_.getBrightness();
    brightness = constrain((int)brightness - 8, 1, 255);
    set_brightness(brightness);
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

void LEDs::set_all(uint32_t color)
{
    set_brightness(last_brightness);
    last_color = color;
    strip_.fill(color);
    strip_.show();
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
    switch (button)
    {
    case ir_protocols::SmallRemote::Buttons::On:
        set_brightness(last_brightness);
        set_all(last_color);
        break;
    case ir_protocols::SmallRemote::Buttons::Off:
        off();
        break;
    case ir_protocols::SmallRemote::Buttons::Brighter:
        brightness_up();
        break;
    case ir_protocols::SmallRemote::Buttons::Dimmer:
        brightness_down();
        break;
    case ir_protocols::SmallRemote::Buttons::Red:
        set_all(Color(255, 0, 0));
        break;
    case ir_protocols::SmallRemote::Buttons::Green:
        set_all(Color(0, 255, 0));
        break;
    case ir_protocols::SmallRemote::Buttons::Blue:
        set_all(Color(0, 0, 255));
        break;
    case ir_protocols::SmallRemote::Buttons::White:
        set_all(Color(255, 255, 255));
        break;
    case ir_protocols::SmallRemote::Buttons::R1:
        set_all(Color(255, 40, 0));
        break;
    case ir_protocols::SmallRemote::Buttons::Action1:
        toggle_ww();
        break;
    case ir_protocols::SmallRemote::Buttons::Action3:
        void_fade();
        break;
    default:
        break;
    }
}

void LEDs::off()
{
    strip_.clear();
    strip_.show();
}

uint32_t LEDs::Color(uint8_t r, uint8_t g, uint8_t b)
{
    return strip_.Color(r, g, b);
}

void LEDs::void_fade()
{
    uint32_t current_color = strip_.getPixelColor(0);
    for (int fade = -16; fade < num_leds + 16; fade++)
    {
        for (int i = 0; i < num_leds; i++)
        {
            uint8_t pixel_brightness = constrain((i - fade) * 16, 0, 255);
            strip_.setPixelColor(i, scaleColor(current_color, pixel_brightness));
        }
        strip_.show();
        delay(30);
    }
}