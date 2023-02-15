// Copyright 2023 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum keymap_layers { _NUM, _MOV };

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_NUM] = LAYOUT(
        KC_1          , KC_2    , KC_3,
        KC_4          , KC_5    , KC_6,
        KC_7          , KC_8    , KC_9,
        LT(0,KC_ENTER), KC_0    , KC_DOT
    ),
    [_MOV] = LAYOUT(
        KC_INS       , KC_HOME  , KC_PGUP,
        KC_DEL       , KC_END   , KC_PGDN,
        KC_BSPC      , KC_UP    , KC_ENT,
        LT(0,KC_LEFT), KC_DOWN  , KC_RIGHT
    )
};
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