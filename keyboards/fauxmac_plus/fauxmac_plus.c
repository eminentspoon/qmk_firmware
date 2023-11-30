// Copyright 2023 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "os_detection.h"

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_BOOT:
#if defined(RGBLIGHT_ENABLE)
            rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            rgblight_sethsv_noeeprom(0, 255, 255);
#endif
#if defined(OLED_ENABLE)
            oled_clear();
            oled_write_P(PSTR("RESET"), false);
            oled_render();
#endif
    }
    return process_record_user(keycode, record);
}

#if defined(ENCODER_ENABLE)
bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) {
        return false;
    }
    if (clockwise) {
        tap_code_delay(KC_VOLU, 10);
    } else {
        tap_code_delay(KC_VOLD, 10);
    }
    return true;
}
#endif

uint32_t boot_time = 0;

void keyboard_post_init_kb(void) {
    keyboard_post_init_user();
    boot_time = timer_read32();
}

#if defined(OLED_ENABLE)

bool should_render_logo = true;

static void render_logo(void) {
    static const char PROGMEM fx_logo[] = {
        0,   0,   0,   0,   0,   0,   0, 0,   0,   0,   0,   0,   0,   0,   0, 0,   0,   0,   0,   0,   0,   0,   0, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   124, 254, 254, 254, 254, 254, 124, 0,   0, 0,   0, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0,   0, 0,   252, 6,   246, 246, 246, 246, 246, 246, 254, 6,   246, 246, 246, 246, 246, 6,   254, 6,   254, 254, 254, 254, 254, 6,   254, 6,   126, 254, 254, 254, 126, 6,   254, 6,   246, 246, 6,   246, 246, 6, 254, 6, 246, 246, 246, 246, 246, 6,   254, 6,   246, 246, 246, 246, 246, 246, 254, 252, 124, 254, 254, 254, 254, 254, 124, 0,   124, 254, 254, 254, 254, 254, 124, 0,   124, 254, 254, 254, 254, 254, 124, 0,   124, 254, 254, 254, 254, 254, 124, 0,   124, 254, 254, 254, 254, 254, 124, 0,   0,   0,   124,
        254, 254, 254, 254, 254, 124, 0, 124, 254, 254, 254, 254, 254, 124, 0, 124, 254, 254, 254, 254, 254, 124, 0, 0,   0,   0,   0,   255, 0,   253, 253, 253, 253, 253, 253, 255, 0,   253, 253, 253, 253, 253, 0,   255, 0,   127, 127, 127, 127, 127, 0, 255, 7, 242, 250, 248, 250, 242, 7,   255, 0,   255, 255, 0,   255, 255, 0, 255, 0, 253, 253, 253, 253, 253, 0,   255, 0,   127, 127, 127, 127, 127, 127, 255, 255, 124, 254, 254, 254, 254, 254, 124, 0,   0,   0,   0,   0,   0,   0,   0,   0,   124, 254, 254, 254, 254, 254, 124, 0,   0, 0,   0, 0,   0,   0,   0,   0,   124, 254, 254, 254, 254, 254, 124, 0,   0,   0,   124, 254, 254, 254, 254, 254, 124, 0,   0,   0,   0,   0,   0,   0,   0,   0,   124, 254, 254, 254, 254, 254, 124, 0,   0,   0,   0,   0,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1,   125,
        125, 125, 125, 125, 1,   255, 1, 255, 255, 255, 255, 255, 255, 255, 1, 255, 255, 255, 255, 255, 1,   255, 1, 125, 125, 125, 125, 125, 125, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 124, 254, 254, 254, 254, 254, 124, 0,   0, 0,   0, 0,   0,   0,   0,   0,   124, 254, 254, 254, 254, 254, 124, 0,   0, 0,   0, 0,   0,   0,   0,   0,   124, 254, 254, 254, 254, 254, 124, 0,   0,   0,   124, 254, 254, 254, 254, 254, 124, 0,   124, 254, 254, 254, 254, 254, 124, 0,   124, 254, 254, 254, 254, 254, 124, 0,   0,   0, 0,   0, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 192, 255, 255, 255, 255, 255, 255, 255, 192, 223, 223, 223, 223, 223, 223, 255, 192, 223, 223, 223, 223, 223, 192, 255, 223, 223, 223, 223, 223, 223, 192, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127,
    };
    oled_write_raw_P(fx_logo, sizeof(fx_logo));
}

static void render_os(void) {
    static const char PROGMEM lin_logo[] = {0x20, 0x20, 0x99, 0x9A, 0x20, 0x20, 0x20, 0xB9, 0xBA, 0x20, 0};
    static const char PROGMEM win_logo[] = {0x20, 0x20, 0x97, 0x98, 0x20, 0x20, 0x20, 0xB7, 0xB8, 0x20, 0};
    static const char PROGMEM ban_logo[] = {0x20, 0x20, 0x95, 0x96, 0x20, 0x20, 0x20, 0xB5, 0xB6, 0x20, 0};
    static const char PROGMEM qmk_logo[] = {0x20, 0x81, 0x82, 0x83, 0x84, 0x20, 0xA1, 0xA2, 0xA3, 0xA4, 0x20, 0xC1, 0xC2, 0xC3, 0xC4, 0};
    switch (detected_host_os()) {
        case OS_WINDOWS:
            oled_write_P(win_logo, false);
            break;
        case OS_LINUX:
            oled_write_P(lin_logo, false);
            break;
        case OS_MACOS:
        case OS_IOS:
            oled_write_P(ban_logo, false);
            break;
        default:
            oled_write_P(qmk_logo, false);
            return;
    }
    oled_advance_page(true);
}

bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }

    if (should_render_logo) {
        render_logo();
        if (timer_elapsed32(boot_time) > 2500) {
            should_render_logo = false;
            oled_clear();
            oled_init(OLED_ROTATION_270);
        }
    } else {
        oled_write_P(PSTR("LAYER"), false);
        oled_advance_page(false);
        oled_write(get_u8_str(get_highest_layer(layer_state), ' '), false);
        oled_advance_page(true);
        oled_advance_page(true);
        oled_advance_page(true);
        oled_write_P(PSTR(" NUM "), host_keyboard_led_state().num_lock);
        oled_advance_page(true);
        oled_write_P(PSTR(" CAP "), host_keyboard_led_state().caps_lock);
        oled_advance_page(true);
        oled_write_P(PSTR(" SCR "), host_keyboard_led_state().scroll_lock);
        oled_advance_page(true);
        oled_advance_page(true);

        render_os();
    }
    return true;
}

#endif