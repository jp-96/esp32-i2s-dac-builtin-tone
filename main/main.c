/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_timer.h"
#include "driver/gpio.h"

#include "../lib/i2sdacbuiltintone.h"

#define COUNTDOWN_MODE_15       (15)
#define COUNTDOWN_MODE_30       (30)
#define COUNTDOWN_MODE_60       (60)
#define COUNTDOWN_MODE_90       (90)
#define COUNTDOWN_CYCLE_SECONDS (180)

#define COUNTDOWN_30    (30)
#define COUNTDOWN_20    (20)
#define COUNTDOWN_10    (10)
#define COUNTDOWN_05    (5)
#define COUNTDOWN_00    (0)

#define GPIO_PLUS_30    (GPIO_NUM_27)
#define GPIO_PLUS_60    (GPIO_NUM_14)

void init_gpio_input()
{    
    gpio_set_direction(GPIO_PLUS_60,GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_PLUS_60,GPIO_PULLUP_ONLY);
    gpio_set_direction(GPIO_PLUS_30,GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_PLUS_30,GPIO_PULLUP_ONLY);
}

int16_t get_starting_in_seconds()
{
    int val = gpio_get_level(GPIO_PLUS_60)*2 + gpio_get_level(GPIO_PLUS_30);
    if (val==0)
        return COUNTDOWN_MODE_90;   // GPIO_PLUS_60(GND) + GPIO_PLUS_30(GND)
    if (val==1)
        return COUNTDOWN_MODE_60;   // GPIO_PLUS_60(GND)
    if (val==2)
        return COUNTDOWN_MODE_30;   // GPIO_PLUS_30(GND)
    return COUNTDOWN_MODE_15;       // (PULL UP)
}

void app_main(void)
{
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    
    init_gpio_input();
    i2sdacbuiltin_init();
    int64_t nextTime = (esp_timer_get_time()/1000000)*1000000 + 1000000;
    int count = 0;
    for (;;) {
        int countdown = (COUNTDOWN_CYCLE_SECONDS-count) % get_starting_in_seconds();
        printf("Starting in %d seconds...\n", countdown);
        if (countdown==COUNTDOWN_30)
        {
            i2sdacbuiltin_start();
            i2sdacbuiltin_tone(880, 250);   // A5 ラ5
            i2sdacbuiltin_tone(1760, 250);  // A6 ラ6
            i2sdacbuiltin_stop();    
        }
        else if (countdown==COUNTDOWN_20)
        {
            i2sdacbuiltin_start();
            i2sdacbuiltin_tone(880, 250);   // A5 ラ5
            i2sdacbuiltin_stop();    
        }
        else if (countdown==COUNTDOWN_10)
        {
            i2sdacbuiltin_start();
            i2sdacbuiltin_tone(880, 500);   // A5 ラ5
            i2sdacbuiltin_stop();
        }
        else if (countdown<=COUNTDOWN_05 && countdown>COUNTDOWN_00)
        {
            i2sdacbuiltin_start();
            i2sdacbuiltin_tone(880, 250);   // A5 ラ5
            i2sdacbuiltin_stop();
        }
        else if (countdown==COUNTDOWN_00)
        {
            i2sdacbuiltin_start();
            i2sdacbuiltin_tone(1760, 750);   // A6 ラ6
            i2sdacbuiltin_stop();
        }
        int64_t delay = (nextTime - esp_timer_get_time()) / (1000 * portTICK_PERIOD_MS);
        if (delay<0)
            delay = 0;
        vTaskDelay(delay);
        nextTime = nextTime + 1000000;
        count = (count+1) % COUNTDOWN_CYCLE_SECONDS;
    }
    printf("Restarting now.\n");
    fflush(stdout);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    esp_restart();
}
