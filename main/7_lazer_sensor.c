#include "header.h"

void lazer_sensor(void *arg)
{   
    while(1){
        if(gpio_get_level(lazer0.lazer_front) && !gpio_get_level(lazer0.lazer_rear)){
            WORKING_STATE = CHECKIN;
            while (1)
            {   
                uint8_t lazer_front_value = gpio_get_level(lazer0.lazer_front);
                uint8_t lazer_rear_value = gpio_get_level(lazer0.lazer_rear);

                if(lazer_front_value && lazer_rear_value){
                    PREP_CHECKIN = TRUE;
                    DONE_CHECKIN = FALSE;
                }
                else if(!lazer_front_value && lazer_rear_value){
                    PREP_CHECKIN = FALSE;
                    DONE_CHECKIN = TRUE;
                }
                else if(!lazer_front_value && !lazer_rear_value){
                    WORKING_STATE = IDLE;
                    PREP_CHECKIN = FALSE;
                    DONE_CHECKIN = FALSE;
                    break;
                }

                vTaskDelay(5 / portTICK_PERIOD_MS);
            }
            
        }
        else if (!gpio_get_level(lazer0.lazer_front) && gpio_get_level(lazer0.lazer_rear)){
            WORKING_STATE = CHECKOUT;
            while (1)
            {
                uint8_t lazer_front_value = gpio_get_level(lazer0.lazer_front);
                uint8_t lazer_rear_value = gpio_get_level(lazer0.lazer_rear);

                if(lazer_front_value && lazer_rear_value){
                    PREP_CHECKOUT = TRUE;
                    DONE_CHECKOUT = FALSE;
                }
                else if(lazer_front_value && !lazer_rear_value){
                    PREP_CHECKOUT = FALSE;
                    DONE_CHECKOUT = TRUE;
                }
                else if(!lazer_front_value && !lazer_rear_value){
                    WORKING_STATE = IDLE;
                    PREP_CHECKOUT = FALSE;
                    DONE_CHECKOUT = FALSE;
                    break;
                }
                vTaskDelay(5 / portTICK_PERIOD_MS);
            }
        }
        else{
            WORKING_STATE = IDLE;
        }

        vTaskDelay(DELAY_TIME / portTICK_PERIOD_MS);
    }
}