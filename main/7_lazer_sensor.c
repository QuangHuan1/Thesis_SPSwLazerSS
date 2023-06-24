#include "header.h"

void lazer_sensor(void *arg)
{   
    while(1){
        volatile uint8_t front = gpio_get_level(lazer0.lazer_front);
        volatile uint8_t rear = gpio_get_level(lazer0.lazer_rear);

        printf("FRONT %d\n",front);
        printf("REAR %d\n",rear);
        size_t heapUsedAtStart;
        heapUsedAtStart = heap_caps_get_free_size(MALLOC_CAP_8BIT);
        printf("Heap free at start %d.\n", heapUsedAtStart);

        if(front == 1 && rear == 0 ){
            WORKING_STATE = CHECKIN;
            Get_current_date_time(Current_Date_Time, Current_Date_Time_Raw);
            
            ESP_LOGI(TAG_CAM, "Working in CHECK-IN state!");
            vTaskDelay( 10000 / portTICK_PERIOD_MS);

        }
        else if (front == 0 && rear == 1){
            WORKING_STATE = CHECKOUT;
            Get_current_date_time(Current_Date_Time, Current_Date_Time_Raw);

            ESP_LOGI(TAG_CAM, "Working in CHECK-OUT state!");
            vTaskDelay( 10000 / portTICK_PERIOD_MS);

        }
        else{
            ESP_LOGI(TAG_CAM, "Working in IDLE state!");
            WORKING_STATE = IDLE;
        }
        vTaskDelay(DELAY_TIME / portTICK_PERIOD_MS);
    }
}