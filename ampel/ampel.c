/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"

#define RED 0
#define YELLOW 1
#define GREEN 2

#define RED_PHASE 5000
#define RED_YELLOW_PHASE 4000
#define GREEN_PHASE 4000
#define GREEN_BLINK_PHASE 500
#define YELLOW_PHASE 4000
#define YELLOW_BLINK_PHASE 500

typedef enum {
    RED_STATE,
    RED_YELLOW_STATE,
    GREEN_STATE,
    GREEN_BLINK_STATE,
    YELLOW_STATE,
    YELLOW_BLINK_STATE
} eState;

eState redStateHandler() {
    gpio_put(RED, 1);
    sleep_ms(RED_PHASE);
    gpio_put(RED, 0);
    return RED_YELLOW_STATE;
}

eState redYellowStateHandler() {
    gpio_put(RED, 1);
    gpio_put(YELLOW, 1);
    sleep_ms(RED_YELLOW_PHASE);
    gpio_put(RED, 0);
    gpio_put(YELLOW, 0);
    return GREEN_STATE;
}

eState greenStateHandler() {
    gpio_put(GREEN, 1);
    sleep_ms(GREEN_PHASE);
    gpio_put(GREEN, 0);
    return GREEN_BLINK_STATE;
}

eState greenBlinkStateHandler() {
    for (int i = 0; i < 4; i++) {
        gpio_put(GREEN, 1);
        sleep_ms(GREEN_BLINK_PHASE);
        gpio_put(GREEN, 0);
        sleep_ms(GREEN_BLINK_PHASE);
    }
    return YELLOW_STATE;
}

eState yellowStateHandler() {
    gpio_put(YELLOW, 1);
    sleep_ms(YELLOW_PHASE);
    gpio_put(YELLOW, 0);
    return RED_STATE;
}

void yellowBlinkStateHandler() {
    for (int i = 0; i < 4; i++) {
        gpio_put(YELLOW, 1);
        sleep_ms(YELLOW_BLINK_PHASE);
        gpio_put(YELLOW, 0);
        sleep_ms(YELLOW_BLINK_PHASE);
    }
}

int main() {
    gpio_init(RED);
    gpio_init(YELLOW);
    gpio_init(GREEN);
    gpio_set_dir(RED, GPIO_OUT);
    gpio_set_dir(YELLOW, GPIO_OUT);
    gpio_set_dir(GREEN, GPIO_OUT);
    eState currentState = RED_STATE;
    while (true) {
        switch (currentState) {
            case RED_STATE:
                currentState = redStateHandler();
            case RED_YELLOW_STATE:
                currentState = redYellowStateHandler();
                break;
            case GREEN_STATE:
                currentState = greenStateHandler();
                break;
            case GREEN_BLINK_STATE:
                currentState = greenBlinkStateHandler();
                break;
            case YELLOW_STATE:
                currentState = yellowStateHandler();
                break;
            default:
                yellowBlinkStateHandler();
                break;
        }
    }
}
