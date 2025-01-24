#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "main_menu_page.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "boot_page.h"
#include "LryCommon.h"

#define DEFAULT_SCAN_LIST_SIZE 12
/* 链接wifi名称 */
#define DEFAULT_SSID        "CU-FB60"
/* wifi密码 */
#define DEFAULT_PWD         "dets2749"
/* 事件标志 */
static EventGroupHandle_t   wifi_event;
#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1
static const char *TAG = "static_ip";
static bool gl_got_ip = false;
static uint8_t u8WifiNetFlg = 0;

static const char *HTTP_TAG = "httpTask";
#define MAX_HTTP_OUTPUT_BUFFER 1300
#define HOST "api.seniverse.com"
#define UserKey "SvAKMWgTdChjFTIvr"
#define Location "xiamen"
#define Language "en"
#define Strat "0"
#define Days "5"

//天气解析结构体
typedef struct 
{
    char city[20];
    char weather_text[20];
    char weather_code[2];
    char temperatur[3];
}weather_info;

weather_info weathe;

/* WIFI默认配置 */
#define WIFICONFIG()   {                            \
    .sta = {                                        \
        .ssid = DEFAULT_SSID,                       \
        .password = DEFAULT_PWD,                    \
        .pmf_cfg.capable = 1  ,                 \
        .pmf_cfg.required = 0 ,                  \
        .threshold.authmode = WIFI_AUTH_WPA2_PSK,   \
    },                                              \
}

static void freeBuffer(char* buf, int bufSize){
	for( int i = 0; i < bufSize; i ++ )
		buf[i] = NULL;	
}

static void cJSON_parse_task(char* text)
{
    uint8_t code;
    uint8_t temp;
    cJSON *root,*psub;
    cJSON *arrayItem;

    //截取有效json
    char *index=strchr(text,'{');
    strcpy(text,index);
    root = cJSON_Parse(text);
    if(root!=NULL)
    {
        ESP_LOGI(TAG,"json");
        psub = cJSON_GetObjectItem(root, "results");
        arrayItem = cJSON_GetArrayItem(psub,0);

        cJSON *locat = cJSON_GetObjectItem(arrayItem, "location");
        cJSON *now = cJSON_GetObjectItem(arrayItem, "now");
        if((locat!=NULL)&&(now!=NULL))
        {
            psub=cJSON_GetObjectItem(locat,"name");
            sprintf(weathe.city,"%s",psub->valuestring);
            ESP_LOGI(TAG,"city:%s",weathe.city);
            
            psub=cJSON_GetObjectItem(now,"text");
            sprintf(weathe.weather_text,"%s",psub->valuestring);
            ESP_LOGI(TAG,"weather:%s",weathe.weather_text);
            LryLvglSetWeatherLarber(&weathe.weather_text);

            psub=cJSON_GetObjectItem(now,"code");
            sprintf(weathe.weather_code,"%s",psub->valuestring);
            memcpy(&code,&weathe.weather_code,1);
            ESP_LOGI(TAG,"code:%s",weathe.weather_code);
            LryLvglSetWeather(STEINGTURNHEX(code));

            psub=cJSON_GetObjectItem(now,"temperature");
            sprintf(weathe.temperatur,"%s",psub->valuestring);
            ESP_LOGI(TAG,"temperatur:%s",weathe.temperatur);
            //memcpy(&temp,&weathe.temperatur,1);
            LrtLvglSetTemperature(&weathe.temperatur);
        }
    }
    cJSON_Delete(root);
}

static void time_parse_task(char* text)
{
    cJSON *root,*psub;
    cJSON *arrayItem;
    char data[20];
    //截取有效json
    char *index=strchr(text,'{');
    strcpy(text,index);
    root = cJSON_Parse(text);
    if(root!=NULL)
    {
        psub = cJSON_GetObjectItem(root, "sysTime2");
        // ESP_LOGI(TAG,"suning");
        sprintf(data,"%s",psub->valuestring);
        ESP_LOGI(TAG,"time = %s",data);
        LryLvglSetTime(data);

    }
    cJSON_Delete(root);
}
/** HTTP functions **/
static void http_client_task(void *pvParameters)
{
	char output_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};   // Buffer to store response of http request
    int content_length = 0;                       
    esp_http_client_config_t config = {
        .url = "http://api.seniverse.com/v3/weather/now.json?key=SvAKMWgTdChjFTIvr&location=xiamen&language=zh-Hans&unit=c",        
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // GET Request
    esp_http_client_set_method(client, HTTP_METHOD_GET);
    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
       //ESP_LOGE(HTTP_TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
    } else {
        content_length = esp_http_client_fetch_headers(client);
        if (content_length < 0) {
            //ESP_LOGE(HTTP_TAG, "HTTP client fetch headers failed");
        } else {
            int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);
            if (data_read >= 0) {
                //ESP_LOGI(HTTP_TAG, "HTTP GET Status = %d, content_length = %d",
                //esp_http_client_get_status_code(client),
                //esp_http_client_get_content_length(client));
                //printf("data:%s", output_buffer);
				cJSON_parse_task(output_buffer);
                ESP_LOGI(TAG, "connect to the http Succeed");
            } else {
                //ESP_LOGE(HTTP_TAG, "Failed to read response");
            }
        }
    }
    esp_http_client_close(client);
	vTaskDelete(NULL);
}


static void http_time_task(void *pvParameters){
	char output_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};   // Buffer to store response of http request
    int content_length = 0;
	// static const char *URL = "http://"HOST"/v3/weather/daily.json?"	
	// 						 "key="UserKey"&location="Location		//
	// 						 "&language="Language					//
	// 						 "&unit=c&start="Strat"&days="Days;
    //static const char *URL = "https://api.seniverse.com/v3/weather/now.json?key=SvAKMWgTdChjFTIvr&location=xiamen&language=en&unit=c";
                              
    esp_http_client_config_t config = {
        .url = "http://quan.suning.com/getSysTime.do",        
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // GET Request
    esp_http_client_set_method(client, HTTP_METHOD_GET);
    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
       //ESP_LOGE(HTTP_TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
    } else {
        content_length = esp_http_client_fetch_headers(client);
        if (content_length < 0) {
            //ESP_LOGE(HTTP_TAG, "HTTP client fetch headers failed");
        } else {
            int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);
            if (data_read >= 0) {
                //ESP_LOGI(HTTP_TAG, "HTTP GET Status = %d, content_length = %d",
                //esp_http_client_get_status_code(client),
                //esp_http_client_get_content_length(client));
                //printf("data:%s", output_buffer);
				time_parse_task(output_buffer);
                ESP_LOGI(TAG, "connect to the suning Succeed");
            } else {
                //ESP_LOGE(HTTP_TAG, "Failed to read response");
            }
        }
    }
    esp_http_client_close(client);
	vTaskDelete(NULL);

    //vTaskDelay(pdMS_TO_TICKS(1));  /* 延时10毫秒 */
}


/**
 * @brief       WIFI链接糊掉函数
 * @param       arg:传入网卡控制块
 * @param       event_base:WIFI事件
 * @param       event_id:事件ID
 * @param       event_data:事件数据
 * @retval      无
 */
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    static int s_retry_num = 0;

    /* 扫描到要连接的WIFI事件 */
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    /* 连接WIFI事件 */
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED)
    {
        ESP_LOGI(TAG, "connect to the AP Succeed");
    }
    /* 连接WIFI失败事件 */
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        /* 尝试连接 */
        if (s_retry_num < 20)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        }
        else
        {
            //xEventGroupSetBits(wifi_event, WIFI_FAIL_BIT);
        }

        ESP_LOGI(TAG,"connect to the AP fail");
    }
    /* 工作站从连接的AP获得IP */
    else if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "static ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(wifi_event, WIFI_CONNECTED_BIT);
        xTaskCreate(http_client_task, "http_client", 5120, NULL, 3, NULL);
        xTaskCreate(http_time_task, "http_time", 5120, NULL, 3, NULL);
        u8WifiNetFlg = 1;
    }
}


void wifi_sta_init(void)
{
    static esp_netif_t *sta_netif = NULL;
    wifi_event= xEventGroupCreate();    /* 创建一个事件标志组 */
    /* 网卡初始化 */
    ESP_ERROR_CHECK(esp_netif_init());
    /* 创建新的事件循环 */
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    //assert(sta_netif);
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));  

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL) );
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL) );
      
    wifi_config_t  wifi_config = WIFICONFIG();
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    // /* 等待链接成功后、ip生成 */
    // EventBits_t bits = xEventGroupWaitBits(wifi_event,
    //         WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
    //         pdFALSE,
    //         pdFALSE,
    //         portMAX_DELAY);

    // /* 判断连接事件 */
    // if (bits & WIFI_CONNECTED_BIT)
    // {
    //     ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
    //              DEFAULT_SSID, DEFAULT_PWD);
    // }
    // else if (bits & WIFI_FAIL_BIT)
    // {
    //     ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
    //              DEFAULT_SSID, DEFAULT_PWD);
    // }
    // else
    // {
    //     ESP_LOGE(TAG, "UNEXPECTED EVENT");
    // }

    // vEventGroupDelete(wifi_event);
}

void wifi_scan(void)
{
    char lcd_buff[100] = {0};
    /* 网卡初始化 */
    ESP_ERROR_CHECK(esp_netif_init());
    /* 创建新的事件循环 */
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    /* 用户初始化 STA 模式 */
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);
    /* wifi 配置初始化 */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));
    /* 设置 WIFI 为 STA 模式 */
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    /* 启动 WIFI */
    ESP_ERROR_CHECK(esp_wifi_start());
    /* 开始扫描附件的 WIFI */
    esp_wifi_scan_start(NULL, true);
        /* 获取上次扫描中找到的 AP 数量 */
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    ESP_LOGI(TAG, "Total APs scanned = %u", ap_count);
    /* 获取上次扫描中找到的 AP 列表 */
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));

    /* 下面是打印附件的 WIFI 信息 */
    for (int i = 0; (i < DEFAULT_SCAN_LIST_SIZE) && (i < ap_count); i++)
    {
        sprintf(lcd_buff, "%s",ap_info[i].ssid);
        ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
        ESP_LOGI(TAG, "RSSI \t\t%d", ap_info[i].rssi);
        //print_auth_mode(ap_info[i].authmode);

        if (ap_info[i].authmode != WIFI_AUTH_WEP)
        {
            //print_cipher_type(ap_info[i].pairwise_cipher,ap_info[i].group_cipher);
        }
        ESP_LOGI(TAG, "Channel \t\t%d\n", ap_info[i].primary);
     }
}


