#include "Fonts/SSD1306_Fonts.hh"

#include <string.h>

#include "Fonts/7x10.hh"
#include "Fonts/11x18.hh"
#include "Fonts/16x26.hh"


namespace Fonts {
    FontDef_t Font_7x10 = {
        7,
        10,
        Font7x10
    };

    FontDef_t Font_11x18 = {
        11,
        18,
        Font11x18
    };

    FontDef_t Font_16x26 = {
        16,
        26,
        Font16x26
    };

    char* get_string_size(char* str, FONTS_SIZE_t* SizeStruct, FontDef_t* Font) {
        /* Fill settings */
        SizeStruct->Height = Font->FontHeight;
        SizeStruct->Length = Font->FontWidth * strlen(str);

        /* Return pointer */
        return str;
    }
}

