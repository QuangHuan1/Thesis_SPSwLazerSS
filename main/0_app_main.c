/* 
    KLTN designed by Quyen DQ & Huan TQ
*/
#include "header.h"

server server_infor = {
    .web_server = "192.168.1.8",
    .web_port = "3000",
    .post_image_checkin_path = "/check-in-out-image/check-in",
    .post_image_checkout_path = "/check-in-out-image/check-out",
    .post_checkin_path = "/check-in",
    .post_checkout_path = "/check-out",
    .post_checkin_area_path = "/check-in/check-in-area",
    .post_checkout_area_path = "/check-out/check-out-area"
};

#ifdef ESP32CAM
uart_pin uart0 = {
    .uart_num = UART_NUM_0,
    .txd_pin = GPIO_NUM_1,
    .rxd_pin = GPIO_NUM_3,
};

gpio_serveral gpio0 = {
    .reader_trigger_pin = GPIO_NUM_12,
    .LED_Wifi_Status = GPIO_NUM_14,
    .LED_Sensor_Status = GPIO_NUM_2,
};

lazer_pin lazer0 = {
    .lazer_front = GPIO_NUM_15,
    .lazer_rear = GPIO_NUM_13,
};
#endif


void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    gpio_pad_select_gpio (gpio0.reader_trigger_pin);
    gpio_set_direction(gpio0.reader_trigger_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio0.reader_trigger_pin, 0);

    gpio_pad_select_gpio (gpio0.LED_Wifi_Status);
    gpio_set_direction(gpio0.LED_Wifi_Status, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio0.LED_Wifi_Status, 1);

    gpio_pad_select_gpio (gpio0.LED_Sensor_Status);
    gpio_set_direction(gpio0.LED_Sensor_Status, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio0.LED_Sensor_Status, 1);

    gpio_pad_select_gpio (lazer0.lazer_front);
    gpio_set_direction(lazer0.lazer_front, GPIO_MODE_INPUT);
    // gpio_set_pull_mode (lazer0.lazer_front, GPIO_PULLDOWN_ONLY);
    
    gpio_pad_select_gpio (lazer0.lazer_rear);
    gpio_set_direction(lazer0.lazer_rear, GPIO_MODE_INPUT);


    wifi_err = 1;
    sensor_err = 1;
    xTaskCreatePinnedToCore(&indicator, "indicator", 1024, NULL, 1, NULL, 0);

    allow_reader = FALSE;
    allow_camera = FALSE;
    capture_done = FALSE;
    readtag_done = FALSE;
    postetag_done = FALSE;
    postimage_done = FALSE;

    // NO_CHECKIN = FALSE;
    // SHALL_CHECKIN = FALSE;
    // PREP_CHECKIN = FALSE;
    // DONE_CHECKIN = FALSE;

    // NO_CHECKOUT = FALSE;
    // SHALL_CHECKOUT = FALSE;
    // PREP_CHECKOUT = FALSE;
    // DONE_CHECKOUT = FALSE;
    esp_log_level_set("*", ESP_LOG_INFO);
    // esp_log_level_set("*", ESP_LOG_NONE);

    wifi_err = wifi_connect();
    if (wifi_err == ESP_OK)
    {
        if (initiate_camera() != ESP_OK)
        {
            ESP_LOGE(TAG_CAM, "Error while init camera.\n");
            return;
        }
        ESP_ERROR_CHECK(init_uart());
        Set_SystemTime_SNTP();
        vTaskDelay(500 / portTICK_PERIOD_MS);
        gpio_set_pull_mode (lazer0.lazer_front, GPIO_PULLDOWN_ONLY);
        gpio_set_pull_mode (lazer0.lazer_rear, GPIO_PULLDOWN_ONLY);

        
        xTaskCreatePinnedToCore(&lazer_sensor, "lazer_sensor", 1024*2, NULL, 0, NULL, 0);
        xTaskCreatePinnedToCore(&check_state, "check_state", 1024*2, NULL, 2, NULL, 0);
        xTaskCreatePinnedToCore(&camera_capture, "camera_capture", 1024*6, NULL, 0, NULL, 1);
        xTaskCreatePinnedToCore(&reader_readata, "reader_readata", 1024*6, NULL, 1, NULL, 1);

        ESP_LOGI(TAG_CAM, "Tasks is created and running\n");
    }
    else{
        ESP_LOGI(TAG_CAM, "Failed to connected with Wi-Fi\n");
    }
}
