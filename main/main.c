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

#include "../lib/i2sdacbuiltintone.h"

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
    
    i2sdacbuiltin_init();
    int64_t nextTime = (esp_timer_get_time()/1000000)*1000000 + 1000000;
    int cnt = 0;
    for (;;) {
        for (int i = 29; i >= 0; i--) {
            printf("nextTime: %lld \n", nextTime/1000);
            printf("[%d] Restarting in %d seconds...\n", cnt, i);
            if (i==20)
            {
                i2sdacbuiltin_start();
                i2sdacbuiltin_tone(880, 250);   // A5 ラ5
                i2sdacbuiltin_tone(1760, 250);  // A6 ラ6
                i2sdacbuiltin_stop();    
            }
            else if (i==10)
            {
                i2sdacbuiltin_start();
                i2sdacbuiltin_tone(880, 500);   // A5 ラ5
                i2sdacbuiltin_stop();
            }
            else if (i<6 && i>0)
            {
                i2sdacbuiltin_start();
                i2sdacbuiltin_tone(880, 250);   // A5 ラ5
                i2sdacbuiltin_stop();
            }
            else if (i==0)
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
        }
        cnt++;
    }
    printf("Restarting now.\n");
    fflush(stdout);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    esp_restart();
}
