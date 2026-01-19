#include "usart.h"
#include "exfuns.h"
#include "sdio_sdcard.h"
#include "Data_Save.h"

//*************************************************************************************
uint8_t *Nor_Flash_path = "1:";
uint8_t *SD_path =  "0:";

Nor_Flash Flash_Fat_fs;
SD_Struct SD_Inf;

FRESULT DATA_res = FR_OK;

//*************************************************************************************
// * @brief       数据保存初始化函数
// * @note	      本函数用于初始化数据保存所需的文件系统
// * @param       无
// * @retval      无
//*************************************************************************************
void Data_Save_Init(void)
{	
	FRESULT res = FR_OK;
	char path[50] = "0:logfile.txt";
	res = f_mount(fs[0], "0", 1);  /* 挂载SD*/
	if(res != FR_OK)
	{
		SD_Inf.SD_insert = 0;
		my_printf("SD Fail \r\n");
	}
	else
	{
		SD_Inf.SD_insert = 1;
		DATA_res=exfuns_get_free(Nor_Flash_path, &Flash_Fat_fs.Toal_Num, &Flash_Fat_fs.Free_Num);
		DATA_res=exfuns_get_free(SD_path, &SD_Inf.Toal_Num, &SD_Inf.Free_Num);
		my_printf("NOR FLASH readly :free %5d FREE %5d \r\nSD readly :free %5d FREE %5d \r\n"\
				,Flash_Fat_fs.Toal_Num,Flash_Fat_fs.Free_Num,SD_Inf.Toal_Num,SD_Inf.Free_Num);
	}

}

//*************************************************************************** 
// * @brief       数据保存函数
// * @note	      本函数用于保存数据到SD卡
// * @param       无
// * @retval      无
//***************************************************************************
FIL file_error={0};

void Data_save(void)
{
	uint16_t write_length=0;
	
	char path[50] = "0:logfile.txt";

	//************ 记录SD文件系统信息 ************/
	if(SD_Inf.SD_insert)
	{
		
		DATA_res = f_open(&file_error, path, FA_WRITE | FA_OPEN_ALWAYS);
		if (DATA_res != FR_OK)
		{
			return;
		}
		/* Seek to end of file */
		DATA_res = f_lseek(&file_error, f_size(&file_error));
		if (DATA_res != FR_OK)
		{
			f_close(&file_error);
			return;
		}
		
		write_length = sizeof(Flash_Fat_fs);
		DATA_res = f_write(&file_error, &Flash_Fat_fs, write_length, &bw);
		if ((DATA_res != FR_OK) || (bw != write_length))
		{
			f_close(&file_error);
			return;
		}
		
		/* Force data to disk */
		f_sync(&file_error);
		f_close(&file_error);

	}
}
void Temp_Humit_Data_save(void)
{
	
}
void Can_Data_Save(void)
{
	
}

