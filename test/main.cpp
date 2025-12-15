#include <Adafruit_NeoPixel.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Arduino.h>

#define SSID "IoT"
#define PASSWORD "Gosse69420"
#define SERVER_ADDRESS "http://192.168.68.52:12345/device/0/data/weather"

// -------------------- LED Configuration --------------------
#define IR_PIN D7
#define LED_PIN D6
#define CW_PIN D5
#define NUM_LEDS 18
#define BRIGHTNESS 80

Adafruit_NeoPixel strip(
    NUM_LEDS,
    LED_PIN,
    NEO_RGB + NEO_KHZ800 // WS2815 / WS2812 timing
);

// -------------------- Temperature → Color --------------------
uint32_t temperatureToColor(float tempC)
{
    if (tempC >= 21.0 && tempC <= 23.0)
    {
        return strip.Color(0, 255, 0);
    }

    tempC = constrain(tempC, 16.0, 30.0);

    // Hue: blue (160°) → red (0°)
    uint16_t hue = map(tempC * 10, 160, 300, 160, 0) * 256;

    // Slightly reduced saturation looks better indoors
    uint8_t sat = 200;
    uint8_t val = 255;

    // Convert HSV → RGB
    uint32_t rawColor = strip.ColorHSV(hue, sat, val);

    // APPLY GAMMA CORRECTION (this is the missing piece)
    return strip.gamma32(rawColor);
}

// -------------------- Setup --------------------
void setup()
{
    Serial.begin(9600);

    strip.begin();
    strip.setBrightness(BRIGHTNESS);
    strip.show(); // clear LEDs

    // Connect to WiFi
    WiFi.begin(SSID, PASSWORD);

    Serial.println("Connecting ...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
    }

    Serial.print("\nConnected with IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Setup done");
}

// -------------------- Main Loop --------------------
void loop()
{
    WiFiClient client;
    HTTPClient http;

    if (http.begin(client, SERVER_ADDRESS))
    {
        int httpCode = http.GET();

        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();

            float temperature =
                payload.substring(
                           payload.indexOf("temperature") + 13,
                           payload.indexOf(",", payload.indexOf("temperature")))
                    .toFloat();

            Serial.print("Received temperature: ");
            Serial.println(temperature);

            uint32_t color = temperatureToColor(temperature);

            for (int i = 0; i < NUM_LEDS; i++)
            {
                strip.setPixelColor(i, color);
            }

            strip.show();
        }
        else
        {
            Serial.printf(
                "HTTP GET failed, error: %s\n",
                http.errorToString(httpCode).c_str());
        }

        http.end();
    }
    else
    {
        Serial.println("Unable to connect");
    }

    delay(10000);
}
