//
// Created by amsakan on 04.11.25.
//
#include "ST7789.hh"
#include "ST7789_Commands.hh"
#include "pico/stdlib.h"


namespace ST7789 {
    auto ST7789::init() -> void {
        _gpio_init(cs_pin);
        gpio_set_dir(cs_pin, true);
        gpio_put(cs_pin, true);

        _spi_init(spi_handle, 625 * 100 * 1000);
        gpio_set_function(scl, GPIO_FUNC_SPI);
        gpio_set_function(sda, GPIO_FUNC_SPI);

        _gpio_init(reset_pin);
        gpio_set_dir(reset_pin, GPIO_OUT);

        _gpio_init(dc_pin);
        gpio_set_dir(dc_pin, GPIO_OUT);
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