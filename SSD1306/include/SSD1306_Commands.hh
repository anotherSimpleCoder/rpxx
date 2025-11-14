//
// Created by amsakan on 03.11.25.
//

#ifndef SSD1306_SSD1306_COMMANDS_HH
#define SSD1306_SSD1306_COMMANDS_HH
namespace SSD1306 {
    enum Commands {
        DISPLAY_OFF                    = 0xAE, // display off
        SET_MEMORY_ADDRESSING_MODE     = 0x20, // Set Memory Addressing Mode (followed by a mode byte)
        MEMORY_ADDRESSING_ARG          = 0x10, // (used in your sequence) addressing-mode argument / or high column in another context
        SET_PAGE_START_ADDR            = 0xB0, // Set Page Start Address for Page Addressing Mode, 0-7
        SET_COM_OUTPUT_SCAN_DIR        = 0xC8, // Set COM Output Scan Direction (remapped)
        SET_LOW_COLUMN_ADDR            = 0x00, // Set low column address (lower nibble)
        SET_HIGH_COLUMN_ADDR           = 0x10, // Set high column address (higher nibble)
        SET_START_LINE_ADDR            = 0x40, // Set start line address
        SET_CONTRAST                   = 0x81, // Set contrast control register (followed by contrast byte)
        CONTRAST_MAX                   = 0xFF, // Example contrast value used in your sequence
        SET_SEGMENT_REMAP              = 0xA1, // Set segment re-map (column address 127 is mapped to SEG0)
        SET_NORMAL_DISPLAY             = 0xA6, // Set normal display (A7 = inverse)
        SET_MULTIPLEX_RATIO            = 0xA8, // Set multiplex ratio (followed by ratio byte)
        MULTIPLEX_RATIO_63             = 0x3F, // 0x3F -> 1/64 duty
        OUTPUT_FOLLOWS_RAM             = 0xA4, // Output follows RAM content (A5 = ignore RAM content)
        SET_DISPLAY_OFFSET             = 0xD3, // Set display offset (followed by offset byte)
        DISPLAY_OFFSET_0               = 0x00, // No offset
        SET_DISPLAY_CLK_DIV            = 0xD5, // Set display clock divide ratio/oscillator frequency
        DISPLAY_CLK_DIV_F0             = 0xF0, // Example divider value used in your sequence
        SET_PRECHARGE_PERIOD           = 0xD9, // Set pre-charge period (followed by period byte)
        PRECHARGE_22                   = 0x22, // Example pre-charge value used in your sequence
        SET_COM_PINS_HW_CONFIG         = 0xDA, // Set COM pins hardware configuration (followed by config byte)
        COM_PINS_CFG_12                = 0x12, // Example COM pins configuration used in your sequence
        SET_VCOMH                      = 0xDB, // Set VCOMH deselect level (followed by level byte)
        VCOMH_77PCT_VCC                = 0x20, // 0x20 -> ~0.77 x Vcc (value used in your sequence)
        SET_CHARGE_PUMP                = 0x8D, // Charge pump setting (followed by 0x14 to enable)
        CHARGE_PUMP_ENABLE             = 0x14, // Enable charge pump
        DISPLAY_ON                     = 0xAF,  // Turn on SSD1306 panel
        RIGHT_HORIZONTAL_SCROLL        = 0x26,
        LEFT_HORIZONTAL_SCROLL         = 0x27,
        VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29,
        VERTICAL_AND_LEFT_HORIZONTAL_SCROLL = 0x2A,
        DEACTIVATE_SCROLL = 0x2E,
        ACTIVATE_SCROLL = 0x2F,
        SET_VERTICAL_SCROLL_AREA = 0xA3,
        NORMALDISPLAY = 0xA6,
        INVERTDISPLAY = 0xA7,
    };
}
#endif //SSD1306_SSD1306_COMMANDS_HH