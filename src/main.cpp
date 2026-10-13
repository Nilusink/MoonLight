#include "drivers/ir_remote/remote.hpp"
#include "drivers/leds/leds.hpp"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <Arduino.h>


// -------------------- WiFi Configuration --------------------
#define SSID "IoT"
#define PASSWORD "Gosse69420"

#define LOCAL_IP 192, 168, 69, 101
#define GATEWAY 192, 168, 68, 1
#define SUBNET 255, 255, 254, 0
#define DNS 1, 1, 1, 1


// -------------------- LED Configuration --------------------
#define IR_PIN D7
#define LED_PIN D6
#define WW_PIN D5
#define NUM_LEDS 35


sensors::IRRemote remote(IR_PIN);
decode_results results;
actors::LEDs strip(NUM_LEDS, LED_PIN, WW_PIN);

// -------------------- Setup --------------------
void setup()
{
    Serial.begin(9600);

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
    
    // configure static IP
    if (!WiFi.config(IPAddress(LOCAL_IP), IPAddress(GATEWAY), IPAddress(SUBNET), IPAddress(DNS)))
    {
        strip.set_all(strip.Color(255, 0, 0));
        for (;;) { delay(10); };
    }}

    // begin OTA
    ArduinoOTA.begin();

    Serial.print("\nConnected with IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Setup done");
    strip.set_all(strip.Color(0, 255, 0));
    delay(200);
    strip.off(true);
    delay(200);
    strip.set_all(strip.Color(0, 255, 0));
    delay(200);
    strip.set_all(strip.Color(255, 80, 10));
    strip.set_brightness(128);
    strip.off(true);
}

// -------------------- Main Loop --------------------
ir_protocols::SmallRemote::Buttons last_button = ir_protocols::SmallRemote::Buttons::Off;
bool in_segment_control = false;
uint8_t current_segment = 0;
void loop()
{
    // handle OTA requests
    ArduinoOTA.handle();

    // handle remote
    if (remote.decode(&results) && true)
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
