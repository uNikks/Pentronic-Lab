// Copyright 2026 Pentronic Lab.
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┐
     * │ Q │ W │ E │ R │
     * └───┴───┴───┴───┘
     *  SW1 SW2 SW3 SW4
     */
    [0] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R
    ),

    [1] = LAYOUT(
        _______, _______, _______, _______
    ),

    [2] = LAYOUT(
        _______, _______, _______, _______
    ),

    [3] = LAYOUT(
        _______, _______, _______, _______
    )
};
