//
// Created by amsakan on 12.11.25.
//
#include "ST7789_SPI.hh"

#include "pico/stdlib.h"

namespace ST7789 {
    auto SPI::init() -> void {
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

    auto SPI::write(const uint8_t data) -> void {
        spi_write_blocking(spi_handle, &data, 1);
    }

    auto SPI::write_multi(const uint8_t *data, uint16_t count) -> void {
        spi_write_blocking(spi_handle, data, count);
    }

    auto SPI::get_spi_handle() -> spi_inst* {
        return this->spi_handle;
    }
}