#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

int pe_gpio = 36;

void pe_connect() {
    gpio_set_level(pe_gpio, 0);
}

void pe_breaker_init() {
    gpio_reset_pin(pe_gpio);
    gpio_set_direction(pe_gpio, GPIO_MODE_OUTPUT);
    pe_connect();
}

void pe_break() {
    gpio_set_level(pe_gpio, 1);
}
