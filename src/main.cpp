#include "drivers/ir_remote/remote.hpp"
#include "drivers/leds/leds.hpp"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Arduino.h>


// -------------------- WiFi Configuration --------------------
#define SSID "IoT"
#define PASSWORD "Gosse69420"
#define SERVER_ADDRESS "http://192.168.68.52:12345/device/0/data/weather"


// -------------------- LED Configuration --------------------
#define IR_PIN D7
#define LED_PIN D6
#define WW_PIN D5
#define NUM_LEDS 42


sensors::IRRemote remote(IR_PIN);
decode_results results;
actors::LEDs strip(NUM_LEDS, LED_PIN, WW_PIN);

// -------------------- Setup --------------------
void setup()
{
    Serial.begin(115200);

    strip.begin();
    strip.set_brightness(20);

    // Connect to WiFi
    strip.set_all(strip.Color(0, 0, 255));
    WiFi.begin(SSID, PASSWORD);

    Serial.println("Connecting ...");
    bool ledState = true;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
        if (ledState)
        {
            strip.set_all(strip.Color(0, 0, 255));
        }
        else
        {
            strip.off();
        }
        ledState = !ledState;
    }

    Serial.print("\nConnected with IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Setup done");
    strip.set_all(strip.Color(0, 255, 0));
    delay(200);
    strip.off();
    delay(200);
    strip.set_all(strip.Color(0, 255, 0));
    delay(200);
    strip.set_brightness(128);
    strip.off();
}

// -------------------- Main Loop --------------------
ir_protocols::SmallRemote::Buttons last_button = ir_protocols::SmallRemote::Buttons::Off;
void loop()
{
    if (remote.decode(&results))
    {
        ir_protocols::SmallRemote::Buttons btn = static_cast<ir_protocols::SmallRemote::Buttons>(results.value);

        if (btn == ir_protocols::SmallRemote::Buttons::repeat)
        {
            btn = last_button;
        }
        else
        {
            last_button = btn;
        }

        strip.set_remote(btn);
        remote.resume(); // Receive the next value
    }
}
