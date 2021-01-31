#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"


int pp_gpios[] = {7,8,9,10,11};
int PP_GPIO_LEN=5;

void pp_init() {
    for (int i=0; i<PP_GPIO_LEN; i++) {
        gpio_reset_pin(pp_gpios[i]);
        gpio_set_direction(pp_gpios[i], GPIO_MODE_OUTPUT);
    }
}

void pp_clear() {
    for (int i=0; i<PP_GPIO_LEN; i++) {
        gpio_set_level(pp_gpios[i], 0);
        printf("Turning off the PP resistor (gpio: %d)\n", pp_gpios[i]);
    }
}

void pp(int gpio) {
    gpio_set_level(gpio, 1);
    printf("Turning on the PP resistor (gpio: %d)\n", gpio);
}

void pp_13a() {
    pp_clear();
    pp(10);
}

void pp_20a() {
    pp_clear();
    pp(9);
}

void pp_32a() {
    pp_clear();
    pp(8);
}

void pp_63a() {
    pp_clear();
    pp(7);
}

void pp_low() {
    pp_clear();
    pp(11);
}

void pp_high() {
    pp(7);
    pp(8);
    pp(9);
    pp(10);
    pp(11);
}