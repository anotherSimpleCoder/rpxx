//
// Created by amsakan on 04.11.25.
//

#ifndef ST7789_ST7789_HH
#define ST7789_ST7789_HH
#include "pico/stdlib.h"
#include "ST7789_SPI.hh"

namespace ST7789 {
    class ST7789 {
        static constexpr uint32_t width = 240;
        static constexpr uint32_t height = 320;
        uint8_t buffer[width][height][3] = {};

        SPI spi;

        auto reset() -> void;
        auto sleep_out() -> void;
        auto set_pixel_format() -> void;
        auto set_memory_access_control() -> void;
        auto turn_on_display() -> void;
    public:
        ST7789(
            spi_inst_t* spi_handle,
            uint8_t scl,
            uint8_t sda,
            uint8_t reset_pin,
            uint8_t dc_pin,
            uint8_t cs_pin
        ): spi(spi_handle, reset_pin, dc_) {}
        auto init() -> void;
        auto display_image() -> void;
    };
};
#endif //ST7789_ST7789_HH