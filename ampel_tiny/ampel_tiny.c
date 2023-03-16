/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"

#define RED 18
#define GREEN 19
#define BLUE 20

#define RED_PHASE 5000
#define RED_YELLOW_PHASE 4000
#define GREEN_PHASE 4000
#define GREEN_BLINK_PHASE 500
#define YELLOW_PHASE 4000
#define YELLOW_BLINK_PHASE 500

void green_on();
void green_off();
void yellow_on();
void yellow_off();
void red_on();
void red_off();

typedef enum {
    RED_STATE,
    RED_YELLOW_STATE,
    GREEN_STATE,
    GREEN_BLINK_STATE,
    YELLOW_STATE,
    YELLOW_BLINK_STATE
} eState;

eState redStateHandler() {
    red_on();
    sleep_ms(RED_PHASE);
    red_off();
    return RED_YELLOW_STATE;
}

eState redYellowStateHandler() {
    red_on();
    yellow_on();
    sleep_ms(RED_YELLOW_PHASE);
    red_off();
    yellow_off();
    return GREEN_STATE;
}

eState greenStateHandler() {
    green_on();
    sleep_ms(GREEN_PHASE);
    green_off();
    return GREEN_BLINK_STATE;
}

eState greenBlinkStateHandler() {
    for (int i = 0; i < 4; i++) {
        green_on();
        sleep_ms(GREEN_BLINK_PHASE);
        green_off();
        sleep_ms(GREEN_BLINK_PHASE);
    }
    return YELLOW_STATE;
}

eState yellowStateHandler() {
    yellow_on();
    sleep_ms(YELLOW_PHASE);
    yellow_off();
    return RED_STATE;
}

void yellowBlinkStateHandler() {
    for (int i = 0; i < 4; i++) {
        yellow_off();
        sleep_ms(YELLOW_BLINK_PHASE);
        yellow_off();
        sleep_ms(YELLOW_BLINK_PHASE);
    }
}

void red_on() {
    gpio_put(RED, 0);
}

void red_off() {
    gpio_put(RED, 1);
}

void green_on() {
    gpio_put(GREEN, 0);
}

void green_off() {
    gpio_put(GREEN, 1);
}

void yellow_on() {
    gpio_put(RED, 0);
    gpio_put(GREEN, 0);
}

void yellow_off() {
    gpio_put(RED, 1);
    gpio_put(GREEN, 1);
}

int main() {
    gpio_init(RED);
    gpio_init(BLUE);
    gpio_init(GREEN);
    gpio_set_dir(RED, GPIO_OUT);
    gpio_set_dir(BLUE, GPIO_OUT);
    gpio_set_dir(GREEN, GPIO_OUT);
    gpio_put(RED, 1);
    gpio_put(GREEN, 1);
    gpio_put(BLUE, 1);
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
