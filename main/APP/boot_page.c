#include "boot_page.h"
#include "lv_conf_template.h"
#include "lvgl.h"

lv_obj_t *ui_home;
void ui_event_header(lv_event_t *e);
lv_obj_t *ui_header;
lv_obj_t *ui_body;
lv_obj_t *ui_home1;
lv_obj_t *ui_camera;
lv_obj_t *ui_cameratext;
lv_obj_t *ui_clock;
lv_obj_t *ui_clocktext;
lv_obj_t *ui_gallery;
lv_obj_t *ui_gallerytext;
lv_obj_t *ui_music;
lv_obj_t *ui_musictext;
lv_obj_t *ui_mobile;

lv_obj_t *ui_settings;
lv_obj_t *ui_settingstext;

lv_obj_t *ui_setting;
lv_obj_t *ui_settingheader;
lv_obj_t *ui_settingmobile;
LV_FONT_DECLARE(WORD);

static void anim_ready_cb(lv_anim_t *a)
{
    obj_set_anim(a->var, 110, 320, 500, 2000, anim_set_y_cb, lv_obj_del_anim_ready_cb, lv_anim_path_ease_in);
}

void boot_page_run(void)
{
    // //LV_IMG_DECLARE(img);
    LV_IMG_DECLARE(back);
    //LV_IMG_DECLARE(ui_img_bg_png);
    //  /* 动态获取屏幕大小 */
    lv_coord_t scr_act_width = lv_obj_get_width(lv_scr_act());
    lv_coord_t scr_act_height = lv_obj_get_height(lv_scr_act());

    // lv_obj_t *scr = lv_scr_act();
   
    // lv_obj_t *image = lv_img_create(scr);
    // lv_img_set_src(image, &ui_img_bg_png);

    //lv_obj_t* parent_obj = lv_obj_create(scr); /* 创建父对象 */
    // lv_obj_set_size(parent_obj, scr_act_width, scr_act_height * 6/50);
    //lv_obj_t *label_time = lv_label_create(parent_obj);
    //lv_obj_set_style_text_font(label_time, &lv_font_montserrat_14,LV_STATE_DEFAULT);
    //lv_label_set_text(label_time,"12:23");

    // lv_obj_t* lv_bar1 = lv_bar_create(image); 
    // lv_obj_set_size(lv_bar1, 30, 220); 
    // lv_obj_set_pos(lv_bar1,600,150);
    // lv_bar_set_value(lv_bar1,20,LV_ANIM_ON);
    // lv_bar_set_range(lv_bar1,0,100);
    // lv_obj_set_style_anim_time(lv_bar1,500,0);
    // lv_obj_set_style_bg_color(lv_bar1,lv_color_hex(0xFF2424),LV_PART_INDICATOR|LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(lv_bar1,lv_color_hex(0xFEB0A4),LV_PART_MAIN|LV_STATE_DEFAULT);


    // lv_obj_t* lv_bar2 = lv_bar_create(image); 
    // lv_obj_set_size(lv_bar2, 30, 220); 
    // lv_obj_set_pos(lv_bar2,700,150);
    // lv_bar_set_value(lv_bar2,20,LV_ANIM_ON);
    // lv_bar_set_range(lv_bar2,0,100);
    // lv_obj_set_style_anim_time(lv_bar2,500,0);
    
    // lv_obj_t *label_time = lv_obj_create(scr);
    // lv_obj_set_size(label_time, scr_act_width, scr_act_height * 1/10);
    // lv_obj_set_style_bg_opa(label_time,100,0);

    // lv_obj_t *label_time1 = lv_obj_create(label_time);
    // lv_obj_set_size(label_time1, scr_act_width, scr_act_height * 1/15);
    //lv_obj_t *label = lv_label_create(label_time);
    
     //lv_obj_set_size(label_time, 200, 100);
    // lv_obj_set_size(label, 10, 10);
    // lv_obj_set_style_bg_opa(label,250,0);
    // lv_obj_set_style_bg_color(label, lv_color_hex(0xFF2424), LV_STATE_DEFAULT);
    //  lv_label_set_text(label,"12:23");
    //lv_obj_set_size(label, scr_act_width, scr_act_height * 1/15);
    //lv_obj_align(label,LV_ALIGN_RIGHT_MID,0,0);
    //lv_obj_set_size(label_time, scr_act_width, scr_act_height * 1/10);
    //lv_obj_set_style_text_font(label_time, &lv_font_montserrat_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    //lv_label_set_text(label_time,LV_SYMBOL_WIFI);
     
    // lv_obj_align(image, LV_ALIGN_TOP_MID, 0, -100);

    // obj_set_anim(image, -100, 110, 800, 1000, anim_set_y_cb, anim_ready_cb, lv_anim_path_bounce);

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                              false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);

    ui_home = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_home, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_img_src(ui_home, &back, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_home, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_header = lv_obj_create(ui_home);
    lv_obj_set_width(ui_header,scr_act_width);
    lv_obj_set_height(ui_header, 50);
    lv_obj_set_align(ui_header, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_header, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(ui_header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_header, lv_color_hex(0x141414), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_header, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_blend_mode(ui_header, LV_BLEND_MODE_NORMAL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_header, 20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_mobile = lv_label_create(ui_home);
    lv_obj_set_width(ui_mobile, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_mobile, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_mobile, 10);
    lv_obj_set_y(ui_mobile, 7);
    lv_label_set_text(ui_mobile, LV_SYMBOL_WIFI);
    lv_obj_clear_flag(ui_mobile, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                                     LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags
    lv_obj_set_style_text_color(ui_mobile, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_mobile, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_setting = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_setting, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(ui_setting, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_setting, lv_color_hex(0xE6E6E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_setting, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_setting, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_settingheader = lv_obj_create(ui_setting);
    lv_obj_set_width(ui_settingheader, scr_act_width);
    lv_obj_set_height(ui_settingheader, 100);
    lv_obj_set_align(ui_settingheader, LV_ALIGN_TOP_MID);
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

    lv_disp_load_scr(ui_home);

}

