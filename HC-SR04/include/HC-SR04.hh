#ifndef HC_SR04_HC_SR04_HH
#define HC_SR04_HC_SR04_HH
#include "pico/stdlib.h"

class HCSR04 {
    uint16_t trigger_pin;
    uint16_t echo_pin;

public:
    explicit HCSR04(uint16_t trigger_pin, uint16_t echo_pin)
        :trigger_pin(trigger_pin), echo_pin(echo_pin) {}

    void init() const;
    void measure(uint32_t* distance) const;
};
#endif //HC_SR04_HC_SR04_HH