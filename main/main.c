#include "nvs_flash.h"
#include "iic.h"
#include "xl9555.h"
#include "es8388.h"
#include "i2s.h"
#include "lvgl_demo.h"
#include "gui_app.h"
#include "boot_page.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_dsp.h"

i2c_obj_t i2c0_master;

/**
 * @brief       程序入口
 * @param       无
 * @retval      无
 */
void app_main(void)
{
    esp_err_t ret;
    
    printf("Hello world!\n");

    ret = nvs_flash_init();             /* 初始化NVS */

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    i2c0_master = iic_init(I2C_NUM_0);  /* 初始化IIC0 */
    xl9555_init(i2c0_master);           /* IO扩展芯片初始化 */

    es8388_init(i2c0_master);           /* ES8388初始化 */

    i2s_init();                         /* I2S初始化 */

    lvgl_demo();                        /* 运行LVGL例程 */
}

  