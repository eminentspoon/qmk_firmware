// Copyright 2023 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum keymap_layers { _NUM, _MOV, _ADJ};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_NUM] = LAYOUT(
        KC_1          , KC_2    , LT(_ADJ,KC_3),
        KC_4          , KC_5    , KC_6,
        KC_7          , KC_8    , KC_9,
        LT(0,KC_ENTER), KC_0    , KC_DOT
    ),
    [_MOV] = LAYOUT(
        KC_INS       , KC_HOME  , LT(_ADJ,KC_PGUP),
        KC_DEL       , KC_END   , KC_PGDN,
        KC_BSPC      , KC_UP    , KC_ENT,
        LT(0,KC_LEFT), KC_DOWN  , KC_RIGHT
    ),
    [_ADJ] = LAYOUT(
        QK_BOOT      , KC_NO    , KC_TRNS,
        RGB_HUI      , RGB_SAI  , RGB_VAI,
        RGB_HUD      , RGB_SAD  , RGB_VAD,
        RGB_TOG      , RGB_MOD  , RGB_RMOD
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_NUM] =  {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [_MOV] =  {ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)},
    [_ADJ] =  {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)}
};
#endif
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(0, KC_ENTER):
            if (!record->tap.count && record->event.pressed) {
                layer_move(_MOV);
                return false;
            }
            return true;
        case LT(0, KC_LEFT):
            if (!record->tap.count && record->event.pressed) {
                layer_move(_NUM);
                return false;
            }
            return true;
    }
    return true;
}