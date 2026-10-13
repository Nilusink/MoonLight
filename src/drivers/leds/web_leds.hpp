/**
 * @file web_leds.hpp
 * @author Nilusink
 * @brief controls leds via http requests
 * @version 0.1
 * @date 2026-10-13
 * 
 * @copyright Copyright Nilusink (c) 2026
 * 
 */
#pragma once
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>


namespace actors
{
    class WebLEDs
    {
    private:
        WiFiClient wclient;
        HTTPClient client;
        char post_buffer[48];

        void post_shorthand(String url, String data);

    public:
        WebLEDs();

        void set_brightness(uint8_t brightness);
        void set_brightness(uint8_t brightness, int8_t segment);

        void set_state(bool state);
        void set_state(bool state, int8_t segment);

        void on();
        void off();

        // segment control
        void toggle_segment();
    };
        
} // namespace actors
