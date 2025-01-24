#include "boot_page.h"
#include "lv_conf_template.h"
#include "lvgl.h"
#include "esp_log.h"
#include "LryCommon.h"


    char time1;
    char time2;
    char time3;
    char time4;
    char time5;
    char time6;
    

char ctime[10] = {'0'};


//字体
LV_FONT_DECLARE(LryWord);               //汉语
LV_FONT_DECLARE(LryWordDigit);          //数字
//图片 - 天气
LV_IMG_DECLARE(WeatherSun);             //晴天(白天)
LV_IMG_DECLARE(WeatherNight);           //晴天(夜晚)
LV_IMG_DECLARE(WeatherCloudy);          //多云
LV_IMG_DECLARE(WeatherSky);             //阴天
LV_IMG_DECLARE(WeatherShower);          //阵雨
LV_IMG_DECLARE(WeatherLightRain);       //小雨
LV_IMG_DECLARE(WeatherModerateRain);    //中雨
LV_IMG_DECLARE(WeatherHeavyRain);       //大雨

LV_IMG_DECLARE(temperature);            //温度
LV_IMG_DECLARE(humidity);               //湿度
LV_IMG_DECLARE(DegreeCentigrade);       //摄氏度

LV_IMG_DECLARE(spaceman);               //太空人
LV_IMG_DECLARE(spaceman1);               //太空人
LV_IMG_DECLARE(spaceman2);               //太空人

static const lv_img_dsc_t * anim_imgs[3] = {
    &spaceman,
    &spaceman1,
    &spaceman2,
};



lv_obj_t *ui_home;
lv_obj_t *ui_header;

lv_obj_t *ui_wifi;
lv_obj_t *lv_werther_name;
lv_obj_t *lv_werther_image;
lv_obj_t *ui_func1;
lv_obj_t *ui_func2;
lv_obj_t *ui_func3;
lv_obj_t *ui_func4;
lv_obj_t *label_time_hour;
lv_obj_t *label_time_min;
lv_obj_t *label_time_sec;
lv_obj_t *label_time_text;
lv_obj_t* lv_bar1;
lv_obj_t *lv_temperaturenumberber;
lv_obj_t* lv_DegreeCentigrade;

lv_obj_t *ui_setting;
lv_obj_t *ui_settingheader;
lv_obj_t *ui_settingmobile;



uint8_t time_s;
uint8_t time_m;
uint8_t time_z;

tNetTime NetTime;

void LrtLvglSetTemperature(char *u8temp)
{
    lv_bar_set_value(lv_bar1,STEINGTURNHEX(u8temp[0]),LV_ANIM_ON);
    lv_label_set_text(lv_temperaturenumberber,u8temp);
}

void LryLvglSetWeather(uint8_t cWeatherCode)
{
    switch (cWeatherCode)
    {
        case E_NUM_WERTHERSUN:
            lv_img_set_src(lv_werther_image, &WeatherSun);
            break;
        case E_NUM_WERTHERNIGHT:
            lv_img_set_src(lv_werther_image, &WeatherNight);
            break;
        case E_NUM_CLOUDY:
            lv_img_set_src(lv_werther_image, &WeatherCloudy);
            break;
        case E_NUM_RESERVESKY:
            lv_img_set_src(lv_werther_image, &WeatherSky);
            break;
        case E_NUM_WERTHERSHOWER:
            lv_img_set_src(lv_werther_image, &WeatherShower);
            break;
        case E_NUM_WERTHERLIGHTRAIN:
            lv_img_set_src(lv_werther_image, &WeatherLightRain);
            break;
        case E_NUM_WERTHERMODERATERAIN:
            lv_img_set_src(lv_werther_image, &WeatherModerateRain);
            break;
        case E_NUM_WERTHERHEAVYRAIN:
            lv_img_set_src(lv_werther_image, &WeatherHeavyRain);
            break;
        default:
            break;
    }
}
   
void LryLvglSetWeatherLarber(char *cWeatherName)
{
    lv_label_set_text(lv_werther_name,cWeatherName);
}

static void update_time_cb(lv_timer_t *timer)
{
    char hour[3]= {'0'};
    char min[3] = {'0'};
    char sec[3] = {'0'};


    if(++time_s >= 60)
    {
        time_s = 0;
        time_m ++;
    }
    if(time_m >= 60)
    {
        time_m = 0;
        time_z ++;
    }
    if(time_z >= 24)
    {
        time_z = 0;
    }
    ctime[0] = (time_z/10 + '0');
    ctime[1] = (time_z%10 + '0');
    ctime[3] = (time_m/10 + '0');
    ctime[4] = (time_m%10 + '0');
    ctime[6] = (time_s/10 + '0');
    ctime[7] = (time_s%10 + '0');

    memcpy(hour,&ctime[0],2); 
    memcpy(min,&ctime[3],2); 
    memcpy(sec,&ctime[6],2); 

    lv_label_set_text(label_time_hour,hour);
    lv_label_set_text(label_time_min,min);
    lv_label_set_text(label_time_sec,sec);
}

void LryLvglSetTime(char *data)
{
    char hour[3]= {'0'};
    char min[3] = {'0'};
    char sec[3] = {'0'};
    char text[20] = {'0'};

    memcpy(text,&data[0],10);
    memcpy(hour,&data[11],2); 
    memcpy(min,&data[14],2); 
    memcpy(sec,&data[17],2); 

    time_s = (data[17] - '0' ) * 10 + (data[18] - '0');
    time_m = (data[14] - '0' ) * 10 + (data[15] - '0');
    time_z = (data[11] - '0' ) * 10 + (data[12] - '0');

    // ESP_LOGI("bug", "NetTime.hour%s",NetTime.hour);
    // ESP_LOGI("bug", "NetTime.min%s",NetTime.min);
    // ESP_LOGI("bug", "NetTime.sec%s",NetTime.sec);

    lv_label_set_text(label_time_text,text);
    lv_label_set_text(label_time_hour,hour);
    lv_label_set_text(label_time_min,min);
    lv_label_set_text(label_time_sec,sec);
}

static void anim_ready_cb(lv_anim_t *a)
{
    obj_set_anim(a->var, 110, 320, 500, 2000, anim_set_y_cb, lv_obj_del_anim_ready_cb, lv_anim_path_ease_in);
}

void boot_page_run(void)
{

    //  /* 动态获取屏幕大小 */
    lv_coord_t scr_act_width = lv_obj_get_width(lv_scr_act());          //
    lv_coord_t scr_act_height = lv_obj_get_height(lv_scr_act());

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                              false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);

    ui_home = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_home, LV_OBJ_FLAG_SCROLLABLE); 
    lv_obj_set_style_bg_color(ui_home, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);  
    //lv_obj_set_style_bg_img_src(ui_home, &back, LV_PART_MAIN | LV_STATE_DEFAULT);
    //lv_obj_set_style_text_font(ui_home, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    /* ui head*/
    ui_header = lv_obj_create(ui_home);
    lv_obj_set_width(ui_header,scr_act_width);
    lv_obj_set_height(ui_header, 50);
    lv_obj_set_align(ui_header, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_header, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE); 
    lv_obj_set_style_radius(ui_header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_header, lv_color_hex(0x141414), LV_PART_MAIN | LV_STATE_DEFAULT);   //设置颜色
    lv_obj_set_style_bg_opa(ui_header, 255, LV_PART_MAIN | LV_STATE_DEFAULT);                        //设置背景透明度
    lv_obj_set_style_border_width(ui_header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_blend_mode(ui_header, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_header, 20, LV_PART_MAIN | LV_STATE_DEFAULT);


    /* ui head wifi*/
    ui_wifi = lv_label_create(ui_home);
    // lv_obj_set_width(ui_mobile, 50);  
    // lv_obj_set_height(ui_mobile, 50); 
    lv_obj_set_x(ui_wifi, 10);
    lv_obj_set_y(ui_wifi, 12);
    lv_label_set_text(ui_wifi, LV_SYMBOL_WIFI);
    lv_obj_clear_flag(ui_wifi, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                                     LV_OBJ_FLAG_SCROLL_CHAIN); 
    lv_obj_set_style_text_color(ui_wifi, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_wifi, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_wifi, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_setting = lv_obj_create(NULL);
    // lv_obj_clear_flag(ui_setting, LV_OBJ_FLAG_SCROLLABLE); 
    // lv_obj_set_style_radius(ui_setting, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(ui_setting, lv_color_hex(0xE6E6E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(ui_setting, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_font(ui_setting, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_settingheader = lv_obj_create(ui_setting);
    // lv_obj_set_width(ui_settingheader, scr_act_width);
    // lv_obj_set_height(ui_settingheader, 100);
    // lv_obj_set_align(ui_settingheader, LV_ALIGN_TOP_MID);
    // lv_obj_clear_flag(ui_settingheader, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    // lv_obj_set_style_radius(ui_settingheader, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(ui_settingheader, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(ui_settingheader, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_color(ui_settingheader, lv_color_hex(0xB4B4B4), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_opa(ui_settingheader, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_width(ui_settingheader, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_side(ui_settingheader, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_pad_left(ui_settingheader, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_pad_right(ui_settingheader, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_pad_top(ui_settingheader, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_pad_bottom(ui_settingheader, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_color(ui_settingheader, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_opa(ui_settingheader, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_font(ui_settingheader, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    //ui_settingmobile = lv_label_create(ui_settingheader);
    //lv_obj_set_width(ui_settingmobile, LV_SIZE_CONTENT);  /// 1
    //lv_obj_set_height(ui_settingmobile, LV_SIZE_CONTENT); /// 1
    //lv_obj_set_x(ui_settingmobile, 10);
    //lv_obj_set_y(ui_settingmobile, 7);
    //lv_label_set_text(ui_settingmobile, "中国移动");


    ui_func1 = lv_obj_create(ui_home);
    lv_obj_set_width(ui_func1, 150);  
    lv_obj_set_height(ui_func1, 100); 
    lv_obj_set_x(ui_func1, 50);
    lv_obj_set_y(ui_func1, 200);
    lv_obj_set_style_radius(ui_func1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_func1, lv_color_hex(0x141414), LV_PART_MAIN | LV_STATE_DEFAULT);   //设置颜色
    lv_obj_set_style_bg_opa(ui_func1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);                        //设置背景透明度
    lv_obj_set_style_opa(ui_func1, 20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_func2 = lv_obj_create(ui_home);
    lv_obj_set_width(ui_func2, 150);  
    lv_obj_set_height(ui_func2, 100); 
    lv_obj_set_x(ui_func2, 250);
    lv_obj_set_y(ui_func2, 200);
    lv_obj_set_style_radius(ui_func2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_func2, lv_color_hex(0x141414), LV_PART_MAIN | LV_STATE_DEFAULT);   //设置颜色
    lv_obj_set_style_bg_opa(ui_func2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);                        //设置背景透明度
    lv_obj_set_style_opa(ui_func2, 20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_func3 = lv_obj_create(ui_home);
    lv_obj_set_width(ui_func3, 150);  
    lv_obj_set_height(ui_func3, 100); 
    lv_obj_set_x(ui_func3, 50);
    lv_obj_set_y(ui_func3, 320);
    lv_obj_set_style_radius(ui_func3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_func3, lv_color_hex(0x141414), LV_PART_MAIN | LV_STATE_DEFAULT);   //设置颜色
    lv_obj_set_style_bg_opa(ui_func3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);                        //设置背景透明度
    lv_obj_set_style_opa(ui_func3, 20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_func4 = lv_obj_create(ui_home);
    lv_obj_set_width(ui_func4, 150);  
    lv_obj_set_height(ui_func4, 100); 
    lv_obj_set_x(ui_func4, 250);
    lv_obj_set_y(ui_func4, 320);
    lv_obj_set_style_radius(ui_func4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_func4, lv_color_hex(0x141414), LV_PART_MAIN | LV_STATE_DEFAULT);   //设置颜色
    lv_obj_set_style_bg_opa(ui_func4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);                        //设置背景透明度
    lv_obj_set_style_opa(ui_func4, 20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_werther_image = lv_img_create(ui_home);
    lv_obj_set_style_img_recolor_opa(lv_werther_image,0,LV_PART_MAIN);
    lv_obj_set_pos(lv_werther_image,50,60);

    lv_obj_t *lv_temperature = lv_img_create(ui_home);                                              //温度
    lv_obj_set_style_img_recolor_opa(lv_temperature,0,LV_PART_MAIN);
    lv_img_set_src(lv_temperature, &temperature);
    lv_obj_set_pos(lv_temperature,650,410);

    lv_obj_t *lv_humidity = lv_img_create(ui_home);                                                //湿度
    lv_obj_set_style_img_recolor_opa(lv_humidity,0,LV_PART_MAIN);
    lv_img_set_src(lv_humidity, &humidity);
    lv_obj_set_pos(lv_humidity,730,410);

    lv_temperaturenumberber = lv_label_create(ui_home);
    lv_obj_set_pos(lv_temperaturenumberber,650,180);
    lv_obj_set_style_text_font(lv_temperaturenumberber, &LryWord, LV_PART_MAIN);
    //lv_label_set_text(lv_temperaturenumberber,"00");
    lv_obj_set_style_bg_opa(lv_temperaturenumberber,0,LV_PART_MAIN);
    lv_obj_set_style_text_color(lv_temperaturenumberber,lv_color_hex(0xFFFFFF),LV_PART_MAIN);

    // lv_obj_t *lv_DegreeCentigrade = lv_img_create(ui_home);                                        //摄氏度
    // lv_obj_set_style_img_recolor_opa(lv_DegreeCentigrade,0,LV_PART_MAIN);
    // lv_img_set_src(lv_DegreeCentigrade, &DegreeCentigrade);
    // lv_obj_set_pos(lv_DegreeCentigrade,660,180);

    lv_obj_t* lv_location = lv_label_create(ui_home); 
    lv_obj_set_pos(lv_location,200,60);
    lv_obj_set_style_bg_opa(lv_location,0,LV_PART_MAIN);
    lv_label_set_text(lv_location,"厦门");
    lv_obj_set_style_text_font(lv_location, &LryWord, LV_PART_MAIN);
    lv_obj_set_style_text_color(lv_location,lv_color_hex(0xFFFFFF),LV_PART_MAIN);

    lv_werther_name = lv_label_create(ui_home); 
    lv_obj_set_pos(lv_werther_name,200,90);
    lv_obj_set_style_bg_opa(lv_werther_name,0,LV_PART_MAIN);
    lv_obj_set_style_text_font(lv_werther_name, &LryWord, LV_PART_MAIN);
    lv_obj_set_style_text_color(lv_werther_name,lv_color_hex(0xFFFFFF),LV_PART_MAIN);

    label_time_hour = lv_label_create(ui_home);
    //lv_obj_set_size(label_time, scr_act_width/2, scr_act_height * 2/10);
    lv_obj_set_pos(label_time_hour,250,60);
    lv_obj_set_style_text_font(label_time_hour, &LryWordDigit, LV_PART_MAIN);
    lv_label_set_text(label_time_hour,"00");
    lv_obj_set_style_bg_opa(label_time_hour,0,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_time_hour,lv_color_hex(0x01A0E8),LV_PART_MAIN);

    lv_obj_t *label_time_interval = lv_label_create(ui_home);
    lv_obj_set_pos(label_time_interval,400,60);
    lv_obj_set_style_text_font(label_time_interval, &LryWordDigit, LV_PART_MAIN);
    lv_label_set_text(label_time_interval,":");
    lv_obj_set_style_bg_opa(label_time_interval,0,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_time_interval,lv_color_hex(0xFFFFFF),LV_PART_MAIN);

    label_time_min = lv_label_create(ui_home);
    //lv_obj_set_size(label_time, scr_act_width/2, scr_act_height * 2/10);
    lv_obj_set_pos(label_time_min,450,60);
    lv_obj_set_style_text_font(label_time_min, &LryWordDigit, LV_PART_MAIN);
    lv_label_set_text(label_time_min,"00");
    lv_obj_set_style_bg_opa(label_time_min,0,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_time_min,lv_color_hex(0xE70082),LV_PART_MAIN);

    label_time_sec = lv_label_create(ui_home);
    //lv_obj_set_size(label_time, scr_act_width/2, scr_act_height * 2/10);
    lv_obj_set_pos(label_time_sec,600,100);
    lv_obj_set_style_text_font(label_time_sec, &lv_font_montserrat_30, LV_PART_MAIN);
    lv_label_set_text(label_time_sec,"00");
    lv_obj_set_style_bg_opa(label_time_sec,0,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_time_sec,lv_color_hex(0xFFFFFF),LV_PART_MAIN);
    lv_timer_create(update_time_cb, 1000, NULL);//创建一个定时器，每秒更新一次日期、时间、时长

    label_time_text = lv_label_create(ui_home);
    //lv_obj_set_size(label_time, scr_act_width/2, scr_act_height * 2/10);
    lv_obj_set_pos(label_time_text,360,160);
    lv_obj_set_style_text_font(label_time_text, &LryWord, LV_PART_MAIN);
    lv_label_set_text(label_time_text,"00");
    lv_obj_set_style_bg_opa(label_time_text,0,LV_PART_MAIN);
    lv_obj_set_style_text_color(label_time_text,lv_color_hex(0xFFFFFF),LV_PART_MAIN);

    
    lv_bar1 = lv_bar_create(ui_home); 
    lv_obj_set_size(lv_bar1, 25, 200); 
    lv_obj_set_pos(lv_bar1,650,200);
    lv_bar_set_value(lv_bar1,20,LV_ANIM_ON);
    lv_bar_set_range(lv_bar1,-20,40);
    lv_obj_set_style_anim_time(lv_bar1,500,0);
    lv_obj_set_style_bg_color(lv_bar1,lv_color_hex(0xFF2424),LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(lv_bar1,lv_color_hex(0xEC1829),LV_PART_MAIN|LV_STATE_DEFAULT);

    lv_obj_t* lv_bar2 = lv_bar_create(ui_home); 
    lv_obj_set_size(lv_bar2, 25, 200); 
    lv_obj_set_pos(lv_bar2,730,200);
    lv_bar_set_value(lv_bar2,20,LV_ANIM_ON);
    lv_bar_set_range(lv_bar2,0,100);
    lv_obj_set_style_anim_time(lv_bar2,500,0);
    lv_obj_set_style_bg_color(lv_bar2,lv_color_hex(0x5FA1AD),LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(lv_bar2,lv_color_hex(0x0A118D),LV_PART_MAIN|LV_STATE_DEFAULT);

    // lv_obj_t *animimg = lv_img_create(ui_home);                                              //温度
    // lv_obj_set_style_img_recolor_opa(animimg,0,LV_PART_MAIN);
    // lv_img_set_src(animimg, &spaceman);
    // lv_obj_set_pos(animimg,410,205);
    // lv_obj_t *img = lv_gif_create(ui_home);
    // lv_gif_set_src(img, &spaceman);
    // lv_obj_set_pos(img, 400, 140);
    lv_obj_t * animimg = lv_animimg_create(ui_home);
    lv_obj_set_style_img_recolor_opa(animimg,200,LV_PART_MAIN);
    lv_animimg_set_src(animimg, (lv_img_dsc_t**)anim_imgs, 3);
    lv_animimg_set_duration(animimg, 1000);
    lv_obj_set_pos(animimg,420,205);
    lv_animimg_set_repeat_count(animimg, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(animimg);


    lv_disp_load_scr(ui_home);

}





