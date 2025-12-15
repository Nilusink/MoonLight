#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "drivers/ir_remote/protocols.hpp"

#define IR_PIN D7        // GPIO for IR receiver OUT
#define LED_PIN D5       // GPIO for LED or MOSFET

IRrecv irrecv(IR_PIN);
decode_results results;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  irrecv.enableIRIn();  // Start the receiver
  Serial.println("IR Receiver ready");
}

void loop() {
  if (irrecv.decode(&results)) {
    // Print raw value
    Serial.print("Raw value: 0x");
    Serial.println(results.value, HEX);

    // Print protocol
    Serial.print("Protocol: ");
    Serial.println(typeToString(results.decode_type));

    // Example: toggle LED based on button
    ir_protocols::SmallRemote::Buttons btn = static_cast<ir_protocols::SmallRemote::Buttons>(results.value);
    switch (btn)
    {
    case ir_protocols::SmallRemote::Buttons::On:
        Serial.println("Button: ON");
        break;
    
    case ir_protocols::SmallRemote::Buttons::Off:
        Serial.println("Button: OFF");
        break;

    default:
        break;
    }

    irrecv.resume();  // Receive the next value
  }
}


