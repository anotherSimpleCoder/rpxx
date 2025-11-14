//
// Created by abav on 14.11.2025.
//

#ifndef SSD1306_HH
#define SSD1306_HH

#include "pico/stdlib.h"
#include "SSD1306_I2C.hh"
#include "Fonts/SSD1306_Fonts.hh"

namespace SSD1306 {
    enum Color {
        Black = 0x00,
        White = 0x01,
    };

    class SSD1306 {
        static constexpr uint32_t width = 128;
        static constexpr uint32_t height = 32;
        I2C i2c;
        uint8_t buffer[width * height / 8] = {};

        uint16_t current_x;
        uint16_t current_y;
        bool inverted;
        bool initialized;
    public:
        explicit SSD1306(i2c_inst* i2c_handle, uint16_t sda, uint16_t scl)
            : i2c(i2c_handle, sda, scl) {
            current_x = 0;
            current_y = 0;
            inverted = false;
            initialized = false;
        }

        bool init();
        void write_command(uint8_t command) const;
        void write_data(uint8_t data) const;
        void update_screen();
        void toggle_invert();

        void fill(Color color);
        void clear();
        void invert_display(bool invert) const;
        void goto_xy(uint16_t x, uint16_t y);
        char putc(char ch, Fonts::FontDef_t* Font, Color color);
        char puts(const char* str, Fonts::FontDef_t* Font, Color color);
        void draw_pixel(uint16_t x, uint16_t y, Color color);
        void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color c);
        void draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color c);
        void draw_filled_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color c);
        void draw_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, Color color);
        void draw_circle(int16_t x0, int16_t y0, int16_t r, Color c);
        void draw_filled_circle(int16_t x0, int16_t y0, int16_t r, Color c);
        void draw_bitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, Color color);

        void scroll_right(uint8_t start_row, uint8_t end_row) const;
        void scroll_left(uint8_t start_row, uint8_t end_row) const;
        void scroll_diagonally_right(uint8_t start_row, uint8_t end_row) const;
        void scroll_diagonally_left(uint8_t start_row, uint8_t end_row) const;
        void stop_scroll() const;
    };
}

#endif //SSD1306_HH
