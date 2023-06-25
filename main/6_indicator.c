#include "header.h"

void indicator(void *arg)
{   
    while(1){
        gpio_set_level(gpio0.LED_Wifi_Status, 1);
        gpio_set_level(gpio0.LED_Sensor_Status, 0);
        
        if(wifi_err != ESP_OK){
            vTaskDelay(200/ portTICK_PERIOD_MS);
            if(wifi_err != ESP_OK){
                gpio_set_level(gpio0.LED_Wifi_Status, 0);
            }
        }
        
        if(WORKING_STATE == CHECKIN || WORKING_STATE == CHECKOUT){
            gpio_set_level(gpio0.LED_Sensor_Status, 1);
        }
        vTaskDelay(100/ portTICK_PERIOD_MS);
    }
}
