//
// Created by amsakan on 04.11.25.
//
#include "ST7789.hh"
#include "ST7789_Commands.hh"
#include "pico/stdlib.h"

namespace ST7789 {
    auto ST7789::init() -> void {
        spi.init();
        reset();
        sleep_out();
        set_pixel_format();
        set_memory_access_control();
        turn_on_display();
    }

    auto ST7789::render_image(const uint8_t* image) -> void {
        uint8_t column_addresses[4] = {0, 0, (((width -1) >> 8) & 0xff), ((width-1) & 0xff)};
        uint8_t row_addresses[4] = {0, 0, (((height -1) >> 8) & 0xff), ((height-1) & 0xff)};

        gpio_put(cs_pin, false);

        gpio_put(dc_pin, false);
        spi.write(static_cast<uint8_t>(Commands::COL_ADR_SET));
        gpio_put(dc_pin, true);
        spi.write_multi(column_addresses, 4);

        gpio_put(dc_pin, false);
        spi.write(static_cast<uint8_t>(Commands::ROW_ADR_SET));
        gpio_put(dc_pin, true);
        spi.write_multi(row_addresses, 4);

        gpio_put(dc_pin, false);
        spi.write(static_cast<uint8_t>(Commands::MEM_WRT));
        gpio_put(dc_pin, true);

        for(uint32_t y = 0; y < height; y++) {
            for(uint32_t x = 0; x < width; x++) {
                spi.write_multi(image+(width * x + y), 3);
            }
        }

        gpio_put(cs_pin, true);
    }

    auto ST7789::reset() -> void {
        gpio_put(reset_pin, false);
        sleep_us(10);
        gpio_put(reset_pin, true);
        sleep_ms(120);

        gpio_put(cs_pin, false);
        gpio_put(dc_pin, false);
        spi.write(static_cast<uint8_t>(Commands::SOFTWARE_RESET));
        gpio_put(cs_pin, true);
        sleep_ms(120);
    }

    auto ST7789::sleep_out() -> void {
        gpio_put(cs_pin, false);
        gpio_put(dc_pin, false);
        spi.write(static_cast<uint8_t>(Commands::SLEEP_OUT));
        gpio_put(cs_pin, true);
        sleep_ms(120);
    }

    auto ST7789::set_pixel_format() -> void {
        constexpr uint8_t color_mod = 0x56;
        gpio_put(cs_pin, false);
        gpio_put(dc_pin, false);

        spi.write(static_cast<uint8_t>(Commands::PIXEL_FORMAT));

        gpio_put(dc_pin, true);
        spi.write(color_mod);

        constexpr uint8_t INVOFF = 0x21;
        gpio_put(dc_pin, false);
        spi.write(INVOFF);

        gpio_put(cs_pin, true);
    }

    auto ST7789::set_memory_access_control() -> void {
        uint8_t color_order = 0x00;
        gpio_put(cs_pin, false);
        gpio_put(dc_pin, false);

        spi.write(static_cast<uint8_t>(Commands::MEM_ACC_CTL));

        gpio_put(dc_pin, true);
        spi.write(color_order);
        gpio_put(cs_pin, true);
    }

    auto ST7789::turn_on_display() -> void {
        gpio_put(cs_pin, false);
        gpio_put(dc_pin, false);

        spi.write(static_cast<uint8_t>(Commands::DISPLAY_ON));

        gpio_put(dc_pin, true);
    }
}