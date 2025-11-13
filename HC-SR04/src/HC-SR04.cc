#include "HC-SR04.hh"

void HCSR04::init() const {
    gpio_put(trigger_pin, false);
}


void HCSR04::measure(uint32_t* distance) const {
    gpio_put(trigger_pin, true);
    sleep_us(10);
    gpio_put(trigger_pin, false);

    uint32_t pMillis = to_ms_since_boot(get_absolute_time());

    while (!gpio_get (echo_pin) && pMillis + 10 >  to_ms_since_boot(get_absolute_time())) {}
    const uint32_t Value1 = time_us_32();

    pMillis = to_ms_since_boot(get_absolute_time());

    while ((gpio_get(echo_pin)) && pMillis + 50 > to_ms_since_boot(get_absolute_time())) {}
    const uint32_t Value2 = time_us_32();

    *distance = (Value2-Value1)* 0.034/2;
    sleep_ms(50);
}