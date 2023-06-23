
#include "header.h" 

void check_state(void *pvParamters)
{
    while(1){

        if(WORKING_STATE == CHECKIN){
            allow_camera = (capture_done == FALSE) ? TRUE : FALSE;
            allow_reader = (readtag_done == FALSE) ? TRUE : FALSE;
        }
        else if(WORKING_STATE == CHECKOUT){
            allow_camera = (capture_done == FALSE) ? TRUE : FALSE;
            allow_reader = (readtag_done == FALSE) ? TRUE : FALSE;
        }
        else {
            capture_done = FALSE;
            readtag_done = FALSE;
            postimage_done = FALSE;
            postetag_done = FALSE;
            allow_camera = FALSE;
            allow_reader = FALSE;
        }
        
        vTaskDelay(DELAY_TIME / portTICK_PERIOD_MS);
	}
}
