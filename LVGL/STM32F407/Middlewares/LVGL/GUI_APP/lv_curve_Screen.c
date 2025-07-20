#include "LVGL/GUI_APP/lv_curve_Screen.h"
#include "LVGL/GUI_APP/lv_mainstart.h"

#include "lvgl.h"
#include <stdio.h>
#include "./SYSTEM/delay/delay.h"
#include "RTC_Read_app.h"
#include "usbd_cdc_interface.h"
#include "FreeRTOS.h"
#include "task.h"
#include "LVGL/GUI_APP/LV_Parment.h"
#include "dht11_app.h"

lv_coord_t example_data[] = 
{
    25, 26, 27, 29, 30, 31, 32, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 
    45, 46, 47, 47, 48, 48, 49, 49, 49, 50, 50, 50, 50, 50, 50, 50, 50, 49, 49, 
    49, 48, 48, 47, 47, 46, 45, 44, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 33, 
    32, 31, 30, 29, 27, 26, 25, 24, 22, 21, 20, 19, 17, 16, 15, 14, 13, 12, 11, 
};


/* 定义图片部件、滑块部件 */

lv_obj_t * Curve_screen;
lv_obj_t * Curve_Dis;
lv_obj_t * btn_turn;
lv_timer_t *Curve_time;
lv_chart_series_t *series_temp;
lv_chart_series_t *series_humi;

lv_obj_t *INF_CHAR;



static void  INF_CHAR_PAIN( lv_event_t *e)
{
	
    lv_event_code_t code = lv_event_get_code(e);                    /* 获取事件类型 */
	lv_obj_t * obj = lv_event_get_target(e);
	lv_area_t coords;

	if(obj==INF_CHAR)
	{
		lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(e);
		if (dsc->part == LV_PART_MAIN) 
		{

			lv_draw_label_dsc_t label_dsc;
			lv_draw_label_dsc_init(&label_dsc);
			label_dsc.color = lv_color_make(255, 0, 0); // 设置红色字体
			label_dsc.font = &myFont_16; // 设置字体
			lv_obj_get_coords(obj, &coords);
			lv_draw_label(dsc->draw_ctx, &label_dsc, &coords, "-TEMP", NULL);

			coords.x1=coords.x1+100;
			coords.x2=coords.x2+100;
			label_dsc.color = lv_color_make(0, 255, 0); // 设置红色字体
			lv_draw_label(dsc->draw_ctx, &label_dsc, &coords, "-HUMI", NULL);
		}

	}
}

//********************************内存监视
static void  curve_timer_task( lv_event_t *e) 
{
	static int test_num=0;
	test_num++;
	lv_chart_set_next_value(Curve_Dis, series_temp, dht11_data_inf.temp);
	//lv_chart_set_next_value(Curve_Dis, series_humi, dht11_data_inf.humi);
	lv_chart_set_next_value(Curve_Dis, series_humi, test_num);
	lv_obj_set_style_line_width(Curve_Dis, 0, LV_PART_ITEMS);  // 禁用连线
	lv_obj_set_style_size(Curve_Dis, 3, LV_PART_INDICATOR);    // 设置点的大小
	
	lv_mem_monitor_t mon;
	lv_mem_monitor(&mon);

}

static void btn_event_return(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);                    /* 获取事件类型 */

	lv_obj_t * obj = lv_event_get_target(e);

	if(obj==btn_turn)
	{
	    if(code == LV_EVENT_RELEASED) 
		{
	       	lv_obj_set_style_bg_color(btn_turn, lv_color_make(100, 128, 255), 0); // 设置蓝色背景
			Screen_INF.go_old_screen=1;
	    }
	}

}

/**
 * @brief  窗口实例
 * @param  无
 * @return 无
 */
 /* 示例数据 */

void LV_CURVE_Win(void)
{
    uint32_t i;

	//*************************************************创建曲线
    Curve_Dis = lv_chart_create(Curve_screen);
	lv_obj_set_pos(Curve_Dis, 40, 5);//**位置
    lv_obj_set_size(Curve_Dis, 280, 180);
	//lv_obj_align(Curve_Dis, LV_ALIGN_TOP_MID, 0, 0);
    lv_chart_set_type(Curve_Dis, LV_CHART_TYPE_LINE);   /*Show lines and points too*/

	lv_chart_set_axis_tick(Curve_Dis, LV_CHART_AXIS_PRIMARY_X, 10, 5, 10, 5, true, 20);
	lv_chart_set_axis_tick(Curve_Dis, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 6, 5, true, 50);
	lv_chart_set_range(Curve_Dis, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
	lv_chart_set_range(Curve_Dis, LV_CHART_AXIS_PRIMARY_X, 0, 20);
	lv_chart_set_point_count(Curve_Dis, 50);  // 最多显示 10 个点
	lv_chart_set_div_line_count(Curve_Dis, 10, 20);
	lv_obj_set_style_text_font(Curve_Dis, &myFont_12, 0); // 使用 Montserrat 字体，大小为 20
	lv_obj_set_style_line_opa(Curve_Dis,LV_OPA_30,LV_PART_MAIN);
	//*************************************************添加曲线数据源
    series_temp = lv_chart_add_series(Curve_Dis, lv_color_hex(0xFF0000), LV_CHART_AXIS_PRIMARY_Y);
	series_humi = lv_chart_add_series(Curve_Dis, lv_color_hex(0x00FF00), LV_CHART_AXIS_PRIMARY_Y);

	
	//*************************************************返回按键
	btn_turn = lv_btn_create(Curve_screen);                              /* 添加按钮 */
	lv_obj_set_pos(btn_turn, 5, 210);//**位置
	lv_obj_set_size(btn_turn, 40, 30);//**大小
	lv_obj_set_style_bg_color(btn_turn, lv_color_make(100, 128, 255), 0); // 设置蓝色背景
	lv_obj_t * label = lv_label_create(btn_turn);
	lv_label_set_text(label, "BACK");
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0); 
	lv_obj_set_style_text_font(label, &myFont_8, 0); // 使用 Montserrat 字体，大小为 20


	INF_CHAR= lv_label_create(Curve_screen);
	lv_obj_set_pos(INF_CHAR, 100, 210);//**位置
	lv_obj_set_size(INF_CHAR, 200, 25);//**大小
	lv_label_set_text(INF_CHAR, "");
	lv_obj_add_event_cb(INF_CHAR, INF_CHAR_PAIN, LV_EVENT_DRAW_PART_BEGIN, NULL);
    lv_obj_add_event_cb(btn_turn, btn_event_return, LV_EVENT_RELEASED, NULL);                       /* 添加事件 */
}

/**
 * @brief  LVGL演示
 * @param  无
 * @return 无
 */
void CURVE_DISPLAY(void)
{
    //*******************************************************************创建背景
    Screen_INF.Screen_Pointer=CURVE_SCREEN;
    Curve_screen = lv_obj_create(lv_scr_act());
	lv_obj_set_size(Curve_screen, LV_HOR_RES, LV_VER_RES);
	lv_obj_set_style_pad_all(Curve_screen, 0, LV_PART_MAIN); // 设置所有方向的内边距为 0
	lv_obj_set_style_border_width(Curve_screen, 0, LV_PART_MAIN); // 移除边框
	
	Curve_time=lv_timer_create(curve_timer_task, 500, NULL);

	LV_CURVE_Win();
	while(1)
	{
		if(Screen_INF.go_old_screen)
		{
			lv_timer_del(Curve_time);
			lv_obj_del(Curve_screen);   // 删除旧页面
			Screen_INF.go_old_screen=0;
			Screen_INF.Screen_Pointer=MAIN_SCREEN;
			Curve_screen=0;
			Curve_time=0;
			break;
		}
		
		lv_timer_handler(); /* LVGL计时器 */
		vTaskDelay(5);
		
	}
}
