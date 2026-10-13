#include "web_leds.hpp"


using namespace actors;

WebLEDs::WebLEDs()
{}


void WebLEDs::post_shorthand(String url, String data)
{
    Serial.println("Sending: ");
    Serial.println(url);
    Serial.println(data);
    Serial.println();

    client.begin(wclient, url);
    client.addHeader("Content-Type", "application/json");

    int http_code = client.POST(data);

    if (http_code <= 0) {
        Serial.printf("POST failed: %s\n", client.errorToString(http_code).c_str());
    }
}

void WebLEDs::set_brightness(uint8_t brightness)
{
    snprintf(
        post_buffer,
        47,
        "{\"brightness\":%d}",
        brightness
    );

    post_shorthand(
        "http://192.168.69.100/brightness",
        post_buffer
    );
}
void WebLEDs::set_brightness(uint8_t brightness, int8_t segment)
{
    snprintf(
        post_buffer,
        47,
        "{\"brightness\":%d,\"segment\":%d}",
        brightness,
        segment
    );

    post_shorthand(
        "http://192.168.69.100/brightness",
        post_buffer
    );
}

void WebLEDs::set_state(bool state)
{
    snprintf(
        post_buffer,
        47,
        "{\"state\":%d}",
        state
    );

    post_shorthand(
        "http://192.168.69.100/state",
        post_buffer
    );
}
void WebLEDs::set_state(bool state, int8_t segment)
{
    snprintf(
        post_buffer,
        47,
        "{\"state\":%d,\"segment\":%d}",
        state,
        segment
    );

    post_shorthand(
        "http://192.168.69.100/state",
        post_buffer
    );
}

void WebLEDs::toggle_segment()
{
    post_shorthand("http://192.168.69.100/toggle", "{}");
}

void WebLEDs::on()
{
    post_shorthand("http://192.168.69.100/on", "{}");
}

void WebLEDs::off()
{
    post_shorthand("http://192.168.69.100/off", "{}");
}
