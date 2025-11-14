//
// Created by abav on 14.11.2025.
//

#ifndef SSD1306_I2C_HH
#define SSD1306_I2C_HH
#include "hardware/i2c.h"

namespace SSD1306 {
    class I2C {
        uint16_t sda, scl;
        i2c_inst* i2c_handle;

    public:
        const uint8_t address = 0x78;
        const unsigned int timeout = 20000;

        explicit I2C(i2c_inst* i2c_handle, uint16_t sda, uint16_t scl)
            : i2c_handle(i2c_handle), sda(sda), scl(scl) {}

        void init();
        void write(uint8_t address, uint8_t reg, uint8_t data) const;
        void write_multi(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) const;
        i2c_inst* get_i2c_handle() const;
    };
};
#endif //SSD1306_I2C_HH
