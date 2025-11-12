//
// Created by amsakan on 04.11.25.
//

#ifndef ST7789_ST7789_SPI_HH
#define ST7789_ST7789_SPI_HH
#include "hardware/spi.h"

namespace ST7789 {
    class SPI {
        spi_inst_t* spi_handle;

        const uint8_t scl;
        uint8_t sda;
        uint8_t reset_pin;
        uint8_t dc_pin;
        uint8_t cs_pin;

    public:
        explicit SPI(
            spi_inst_t* spi_handle,
            uint8_t scl,
            uint8_t sda,
            uint8_t reset_pin,
            uint8_t dc_pin,
            uint8_t cs_pin
        ) : spi_handle(spi_handle), scl(scl), sda(sda), reset_pin(reset_pin), dc_pin(dc_pin), cs_pin(cs_pin) {}

        auto init() -> void;
        auto write(const uint8_t data) -> void;
        auto write_multi(const uint8_t *data, uint16_t count) -> void;
        auto get_spi_handle() -> spi_inst*;
    };
}
#endif //ST7789_ST7789_SPI_HH