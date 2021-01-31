/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "rotary.h"
static xQueueHandle gpio_evt_queue = NULL;

/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 *
 * GPIO status:
 * GPIO18: output
 * GPIO19: output
 * GPIO4:  input, pulled up, interrupt from rising edge and falling edge
 * GPIO5:  input, pulled up, interrupt from rising edge.
 *
 * Test:
 * Connect GPIO18 with GPIO4
 * Connect GPIO19 with GPIO5
 * Generate pulses on GPIO18/19, that triggers interrupt on GPIO4/5
 *
 */

#define GPIO_INPUT_IO_0     4
#define GPIO_INPUT_IO_1     5
#define GPIO_INPUT_IO_2     6
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) | (1ULL<<GPIO_INPUT_IO_1) | (1ULL<<GPIO_INPUT_IO_2))
#define ESP_INTR_FLAG_DEFAULT 0





static int rotary=0;

static int levelA=0;    
static int levelB=0;    
static int lasta = 0;
static int lastb = 0;

static int count=0;



struct Rotaryevent {
	uint32_t gpio;
	int rotary;
} rotaryevent;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;

    rotaryevent.gpio = gpio_num;

    if (gpio_num == GPIO_INPUT_IO_2) {
        xQueueSendFromISR(gpio_evt_queue, &rotaryevent, NULL);
        return;
    }

    levelA = gpio_get_level(4);
    levelB = gpio_get_level(5);

    if (levelA != lasta) {
        count++;
        if (count==2) {
            count=0;

            if(lastb == 1 && levelB==0)
                rotary++;

            if(lastb == 0 && levelB==1)
                rotary--;

            rotaryevent.rotary = rotary;
            

            xQueueSendFromISR(gpio_evt_queue, &rotaryevent, NULL);
        }
        lastb=levelB;
    }

    lasta=levelA;
}

void (*callback_func)(int) = NULL;
void rotary_register_rotary_callback(void* func) {
    callback_func = func;
}

void (*callback_button_func)() = NULL;
void rotary_register_buttonpressed_callback(void* func) {
    callback_button_func = func;
}


static void gpio_task_example(void* arg)
{
    uint32_t io_num;

    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &rotaryevent, portMAX_DELAY)) {

            if (rotaryevent.gpio == GPIO_INPUT_IO_2) {
                if (callback_button_func != NULL)
                    (callback_button_func)();
            }

            else
            //printf("rotary: %d \n", rotary);
                if (callback_func != NULL)
                    (callback_func)(rotary);

        }
    }
}




void rotary_start(void)
{
    gpio_config_t io_conf;

    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    //change gpio intrrupt type for one pin
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE);
    gpio_set_intr_type(GPIO_INPUT_IO_1, GPIO_INTR_ANYEDGE);
    gpio_set_intr_type(GPIO_INPUT_IO_2, GPIO_INTR_POSEDGE);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);
    gpio_isr_handler_add(GPIO_INPUT_IO_2, gpio_isr_handler, (void*) GPIO_INPUT_IO_2);

    //gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);
    //hook isr handler for specific gpio pin
    //gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);

    //remove isr handler for gpio number.
    //gpio_isr_handler_remove(GPIO_INPUT_IO_0);
    //hook isr handler for specific gpio pin again
    //gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());
}
