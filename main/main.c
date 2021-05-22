
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "ssd1306.h"
#include "ssd1306_draw.h"
#include "ssd1306_font.h"
#include "ssd1306_default_if.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "rotary.h"
int _rotary;

#include "pp.h"
#include "cp.h"

static const int I2CDisplayAddress = 0x3C;
static const int I2CDisplayWidth = 128;
static const int I2CDisplayHeight = 64;
static const int I2CResetPin = -1;

struct SSD1306_Device I2CDisplay;


bool DefaultBusInit( void ) {
    assert( SSD1306_I2CMasterInitDefault( ) == true);
    assert( SSD1306_I2CMasterAttachDisplayDefault( &I2CDisplay, I2CDisplayWidth, I2CDisplayHeight, I2CDisplayAddress, I2CResetPin ) == true);

    return true;
}

void display_set_font( struct SSD1306_Device* DisplayHandle, const struct SSD1306_FontDef* Font ) {
    SSD1306_SetFont( DisplayHandle, Font);
}

void switch_on_display_power() {
    gpio_reset_pin(42);
    gpio_set_direction(42, GPIO_MODE_OUTPUT);
    gpio_set_level(42, 0);
    printf("display power switched on....\n");
}

void debugi2c() {
    printf("CONFIG_SSD1306_DEFAULT_I2C_SDA_PIN %d...\n", CONFIG_SSD1306_DEFAULT_I2C_SDA_PIN);
    printf("CONFIG_SSD1306_DEFAULT_I2C_SCL_PIN %d...\n", CONFIG_SSD1306_DEFAULT_I2C_SCL_PIN);
    printf("CONFIG_SSD1306_DEFAULT_I2C_SPEED %d...\n", CONFIG_SSD1306_DEFAULT_I2C_SPEED);
}


void display_update(struct SSD1306_Device* DisplayHandle) {

    SSD1306_Clear( DisplayHandle, SSD_COLOR_BLACK);
    SSD1306_DrawBox(DisplayHandle, 0,_rotary*10, 127,_rotary*10+ 10, SSD_COLOR_WHITE, 1);
    SSD1306_FontDrawString( DisplayHandle, 0, 0, "asdf", _rotary == 1?SSD_COLOR_WHITE : SSD_COLOR_BLACK);
    SSD1306_FontDrawString( DisplayHandle, 0, 10, "aaaaa", _rotary == 2?SSD_COLOR_WHITE : SSD_COLOR_BLACK);

    SSD1306_Update( DisplayHandle);
}

void initdisplay() {
    if ( DefaultBusInit( ) == true ) {
        printf("conntected to display.\n");
        display_set_font( &I2CDisplay, &Font_droid_sans_fallback_11x13);
        display_update( &I2CDisplay);
    }
}

void rotary_rotated(int rotary) {
    printf("rotary_rotated from main %d...\n", rotary);
    _rotary = rotary;

    display_update(&I2CDisplay);
}

void rotary_buttonpressed() {
    printf("rotary_buttonpressed from main...\n");
//    display_update(&I2CDisplay);
}

void test_pp() {
    pp_out_of_spec_too_low();
    pp_13a();
    pp_20a();
    pp_32a();
    pp_63a();
    pp_out_of_spec_too_high();
}

void test_cp() {
    saeJ1772_state_e();
    vTaskDelay(100/portTICK_PERIOD_MS);
    saeJ1772_state_a();
    vTaskDelay(100/portTICK_PERIOD_MS);
    saeJ1772_state_b();
    vTaskDelay(100/portTICK_PERIOD_MS);
    saeJ1772_state_c();
    vTaskDelay(100/portTICK_PERIOD_MS);
    saeJ1772_state_diode_fault();
    vTaskDelay(100/portTICK_PERIOD_MS);
}


void app_main( void ) {
    printf("openevsetester starting...\n");

    pp_init();
    test_pp();

    cp_init();
    test_cp();

    switch_on_display_power();

    debugi2c();
    
    initdisplay();

    rotary_register_rotary_callback(&rotary_rotated);
    rotary_register_buttonpressed_callback(&rotary_buttonpressed);
    rotary_start();
}
