#include "remote.hpp"


using namespace sensors;

IRRemote::IRRemote(uint8_t pin) : irrecv_(pin)
{
    irrecv_.enableIRIn(); // Start the receiver
}

bool IRRemote::decode(decode_results* results)
{
    return irrecv_.decode(results);
}

void IRRemote::resume()
{
    irrecv_.resume(); // Receive the next value
}