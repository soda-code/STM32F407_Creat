#include "GUIDEMO.h"

#include "GUI.h"
#include "WM.h"
#include "DIALOG.h"

#define ID_WINDOW_Homepage 					(GUI_ID_USER + 0x00) 
#define ID_BUTTON_Homepage_SleepReport 		(GUI_ID_USER + 0x01)
#define ID_BUTTON_Homepage_Ramp 			(GUI_ID_USER + 0x02)
#define ID_BUTTON_Homepage_Humidity		  	(GUI_ID_USER + 0x03)
#define ID_BUTTON_Homepage_MaskFit 			(GUI_ID_USER + 0x04)
#define ID_BUTTON_Homepage_Settings		  	(GUI_ID_USER + 0x05)
#define ID_TEXT_Homepage_Time 				(GUI_ID_USER + 0x06)
#define ID_TEXT_Homepage_Signal 			(GUI_ID_USER + 0x07)


WM_HWIN	 Main_Pic;
WM_HTIMER hTimer_Main; 
WM_HTIMER hTimer_Main_1;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
	{ WINDOW_CreateIndirect, "Window", ID_WINDOW_Homepage, 0, 0, 320, 240, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_Homepage_SleepReport, 18, 63, 140, 100, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_Homepage_Ramp, 170, 63, 140, 114, 0, 0x0, 0 },
};


static void _cbDialog(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  switch (pMsg->MsgId) 
  {
	  case WM_INIT_DIALOG://创建对话框后立即发送到对话框窗口。
			hItem = pMsg->hWin;
			WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00000000));

	    break;
			

			break;	
	 	 case WM_NOTIFY_PARENT://告知父窗口，其子窗口中发生了某些改变。
					
		    Id    = WM_GetId(pMsg->hWinSrc);//返回控件的ID
		    NCode = pMsg->Data.v;
    		switch(Id) 
			  {
			    case ID_BUTTON_Homepage_SleepReport: // Notifications sent by 'Button'
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED://此通知消息将在点击窗口后发送。			
						break;
						case WM_NOTIFICATION_RELEASED://此通知消息将在被点击的控件已被释放时发送。
						break;
					}
					break;
			    case ID_BUTTON_Homepage_Ramp: // Notifications sent by 'Button'
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
						break;
						case WM_NOTIFICATION_RELEASED:
						break;
					}
			      break;
			    case ID_BUTTON_Homepage_Humidity: // Notifications sent by 'Button'
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:	
						break;
						case WM_NOTIFICATION_RELEASED:

						break;
					}
					break;
				case ID_BUTTON_Homepage_MaskFit: // Notifications sent by 'Button'
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
						break;
						case WM_NOTIFICATION_RELEASED:

						break;
					}
			 	 break;
				case ID_BUTTON_Homepage_Settings: // Notifications sent by 'Button'
     			 	switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
						break;
						case WM_NOTIFICATION_RELEASED:

						break;
					}
				break;
			}
		break;
		default:
			WM_DefaultProc(pMsg);
		break;
	}
}



static WM_HWIN CreateWindow(void) 
{
	WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	return hWin;
}

void GUIDEMO_Main(void) 
{
  GUI_SetBkColor(GUI_BLACK);
  GUI_SetColor(GUI_WHITE);  
 	Main_Pic=CreateWindow();
  while (1) 
	{
    GUI_Exec();
  }
}

/*************************** End of file ****************************/

