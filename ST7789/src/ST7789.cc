//
// Created by amsakan on 04.11.25.
//
#include "ST7789.hh"
#include "ST7789_Commands.hh"
#include "pico/stdlib.h"


namespace ST7789 {
    auto ST7789::init() -> void {
        spi.init();
    }

    auto ST7789::reset() -> void {
        gpio_put(reset_pin, false);
        sleep_us(10);
        gpio_put(reset_pin, true);
        sleep_ms(120);

        gpio_put(cs_pin, false);
        gpio_put(dc_pin, false);
        spi_write_blocking(spi0, &Commands::SOFTWARE_RESET, 1);
        gpio_put(cs_pin, true);
        sleep_ms(120);
    }
}