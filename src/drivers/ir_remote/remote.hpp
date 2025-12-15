#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "drivers/ir_remote/protocols.hpp"


namespace sensors
{
    class IRRemote
    {
    private:
        IRrecv irrecv_;
        decode_results results;
    public:
        IRRemote(uint8_t pin);

        bool decode(decode_results* results);

        void resume();
    };
}