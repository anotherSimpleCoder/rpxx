//
// Created by abav on 14.11.2025.
//
#include "SSD1306_I2C.hh"


void SSD1306::I2C::init() {
    uint32_t p = 250000;
    while(p>0)
        p--;
}

void SSD1306::I2C::write(uint8_t address, uint8_t reg, uint8_t data) const {
    uint8_t dt[2];
    dt[0] = reg;
    dt[1] = data;
    i2c_write_blocking(i2c_handle, address, dt, 2, true);
}

void SSD1306::I2C::write_multi(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) const {
    uint8_t dt[256];
    dt[0] = reg;
    for(uint8_t i=0;i<count;i++) {
        dt[i+1] = data[i];
    }

    i2c_write_blocking(i2c_handle, address, dt, count+1, true);
}

i2c_inst* SSD1306::I2C::get_i2c_handle() const {
      return i2c_handle;
}