#include "Error_Log_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "exfuns.h"
#include "sdio_sdcard.h"

#define Error_Log_TASK_STACK_SIZE    (2048)
#define Error_Log_TASK_PRIORITY      2u
TaskHandle_t   Error_Log_task_Handler;             /* 任务句柄 */
void Error_Log_task(void *pvParameters);                      /* 任务函数 */

FIL file_error={0};
FRESULT Error_Res=FR_OK;
//********************************************************* 
//@auto: Li
//@brief: Error_Log task function
//@param: none
//@return: none
//*********************************************************
Nor_Flash Flash_Fat_fs;
SD_Struct SD_Inf;

void Error_Log_task(void *pvParameters)
{
	
	uint16_t wite_lenth=0;
	FRESULT res = FR_OK;
	char path[50] = "0:logfile.txt";
	res = f_mount(fs[0], "0", 1);  /* 挂载SD*/
	if(res != FR_OK)
	{
		SD_Inf.SD_insert = 0;
		Error_Res=exfuns_get_free("1:", &Flash_Fat_fs.Toal_Num, &Flash_Fat_fs.Free_Num);
		Error_Res=exfuns_get_free("0:", &SD_Inf.Toal_Num, &SD_Inf.Free_Num);
	}
	else
	{
		SD_Inf.SD_insert = 1;
	}

  while(1)
  {

		//************ 记录SD文件系统信息 ************/
		if(SD_Inf.SD_insert)
		{
			Error_Res = f_open(&file_error, path, FA_WRITE | FA_OPEN_ALWAYS);
			if (Error_Res == FR_OK) 
			{
				/* Seek to end of the file to append data */
				Error_Res = f_lseek(&file_error, f_size(&file_error));
				wite_lenth=sizeof(Flash_Fat_fs);
				Error_Res =f_write(&file_error,&Flash_Fat_fs,wite_lenth,&bw);
				if (bw == wite_lenth)
				{
				//	my_printf("\n bw:%5d  wite_lenth:%5d  \r\n",bw,wite_lenth);
				}
				f_close(&file_error);
	
			}		
		}
		vTaskDelay(1000);
  }
}

//*********************************************************
//@auto: Li
//@brief: Create Error_Log task
//@param: none
//@return: none
//*********************************************************
void Error_Log_task_create(void)
{
    xTaskCreate((TaskFunction_t )Error_Log_task,(const char*)"Error_Log_task",(uint16_t)Error_Log_TASK_STACK_SIZE,(void*)NULL,(UBaseType_t)Error_Log_TASK_PRIORITY,(TaskHandle_t*  )&Error_Log_task_Handler);

}
