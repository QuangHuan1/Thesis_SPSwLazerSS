
#include "header.h" 

void check_state(void *pvParamters)
{
    while(1){
        ESP_LOGI(TAG_SENSOR, "WORKING_STATE state %d ", WORKING_STATE);

        if(WORKING_STATE == CHECKIN){
            if (PREP_CHECKIN)
            {
                ESP_LOGI(TAG_SENSOR, "PREP_CHECKIN state %d ", PREP_CHECKIN);
                Get_current_date_time(Current_Date_Time, Current_Date_Time_Raw);
                allow_camera = (capture_done == FALSE) ? TRUE : FALSE;
                allow_reader = (readtag_done == FALSE) ? TRUE : FALSE;

                DONE_CHECKIN = (readtag_done == TRUE && capture_done == TRUE) ? TRUE : FALSE;
                vTaskDelay(DELAY_TIME * 2/portTICK_PERIOD_MS);
            }
            else if(DONE_CHECKIN)
            {
                ESP_LOGI(TAG_SENSOR, "DONE_CHECKIN state %d ", DONE_CHECKIN);
                uint8_t count = 0;
                while(!(postetag_done || count == 10)){
                    count++;
                    vTaskDelay(500/portTICK_PERIOD_MS);
                }
                postetag_done = FALSE;
            }else{
                capture_done = FALSE;
                readtag_done = FALSE;
                allow_camera = FALSE;
                allow_reader = FALSE;
            }
            vTaskDelay(DELAY_TIME / portTICK_PERIOD_MS);
        }
        else if(WORKING_STATE == CHECKOUT){
            if (PREP_CHECKOUT)
            {
                ESP_LOGI(TAG_SENSOR, "PREP_CHECKOUT state %d ", PREP_CHECKOUT);
                Get_current_date_time(Current_Date_Time, Current_Date_Time_Raw);
                allow_camera = (capture_done == FALSE) ? TRUE : FALSE;
                allow_reader = (readtag_done == FALSE) ? TRUE : FALSE;

                DONE_CHECKOUT = (readtag_done == TRUE && capture_done == TRUE) ? TRUE : FALSE;
                vTaskDelay(DELAY_TIME * 2/portTICK_PERIOD_MS);

            }
            else if (DONE_CHECKOUT)
            {
                ESP_LOGI(TAG_SENSOR, "DONE_CHECKOUT state %d ", DONE_CHECKOUT);
                uint8_t count = 0;
                while(!(postetag_done || count == 10)){
                    count++;
                    vTaskDelay(500/portTICK_PERIOD_MS);
                }
                postetag_done = FALSE;
            }
            else{
                capture_done = FALSE;
                readtag_done = FALSE;
                allow_camera = FALSE;
                allow_reader = FALSE;
            }
            vTaskDelay(DELAY_TIME / portTICK_PERIOD_MS);
        }
        
        vTaskDelay(DELAY_TIME / portTICK_PERIOD_MS);
	}
}
