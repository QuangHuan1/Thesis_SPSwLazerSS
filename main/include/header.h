#include "nvs_flash.h"
#include "driver/uart.h"
#include "camera_header.h"
#include "protocol_common.h"
#include "datetime.h"
#include "stdbool.h"

// #define ESP32
#define ESP32CAM            // Define for Project using esp32cam as main MCU

//Where the MCU is used
#define GATE        0
#define AREA        1

//Determine whether the vehicle is entering or exiting
#define IDLE     0
#define CHECKIN     1
#define CHECKOUT    2


#define POSITION    GATE       //Place at GATE
#define TYPE        CHECKIN_OUT        //Checkin gate

// Declare Gate and Area Code Khu E
#define GATECODE_CHECKIN    "MC00"
#define GATECODE_CHECKOUT   "MC01"
#define AREACODE            "MK00"
#define REAL_AREACODE       "MK01"



// // Declare Gate and Area Code Khu B
// #define GATECODE_CHECKIN    "MC00"
// #define GATECODE_CHECKOUT    "MC01"
// #define AREACODE         "MK00"
// #define REAL_AREACODE    "MK02"


//Define distance for tracking car
#define MAX_DISTANCE_CM     450     // 5m max
#define FAR_THRESHOLD       300    //240 - 300
#define CAPTURE_THRESHOLD   270     //140 - 200
#define NEAR_THRESHOLD      150      //40 - 100
#define THRESHOLD_OFFSET    30      //30

#define BOUNDARY "X-ESPIDF_MULTIPART"

#define OFF     0
#define ON      1

#define TRUE true
#define FALSE false

#define ERROR_THRESHOLD 5
#define ACTION_THRESHOLD 3

#define DELAY_TIME 50



#define TAG_WIFI    "FROM WIFI"
#define TAG_POST    "FROM HTTP POST"
#define TAG_SENSOR  "FROM UltraSonic"
#define TAG_UART    "FROM RX UART"
#define TAG_DATE    "FROM DATETIME"
#define TAG_CAM     "FROM CAMERA"

#define RX_BUF_SIZE 1024

// Web server infomation related
typedef struct {
    char *web_server;
    char *web_port;
    char *post_image_checkin_path;
    char *post_image_checkout_path;
    char *post_checkin_path;
    char *post_checkout_path;
    char *post_checkin_area_path;
    char *post_checkout_area_path;

} server;

// Declare UART PIN
typedef struct {
    uart_port_t uart_num;
    gpio_num_t txd_pin;
    gpio_num_t rxd_pin;
} uart_pin;

// Declare SENSOR PIN
typedef struct {
    gpio_num_t sensor_trigger_pin;
    gpio_num_t sensor_echo_pin;
} sensor_pin;

// Appendable PIN......
typedef struct {
    gpio_num_t reader_trigger_pin;
    gpio_num_t LED_Wifi_Status;
    gpio_num_t LED_Sensor_Status;
} gpio_serveral;

// Declare SENSOR PIN
typedef struct {
    gpio_num_t lazer_front;
    gpio_num_t lazer_rear;
} lazer_pin;


char request_msg[1024];
char request_content[512];
char recv_buf[512];
char hexStr[40];
uint8_t checkin_state;
uint8_t checkout_state;
uint8_t allow_reader;
uint8_t allow_camera;
uint8_t capture_done;
uint8_t readtag_done;
uint8_t postimage_done;
uint8_t postetag_done;


uint8_t NO_CHECKIN;
uint8_t SHALL_CHECKIN;
uint8_t PREP_CHECKIN;
uint8_t DONE_CHECKIN;

uint8_t NO_CHECKOUT;
uint8_t SHALL_CHECKOUT;
uint8_t PREP_CHECKOUT;
uint8_t DONE_CHECKOUT;

uint8_t WORKING_STATE;


esp_err_t wifi_err;
esp_err_t sensor_err;


server server_infor;
uart_pin uart0;
sensor_pin sensor0;
gpio_serveral gpio0;
lazer_pin lazer0;





/**
 * @brief Post JSON data from MCU node to Server using HTTP POST method.
 *  
 * @param tagID Char pointer variable store EPC code that read from ETAG 
 * attached on CAR by UHF Reader.
 * E.g: E2 80 69 95 00 00 40 03 75 57 41 70 (in Hexa form)
 * 
 * @param path Char pointer variable store ROUTE in Server that determine 
 * JSON file will be POST using this PATH
 * E.g: "/check-in/check-in-area"
 */
void post_tagdata(char *tagID, char *path);
void post_warning(char *messgae, char *path);

/**
 * @brief Drive lamp to indicate error case.
 *  
 * @param arg none parameter used.
 *
 */
void indicator(void *arg);


/**
 * @brief Get data from sensor to determine working state.
 *  
 * @param arg None parameter used.
 *
 */
void lazer_sensor(void *arg);

/**
 * @brief From working state, control camera and reader to get data properly
 *  
 * @param arg None parameter used.
 *
 */
void check_state(void *pvParamters);

/**
 * @brief Get data from UHF-Reader
 *  
 * @param arg None parameter used.
 *
 */
void reader_readata(void *arg);


/**
 * @brief Init UART parameter and config for UART usage.
 *  
 * @param arg None parameter used.
 * 
 * @param return ESP_OK if UART initialization  & configuration success.
 */
esp_err_t init_uart(void);







