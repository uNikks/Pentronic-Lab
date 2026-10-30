# QWERuler premium

A 160 x 25 mm PCB ruler that is also a 4-key macropad (black x gold limited edition of QWERuler).

* Keyboard Maintainer: [Pentronic Lab.](https://github.com/uNikks)
* Hardware Supported: QWERuler premium PCB with Waveshare RP2040-Zero (or the optional direct-mount RP2040 circuit on the back side)
* Hardware Availability: [Pentronic Lab.](https://pentronic-lab.com/)

The four switches are wired directly (no matrix, no diodes): SW1 = GP8, SW2 = GP7, SW3 = GP6, SW4 = GP5, the other pin goes to GND.

Make example for this keyboard (after setting up your build environment):

    make pentroniclab/qweruler_premium:vial

Flashing example for this keyboard:

    make pentroniclab/qweruler_premium:vial:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down SW1 (the leftmost key) and plug in the keyboard
* **Physical reset button**: Double-tap the RESET button on the RP2040-Zero (or hold BOOT and press RESET)
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
