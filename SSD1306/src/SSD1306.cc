//
// Created by amsakan on 03.11.25.
//
#include "SSD1306.hh"

#include <cstring>
#include "SSD1306_Commands.hh"


namespace SSD1306 {
    bool SSD1306::init() {
        i2c.init();

        uint32_t p = 2500;
        while(p > 0) {
            p--;
        }

        /* Init LCD */
        write_command(DISPLAY_OFF);                // display off
		write_command(SET_MEMORY_ADDRESSING_MODE); // Set Memory Addressing Mode
		write_command(MEMORY_ADDRESSING_ARG);      // 00: Horizontal, 01: Vertical, 10: Page (RESET)
		write_command(SET_PAGE_START_ADDR);        // Set Page Start Address for Page Addressing Mode,0-7
		write_command(SET_COM_OUTPUT_SCAN_DIR);    // Set COM Output Scan Direction
		write_command(SET_LOW_COLUMN_ADDR);        // set low column address
		write_command(SET_HIGH_COLUMN_ADDR);       // set high column address
		write_command(SET_START_LINE_ADDR);        // set start line address
		write_command(SET_CONTRAST);               // set contrast control register
		write_command(CONTRAST_MAX);
		write_command(SET_SEGMENT_REMAP);          // set segment re-map 0 to 127
		write_command(SET_NORMAL_DISPLAY);         // set normal display
		write_command(SET_MULTIPLEX_RATIO);        // set multiplex ratio(1 to 64)
		write_command(static_cast<uint8_t>(height - 1)); //
		write_command(OUTPUT_FOLLOWS_RAM);         // Output follows RAM content
		write_command(SET_DISPLAY_OFFSET);         // set display offset
		write_command(DISPLAY_OFFSET_0);           // no offset
		write_command(SET_DISPLAY_CLK_DIV);        // set display clock divide ratio/oscillator frequency
		write_command(DISPLAY_CLK_DIV_F0);         // set divide ratio
		write_command(SET_PRECHARGE_PERIOD);       // set pre-charge period
		write_command(PRECHARGE_22);
		write_command(SET_COM_PINS_HW_CONFIG);     // set com pins hardware configuration
		write_command(COM_PINS_CFG_12);
		write_command(SET_VCOMH);                  // set vcomh
		write_command(VCOMH_77PCT_VCC);            // 0x20 -> ~0.77 x Vcc
		write_command(SET_CHARGE_PUMP);            // set DC-DC enable
		write_command(CHARGE_PUMP_ENABLE);
		write_command(DISPLAY_ON);                 // turn on SSD1306
		write_command(DEACTIVATE_SCROLL);

        fill(Black);
        update_screen();

        current_x = 0;
        current_y = 0;
        initialized = true;

        return true;
    }

    void SSD1306::write_command(const uint8_t command) const {
        i2c.write(i2c.address, 0x00, command);
    }

    void SSD1306::write_data(const uint8_t data) const {
        i2c.write(i2c.address, 0x40, data);
    }

    void SSD1306::update_screen() {
    	const uint8_t pages = static_cast<uint8_t>(height / 8);
        for (uint8_t m = 0; m < pages; m++) {
            write_command(0xB0 + m);
            write_command(0x00);
            write_command(0x10);

            /* Write multi data */
            i2c.write_multi(i2c.address, 0x40, &buffer[width * m], width);
        }
    }

    void SSD1306::toggle_invert() {
        /* Toggle invert */
        this->inverted = !(this->inverted);

        /* Do memory toggle */
        for (uint16_t i = 0; i < sizeof(buffer); i++) {
            this->buffer[i] = ~(this->buffer[i]);
        }
    }

    void SSD1306::fill(Color color) {
        memset(buffer, (color == Black) ? 0x00 : 0xFF, sizeof(buffer));
    }


    void SSD1306::clear() {
        fill(Black);
        update_screen();
    }

    void SSD1306::invert_display(const bool invert) const {
        if (invert) write_command (INVERTDISPLAY);
        else write_command (NORMALDISPLAY);
    }

    void SSD1306::goto_xy(const uint16_t x, const uint16_t y) {
        /* Set write pointers */
        current_x = x;
        current_y = y;
    }

    char SSD1306::putc(char ch, Fonts::FontDef_t* Font, const Color color) {
        /* Check available space in LCD */
        if (
            width <= (current_x + Font->FontWidth) ||
            height <= (current_y + Font->FontHeight)
        ) {
            /* Error */
            return 0;
        }

        /* Go through font */
        for (uint32_t i = 0; i < Font->FontHeight; i++) {
            uint32_t b = Font->data[(ch - 32) * Font->FontHeight + i];
            for (uint32_t j = 0; j < Font->FontWidth; j++) {
                if ((b << j) & 0x8000) {
                    draw_pixel(current_x + j, (current_y + i), color);
                } else {
                    draw_pixel(current_x + j, (current_y + i), static_cast<Color>(!color));
                }
            }
        }

        /* Increase pointer */
        current_x += Font->FontWidth;

        /* Return character written */
        return ch;
    }

    char SSD1306::puts(const char* str, Fonts::FontDef_t* Font, const Color color) {
        /* Write characters */
        while (*str) {
            /* Write character by character */
            if (putc(*str, Font, color) != *str) {
                /* Return error */
                return *str;
            }

            /* Increase string pointer */
            str++;
        }

        /* Everything OK, zero should be returned */
        return *str;
    }

    void SSD1306::draw_pixel(const uint16_t x, uint16_t y, Color color) {
        if (
            x >= width ||
            y >= height
        ) {
            /* Error */
            return;
        }

        /* Check if pixels are inverted */
        if (inverted) {
            color = static_cast<Color>(!color);
        }

        /* Set color */
        if (color == White) {
            buffer[x + (y / 8) * width] |= 1 << (y % 8);
        } else {
            buffer[x + (y / 8) * width] &= ~(1 << (y % 8));
        }
    }

    void SSD1306::draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color c) {
        int16_t i, tmp;

        /* Check for overflow */
        if (x0 >= width) {
            x0 = width - 1;
        }
        if (x1 >= width) {
            x1 = width - 1;
        }
        if (y0 >= height) {
            y0 = height - 1;
        }
        if (y1 >= height) {
            y1 = height - 1;
        }

        int16_t dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
        int16_t dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
        int16_t sx = (x0 < x1) ? 1 : -1;
        int16_t sy = (y0 < y1) ? 1 : -1;
        int16_t err = ((dx > dy) ? dx : -dy) / 2;

        if (dx == 0) {
            if (y1 < y0) {
                tmp = y1;
                y1 = y0;
                y0 = tmp;
            }

            if (x1 < x0) {
                tmp = x1;
                x1 = x0;
                x0 = tmp;
            }

            /* Vertical line */
            for (i = y0; i <= y1; i++) {
                draw_pixel(x0, i, c);
            }

            /* Return from function */
            return;
        }

        if (dy == 0) {
            if (y1 < y0) {
                tmp = y1;
                y1 = y0;
                y0 = tmp;
            }

            if (x1 < x0) {
                tmp = x1;
                x1 = x0;
                x0 = tmp;
            }

            /* Horizontal line */
            for (i = x0; i <= x1; i++) {
                draw_pixel(i, y0, c);
            }

            /* Return from function */
            return;
        }

        while (true) {
            draw_pixel(x0, y0, c);
            if (x0 == x1 && y0 == y1) {
                break;
            }
            int16_t e2 = err;
            if (e2 > -dx) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dy) {
                err += dx;
                y0 += sy;
            }
        }
    }

    void SSD1306::draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color c) {
        /* Check input parameters */
        if (
            x >= width ||
            y >= height
        ) {
            /* Return error */
            return;
        }

        /* Check width and height */
        if ((x + w) >= width) {
            w = width - x;
        }
        if ((y + h) >= height) {
            h = height - y;
        }

        /* Draw 4 lines */
        draw_line(x, y, x + w, y, c);         /* Top line */
        draw_line(x, y + h, x + w, y + h, c); /* Bottom line */
        draw_line(x, y, x, y + h, c);         /* Left line */
        draw_line(x + w, y, x + w, y + h, c); /* Right line */
    }

    void SSD1306::draw_filled_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color c) {
        /* Check input parameters */
        if (
            x >= width ||
            y >= height
        ) {
            /* Return error */
            return;
        }

        /* Check width and height */
        if ((x + w) >= width) {
            w = width - x;
        }
        if ((y + h) >= height) {
            h = height - y;
        }

        /* Draw lines */
        for (uint8_t i = 0; i <= h; i++) {
            /* Draw lines */
            draw_line(x, y + i, x + w, y + i, c);
        }
    }

    void SSD1306::draw_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, Color color) {
        draw_line(x1, y1, x2, y2, color);
        draw_line(x2, y2, x3, y3, color);
        draw_line(x3, y3, x1, y1, color);
    }

    void SSD1306::draw_circle(int16_t x0, int16_t y0, int16_t r, Color c) {
        int16_t f = 1 - r;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * r;
        int16_t x = 0;
        int16_t y = r;

        draw_pixel(x0, y0 + r, c);
        draw_pixel(x0, y0 - r, c);
        draw_pixel(x0 + r, y0, c);
        draw_pixel(x0 - r, y0, c);

        while (x < y) {
            if (f >= 0) {
                y--;
                ddF_y += 2;
                f += ddF_y;
            }
            x++;
            ddF_x += 2;
            f += ddF_x;

            draw_pixel(x0 + x, y0 + y, c);
            draw_pixel(x0 - x, y0 + y, c);
            draw_pixel(x0 + x, y0 - y, c);
            draw_pixel(x0 - x, y0 - y, c);

            draw_pixel(x0 + y, y0 + x, c);
            draw_pixel(x0 - y, y0 + x, c);
            draw_pixel(x0 + y, y0 - x, c);
            draw_pixel(x0 - y, y0 - x, c);
        }
    }

    void SSD1306::draw_filled_circle(int16_t x0, int16_t y0, int16_t r, Color c) {
        int16_t f = 1 - r;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * r;
        int16_t x = 0;
        int16_t y = r;

        draw_pixel(x0, y0 + r, c);
        draw_pixel(x0, y0 - r, c);
        draw_pixel(x0 + r, y0, c);
        draw_pixel(x0 - r, y0, c);
        draw_line(x0 - r, y0, x0 + r, y0, c);

        while (x < y) {
            if (f >= 0) {
                y--;
                ddF_y += 2;
                f += ddF_y;
            }
            x++;
            ddF_x += 2;
            f += ddF_x;

            draw_line(x0 - x, y0 + y, x0 + x, y0 + y, c);
            draw_line(x0 + x, y0 - y, x0 - x, y0 - y, c);

            draw_line(x0 + y, y0 + x, x0 - y, y0 + x, c);
            draw_line(x0 + y, y0 - x, x0 - y, y0 - x, c);
        }
    }

    void SSD1306::draw_bitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, Color color) {
        int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
        uint8_t byte = 0;

        for(int16_t j=0; j<h; j++, y++)
        {
            for(int16_t i=0; i<w; i++)
            {
                if(i & 7)
                {
                    byte <<= 1;
                }
                else
                {
                    byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
                }
                if(byte & 0x80) draw_pixel(x+i, y, color);
            }
        }
    }

    void SSD1306::scroll_right(uint8_t start_row, uint8_t end_row) const {
        write_command(RIGHT_HORIZONTAL_SCROLL);  // send 0x26
        write_command(0x00);  // send dummy
        write_command(start_row);  // start page address
        write_command(0X00);  // time interval 5 frames
        write_command(end_row);  // end page address
        write_command(0X00);
        write_command(0XFF);
        write_command(ACTIVATE_SCROLL); // start scroll
    }

    void SSD1306::scroll_left(uint8_t start_row, uint8_t end_row) const {
        write_command(LEFT_HORIZONTAL_SCROLL);  // send 0x26
        write_command(0x00);  // send dummy
        write_command(start_row);  // start page address
        write_command(0X00);  // time interval 5 frames
        write_command(end_row);  // end page address
        write_command(0X00);
        write_command(0XFF);
        write_command(ACTIVATE_SCROLL); // start scroll
    }

    void SSD1306::scroll_diagonally_right(uint8_t start_row, uint8_t end_row) const {
        write_command(SET_VERTICAL_SCROLL_AREA);  // sect the area
        write_command (0x00);   // write dummy
        write_command(height);

        write_command(VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
        write_command (0x00);
        write_command(start_row);
        write_command(0X00);
        write_command(end_row);
        write_command (0x01);
        write_command(ACTIVATE_SCROLL);
    }

    void SSD1306::scroll_diagonally_left(uint8_t start_row, uint8_t end_row) const {
        write_command(SET_VERTICAL_SCROLL_AREA);  // sect the area
        write_command (0x00);   // write dummy
        write_command(height);

        write_command(VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
        write_command(0x00);
        write_command(start_row);
        write_command(0X00);
        write_command(end_row);
        write_command(0x01);
        write_command(ACTIVATE_SCROLL);
    }

    void SSD1306::stop_scroll() const {
        write_command(DEACTIVATE_SCROLL);
    }
}