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

    client.setTimeout(100);
    int http_code = client.POST(data);

    if (http_code <= 0)
    {
        Serial.printf("POST failed: %s\n", client.errorToString(http_code).c_str());
    }
}

void WebLEDs::set_brightness(uint8_t brightness)
{
    // post to shelf light
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
    // post to shelf light
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
    // post to shelf light
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
    // post to shelf light
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
    // post to shelf light
    post_shorthand("http://192.168.69.100/toggle", "{}");
}

void WebLEDs::on()
{
    // post to shelf light
    post_shorthand("http://192.168.69.100/on", "{}");
}

void WebLEDs::main_on()
{
    // post to tuya connector
    post_shorthand(
        "http://192.168.68.52:12344/device/bf1af55127739d4dc05qsh",
        "{\"dps\": 1, \"value\": true}"
    );
}

void WebLEDs::off()
{
    // post to shelf light
    post_shorthand("http://192.168.69.100/off", "{}");

    // post to tuya connector
    post_shorthand(
        "http://192.168.68.52:12344/device/bf1af55127739d4dc05qsh",
        "{\"dps\": 1, \"value\": false}"
    );
}

void WebLEDs::toggle_main()
{
    // post to tuya connector
    post_shorthand(
        "http://192.168.68.52:12344/device/bf1af55127739d4dc05qsh/toggle",
        "{\"}"
    );
}
