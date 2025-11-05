//
// Created by abav on 04.11.2025.
//

#ifndef ST7789_COMMANDS_HH
#define ST7789_COMMANDS_HH
#include "pico/stdlib.h"

namespace ST7789 {
    enum class Commands : uint8_t {
        SOFTWARE_RESET = 0x01,
        SLEEP_OUT = 0x11,
        PIXEL_FORMAT = 0x3A,
        MEM_ACC_CTL = 0x06,
        DISPLAY_ON = 0x29,
        COL_ADR_SET = 0x2A,
        ROW_ADR_SET = 0x2B,
        MEM_WRT = 0x2C
    };
};
#endif //ST7789_COMMANDS_HH