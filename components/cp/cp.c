#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

int cp_gpios[] = {37,38,39,40};
int CP_GPIO_LEN = 4;

void cp_init() {
    for (int i=0; i<CP_GPIO_LEN; i++) {
        gpio_reset_pin(cp_gpios[i]);
        gpio_set_direction(cp_gpios[i], GPIO_MODE_OUTPUT);
    }
}

void cp_relais_clear() {
    for (int i=0; i<CP_GPIO_LEN; i++) {
        gpio_set_level(cp_gpios[i], 0);
        printf("Turning off the CP resistor (gpio: %d)\n", cp_gpios[i]);
    }
}

void cp(int gpio) {
    gpio_set_level(gpio, 1);
    printf("Turning on the CP resistor (gpio: %d)\n", gpio);
}

void saeJ1772_state_a() {
    cp_relais_clear();
}

void saeJ1772_state_b() {
    cp_relais_clear();
    cp(37);
}

void saeJ1772_state_c() {
    cp_relais_clear();
    cp(37);
    cp(38);
}

void saeJ1772_state_diode_fault() {
    cp_relais_clear();
    cp(39);
}

void saeJ1772_state_e() {
    cp_relais_clear();
    cp(40);
}
