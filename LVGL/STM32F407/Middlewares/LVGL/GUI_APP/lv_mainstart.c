#include "LVGL/GUI_APP/lv_mainstart.h"
#include "lvgl.h"
#include <stdio.h>
#include "./SYSTEM/delay/delay.h"
#include "RTC_Read_app.h"
#include "usbd_cdc_interface.h"
#include "LVGL/GUI_APP/lv_curve_Screen.h"
#include "FreeRTOS.h"
#include "task.h"
#include "LVGL/GUI_APP/LV_Parment.h"

/* 获取当前活动屏幕的宽高 */
#define scr_act_width()  lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())


Screen_STRUCT Screen_INF;

LV_IMG_DECLARE(home);                     /* 声明图片 */

static lv_obj_t *TIMER_DIS; 
/* 定义图片部件、滑块部件 */
static lv_obj_t *img;

lv_timer_t *main_timer;
lv_timer_t *Screen_Monit_timer;

lv_obj_t *btn_close;
lv_obj_t *btn_close1;
lv_obj_t *btn_close2;
lv_obj_t *btn_close3;
lv_obj_t * main_screen;


uint8_t time_buf[50]={0}; 




void update_title(lv_obj_t * label)
{
	sprintf((char*)time_buf,"%02d:%02d:%02d",RTC_Info.hour,RTC_Info.min,RTC_Info.sec);
    lv_label_set_text(label, (char*)time_buf);
}

//********************************内存监视
static void  timer_task( lv_event_t *e) 
{
	lv_mem_monitor_t mon;
	update_title(TIMER_DIS);
	lv_mem_monitor(&mon);

    //usb_printf("Timer task called: %d\n", count++);
}



/**
 * @brief  按钮事件回调
 * @param  *e ：事件相关参数的集合，它包含了该事件的所有数据
 * @return 无
 */
static void btn_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);                    /* 获取事件类型 */

	lv_obj_t * obj = lv_event_get_target(e);

	if(obj==btn_close)
	{
	    if(code == LV_EVENT_RELEASED) 
		{
	       	lv_obj_set_style_bg_color(btn_close, lv_color_make(100, 128, 255), 0); // 设置蓝色背景
			Screen_INF.go_new_screen=1;
			
	    }
	}

}

/**
 * @brief  窗口实例
 * @param  无
 * @return 无
 */

 void LV_Main_Win(void)
{
 	//************************************************按键创建
  	btn_close = lv_btn_create(main_screen);                              /* 添加按钮 */
	lv_obj_set_pos(btn_close, 10, 195);//**位置
	lv_obj_set_size(btn_close, 68, 40);//**大小
	lv_obj_set_style_bg_color(btn_close, lv_color_make(100, 128, 255), 0); // 设置蓝色背景
	lv_obj_t * label = lv_label_create(btn_close);
	lv_label_set_text(label, "CURVE");
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0); 
	lv_obj_set_style_text_font(label, &myFont_12, 0); // 使用 Montserrat 字体，大小为 20

	
  	btn_close1 = lv_btn_create(main_screen);                              /* 添加按钮 */
	lv_obj_set_pos(btn_close1, 87, 195);//**位置
	lv_obj_set_size(btn_close1, 68, 40);//**大小
	lv_obj_set_style_bg_color(btn_close1, lv_color_make(100, 128, 255), 0); // 设置蓝色背景

	 btn_close2 = lv_btn_create(main_screen);                              /* 添加按钮 */
	lv_obj_set_pos(btn_close2, 165, 195);//**位置
	lv_obj_set_size(btn_close2, 68, 40);//**大小
	lv_obj_set_style_bg_color(btn_close2, lv_color_make(100, 128, 255), 0); // 设置蓝色背景


	btn_close3 = lv_btn_create(main_screen);                              /* 添加按钮 */
	lv_obj_set_pos(btn_close3, 243, 195);//**位置
	lv_obj_set_size(btn_close3, 68, 40);//**大小
	lv_obj_set_style_bg_color(btn_close3, lv_color_make(100, 128, 255), 0); // 设置蓝色背景


    TIMER_DIS = lv_label_create(main_screen);

    lv_obj_align(TIMER_DIS, LV_ALIGN_TOP_MID, 0, 0);
	lv_obj_set_style_text_font(TIMER_DIS, &myFont_26, 0); // 使用 Montserrat 字体，大小为 20
	lv_label_set_text(TIMER_DIS, "");

    lv_obj_add_event_cb(btn_close, btn_event_cb, LV_EVENT_RELEASED, NULL);                       /* 添加事件 */
    lv_obj_add_event_cb(btn_close1, btn_event_cb, LV_EVENT_RELEASED, NULL);                       /* 添加事件 */
    lv_obj_add_event_cb(btn_close2, btn_event_cb, LV_EVENT_RELEASED, NULL);                       /* 添加事件 */
    lv_obj_add_event_cb(btn_close3, btn_event_cb, LV_EVENT_RELEASED, NULL);                       /* 添加事件 */
	lv_refr_now(NULL);
    lv_obj_add_event_cb(btn_close, btn_event_cb, LV_EVENT_RELEASED, NULL);                       /* 添加事件 */


    
}

/**
 * @brief  LVGL演示
 * @param  无
 * @return 无
 */
void HOME_DISPLAY(void)
{
	//********************************************主界面********************
	
	Screen_INF.Screen_Pointer=MAIN_SCREEN;
    main_screen = lv_obj_create(lv_scr_act());
	lv_obj_set_size(main_screen, LV_HOR_RES, LV_VER_RES);
	lv_obj_set_style_pad_all(main_screen, 0, LV_PART_MAIN); // 设置所有方向的内边距为 0
	lv_obj_set_style_border_width(main_screen, 0, LV_PART_MAIN); // 移除边框 
	main_timer=lv_timer_create(timer_task, 500, NULL);
    LV_Main_Win();  
	while(1)
	{
		if(Screen_INF.go_new_screen)
		{
			lv_timer_del(main_timer);
			CURVE_DISPLAY();
			Screen_INF.go_new_screen=0;
			main_timer=lv_timer_create(timer_task, 500, NULL);
		}
		
		lv_timer_handler(); /* LVGL计时器 */
		vTaskDelay(5);
	}
	//CURVE_DISPLAY();

}
