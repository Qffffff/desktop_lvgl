#ifndef __BOOT_PAGE_H
#define __BOOT_PAGE_H

#include "lvgl.h"
#include "anim.h"
#include "main_menu_page.h"


typedef enum 
{
    E_NUM_WERTHERSUN = 0,
    E_NUM_WERTHERNIGHT ,
    E_NUM_RESERVE1 ,
    E_NUM_RESERVE2 ,
    E_NUM_CLOUDY ,
    E_NUM_RESERVE3 ,
    E_NUM_RESERVE4 ,
    E_NUM_RESERVE5 ,
    E_NUM_RESERVE6 ,
    E_NUM_RESERVESKY ,
    E_NUM_WERTHERSHOWER ,
    E_NUM_RESERVE7 ,
    E_NUM_RESERVE8 ,
    E_NUM_WERTHERLIGHTRAIN ,
    E_NUM_WERTHERMODERATERAIN ,
    E_NUM_WERTHERHEAVYRAIN 
};

typedef struct
{
    char *hour;
    char *min;
    char *sec;
}tNetTime;

void boot_page_run(void);
void LrtLvglSetTemperature(char *u8temp);
void LryLvglSetTime(char *data);
void LryLvglSetWeather(uint8_t cWeatherCode);
void LryLvglSetWeatherLarber(char *cWeatherName);
#endif